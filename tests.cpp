#include "library.h"

using namespace template_graphs;

#include <iostream>
#include <string>
#include <typeinfo>
#include <variant>


int main() {
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

    StaticAdjacencyMatrixGraph<std::function<void(int)>, std::monostate, 3> composite;
    composite.Vertex(0) = [](int a) { std::cout << "Quack!\n"; };
    composite.Vertex(1) = [](int a) { std::cout << "Grok " << a << "\n"; };
    composite.Vertex(2) = [](int a) { std::cout << "Mrok " << a << "\n"; };
    composite.AddEdge({}, 0, 1);
    composite.AddEdge({}, 0, 2);

    CompositeAdapter ca(composite, 0);
    ca.Invoke(5);

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

    return 0;
}