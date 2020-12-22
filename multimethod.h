#ifndef TEMPLATE_GRAPHS_MULTIMETHOD_H
#define TEMPLATE_GRAPHS_MULTIMETHOD_H

#include "concepts.h"
#include "iterators.h"

#include <concepts>
#include <exception>
#include <iterator>
#include <ranges>
#include <typeinfo>
#include <typeindex>

namespace template_graphs {

    template<typename GraphType> requires Graph<GraphType> && std::same_as<typename GraphType::VertexType, std::type_index>
    class MultimethodAdapter {
    private:
        GraphType &graph_;
    public:
        explicit MultimethodAdapter(GraphType &graph) : graph_(graph) {};

        template<typename T>
        requires Bifunctor<typename GraphType::EdgeType, T &, T &>
        auto Invoke(T &first, T &second) {
            auto rn = IndexRange(graph_);

            auto it1 = std::find(rn.begin(), rn.end(), std::type_index(typeid(first)));
            if (it1 == rn.end())
                throw std::invalid_argument("Type error: argument of invalid type passed to multimethod");

            auto it2 = std::find(rn.begin(), rn.end(), std::type_index(typeid(second)));
            if (it2 == rn.end())
                throw std::invalid_argument("Type error: argument of invalid type passed to multimethod");

            int idx1 = std::distance(rn.begin(), it1);
            int idx2 = std::distance(rn.begin(), it2);

            if (!graph_.HasEdge(idx1, idx2))
                throw std::invalid_argument("Type error: multimethod has no overload for passed argument types");

            return graph_.Edge(idx1, idx2)(first, second);
        }

    };

}

#endif //TEMPLATE_GRAPHS_MULTIMETHOD_H
