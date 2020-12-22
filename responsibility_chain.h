#ifndef TEMPLATE_GRAPHS_RESPONSIBILITY_CHAIN_H
#define TEMPLATE_GRAPHS_RESPONSIBILITY_CHAIN_H

#include "concepts.h"
#include "iterators.h"

namespace template_graphs {

    template<typename GraphType>
    class ResponsibilityChainAdapter {
    private:
        GraphType &graph_;
        size_t root_;
    public:
        explicit ResponsibilityChainAdapter(GraphType &graph, size_t root) : graph_(graph), root_(root) {};

        template<typename T>
        requires Predicate<typename GraphType::VertexType, T>
        void Invoke(T val) {
            auto rn = BranchRange(graph_, root_);
            for (auto p : rn)
                if (p(val)) break;
        }

    };

}

#endif //TEMPLATE_GRAPHS_RESPONSIBILITY_CHAIN_H
