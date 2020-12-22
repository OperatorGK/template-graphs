#ifndef TEMPLATE_GRAPHS_COMPOSITE_H
#define TEMPLATE_GRAPHS_COMPOSITE_H

#include "concepts.h"
#include "iterators.h"


namespace template_graphs {
    
    template<typename GraphType> requires Graph<GraphType>
    class CompositeAdapter {
    private:
        GraphType &graph_;
        size_t root_;

    public:
        explicit CompositeAdapter(GraphType &graph, size_t root) : graph_(graph), root_(root) {};

        template<typename T>
        requires Functor<typename GraphType::VertexType, T>
        void Invoke(T val) {
            auto rn = BFSRange(graph_, root_);
            for (auto it = rn.end() - 1; it != rn.begin() - 1; --it)
                (*it)(val);
        }
    };
}

#endif //TEMPLATE_GRAPHS_COMPOSITE_H
