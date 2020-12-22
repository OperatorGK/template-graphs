#ifndef TEMPLATE_GRAPHS_CONCEPTS_H
#define TEMPLATE_GRAPHS_CONCEPTS_H

#include <concepts>
#include <ranges>

namespace template_graphs {

    using size_t = std::size_t;

    template<class F, class T>
    concept Functor = requires(F f, T t) { f(t); };

    template<class F, class T1, class T2>
    concept Bifunctor = requires(F f, T1 t1, T2 t2) { f(t1, t2); };

    template<class F, class T>
    concept Predicate = requires(F f, T t) {
        { f(t) } -> std::same_as<bool>;
    };

    template<class GraphType>
    concept Graph = requires(GraphType graph, size_t index, size_t i2) {
        typename GraphType::VertexType;
        typename GraphType::EdgeType;
        { graph.VertexCount() } -> std::same_as<size_t>;
        { graph.Vertex(index) } -> std::same_as<typename GraphType::VertexType &>;
        { graph.Neighbors(index) } -> std::ranges::range;
        { graph.HasEdge(index, i2) } -> std::same_as<bool>;
        { graph.Edge(index, i2) } -> std::same_as<typename GraphType::EdgeType &>;
    };

}

#endif //TEMPLATE_GRAPHS_CONCEPTS_H
