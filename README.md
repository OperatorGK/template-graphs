# template_graphs — C++-библиотека для работы с шаблонными графами

## Требования к компилятору и структура

Библиотека не имеет внешних зависимостей, кроссплатформенна и является header-only. Для использования необходим компилятор, поддерживающий стандарт C++ 20, тестирование проводилось с помощью GCC v10.2.0.

Библиотека разделена на 7 хэдеров, один из которых — `library.h` — содержит в себе только включения 6 остальных и предоставляется для удобства

## `concepts.h`

В этом файле описаны концепты (С++ 20), используемые библиотекой.

`concept Functor<F,T>` описывает объекты, для которых определено выражение `f(t)`

`concept Bifunctor<F, T1, T2>` описывает объекты, для которых определено выражение `f(t1, t2)`

`concept Predicate<F, T>` описывает объекты, для выражение `f(t)` имеет тип `bool`

`concept Graph<T>` описывает графы, доступные в этой библиотеке и предоставляет интерфейс к ним. Графы имеют внутренние типы `VertexType` и `EdgeType` и для них определены следующие операции:

- `.VertexCount()`
- `.Vertex(index)`
- `.Neighbors(index)`
- `.HasEdge(start, end)`
- `.Edge(start, end)`

Вершины графа индексированы от 0 до количества вершин - 1.

## `graphs.h`

В этой файле объявлены типы `StaticAdjacencyMatrixGraph<VertexT, EdgeT, VertexC>`, `DynamicAdjacencyMatrixGraph<VertexT, EdgeT>`, `DynamicAdjacencyListGraph<VertexT, EdgeT>`, которые реализуют графы на матрицах смежности с фиксированным и переменным числом вершин и графы на списках смежности с переменным числом вершин соотвественно. Все графы являются ориентированными.

Для всех графов типы вершин и ребер должны быть `MoveConstructible`, копирование графов определено если они `CopyConstructible`.

 Каждый граф поддерживает концепт `Graph<GraphType<...>>`, а так же методы `.AddEdge(edge, start, end)`, `.RemoveEdge(start, end)`; корректность работы этих методов при попытке добавить существующее ребро или удалить несуществующее не гарантируется.

Dynamic-графы дополнительно имеют методы `.AddVertex(vertex, index)` и `.RemoveVertex(index)`; их вызовы должны соблюдать индексацию графа.

## `iterators.h`

В этом файле объявлены следующие методы, возвращающие range итераторов на вершины графа:

- `VertexRange<GraphType> IndexRange(GraphType &graph)` — обход вершин в порядке индекса
- `VertexRange<GraphType> DFSRange(GraphType &graph, size_t start)` — обход вершин в глубину из точки
- `VertexRange<GraphType> BFSRange(GraphType &graph, size_t start)` — обход вершин в ширину из точки
- `VertexRange<GraphType> BranchRange(GraphType &graph, size_t start)` — обход из вершины по пути до его конца; если из вершины исходит более одного пути, то его выбор не определен; если какой-то путь зацикливается, то операция не определена.

Пример использования:

```cpp
DynamicAdjacencyListGraph<std::string, std::monostate> alphabet;
alphabet.AddVertex("alpha", 0);
alphabet.AddVertex("beta", 1);
alphabet.AddVertex("gamma", 2);
alphabet.AddVertex("delta", 3);
alphabet.AddVertex("epsilon", 4);

alphabet.AddEdge({}, 0, 1);
alphabet.AddEdge({}, 1, 2);
alphabet.AddEdge({}, 0, 3);
alphabet.AddEdge({}, 3, 4);

for (auto x : DFSRange(alphabet, 0)) std::cout << x << "\n";
```

выведет либо

```
alpha
beta
gamma
delta
epsilon
```

либо

```
alpha
delta
epsilon
beta
gamma
```

## `multimethod.h`

Этот хэдер содержит единственный класс `MultimethodAdapter`, который реализует мультиметод на основе графа, вершины которого имеют тип `std::type_index`, а ребра — бифункторы по какому-то типу `T`. Он имеет конструктор по ссылке на граф и единственный метод `Invoke<T>(T a, T b)`, вызывающий мультиметод. Если для пары типов в графе нет перегрузки, поднимается исключение `std::invalid_argument`.

Пример использования:

```cpp
struct Animal {
        virtual ~Animal() = default;
    };

    struct Cat : public Animal {
    };

    struct Dog : public Animal {
    };

    Cat cat;
    Dog dog;

    DynamicAdjacencyMatrixGraph<std::type_index, std::function<void(Animal &, Animal &)>> greeter;
    greeter.AddVertex(std::type_index(typeid(cat)), 0);
    greeter.AddVertex(std::type_index(typeid(dog)), 1);
    greeter.AddEdge([](Animal &a, Animal &b) { std::cout << "Meow! Bark!\n"; }, 0, 1);
    greeter.AddEdge([](Animal &a, Animal &b) { std::cout << "Bark! Meow!\n"; }, 1, 0);

    MultimethodAdapter mm(greeter);

    mm.Invoke<Animal>(cat, dog);
    mm.Invoke<Animal>(dog, cat);

    try {
        mm.Invoke<Animal>(cat, cat);
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
```

выведет:

```
Meow! Bark!
Bark! Meow!
Type error: multimethod has no overload for passed argument types
```

## `composite.h`

Этот хэдер содержит единственный класс `CompositeAdapter`, который реализует паттерн Компоновщик на основе графа. Вершины графа должны быть функторами по какому-то типу `T`.  Класс имеет конструктор по ссылке на граф и индексу корневого объекта в иерархии. Он имеет единственный метод `.Invoke<T>(T x)`, который вызывает каждый функтор в иерархии, начиная с листьев и заканчивая корнем. 

Пример использования:

```cpp
StaticAdjacencyMatrixGraph<std::function<void(int)>, std::monostate, 3> composite;
composite.Vertex(0) = [](int a) { std::cout << "Quack!\n"; };
composite.Vertex(1) = [](int a) { std::cout << "Grok " << a << "\n"; };
composite.Vertex(2) = [](int a) { std::cout << "Mrok " << a << "\n"; };
composite.AddEdge({}, 0, 1);
composite.AddEdge({}, 0, 2);

CompositeAdapter ca(composite, 0);
ca.Invoke(5);
```

выведет:

```
Mrok 5
Grok 5
Quack!
```

## `responsibility_chain.h`

Этот хэдер содержит единственный класс `ResponsibilityChain`, который реализует паттерн Цепочка Ответственностей на основе графа. Вершины графа должны быть предикатами по какому-то типу `T`. Он имеет единственный метод `.Invoke<T>(T x)`, который вызывает каждый предикат в цепочке по порядку совпадающему с порядком `BranchRange(graph, root)`, пока какой-то из предикатов не вернет `true`.

Пример использования:

```cpp
DynamicAdjacencyMatrixGraph<std::function<bool(int)>, std::monostate> chain;
chain.AddVertex([](int x) {
    if (x != 5) return false;
    std::cout << "Found it! 5!\n";
    return true;
}, 0);

chain.AddVertex([](int x) {
    if (x != 4) return false;
    std::cout << "Found it! 4!\n";
    return true;
}, 0);

chain.AddVertex([](int x) {
    if (x != 3) return false;
    std::cout << "Found it! 3!\n";
    return true;
}, 0);

chain.AddEdge({}, 0, 1);
chain.AddEdge({}, 1, 2);

ResponsibilityChainAdapter cc(chain, 0);
cc.Invoke(4);
```

выведет:

```
Found it! 4!
```