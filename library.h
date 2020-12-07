#ifndef TEMPLATE_GRAPHS_LIBRARY_H
#define TEMPLATE_GRAPHS_LIBRARY_H

#include <array>
#include <utility>


namespace template_graphs {

    using size_t = std::size_t;

    struct UntypedDirectedEdge {
        size_t x;
        size_t y;
    };

    template<class VertexType, class EdgeType>
    class DynamicAdjacencyMatrixGraph {
    };

    template<class VertexType, class EdgeType>
    class DynamicAdjacencyListGraph {
    };

    template<class VertexType, class EdgeType>
    class DynamicPointerListGraph {
    };

    template<class VertexType, class EdgeType, size_t VertexCount>
    class StaticAdjacencyMatrixGraph {
    };

    template<class VertexType, class EdgeType, size_t VertexCount, size_t EdgeCount, std::array<UntypedDirectedEdge, EdgeCount> Structure>
    class StaticAdjacencyListGraph {
    };

    template<class VertexType, class EdgeType, size_t VertexCount, size_t EdgeCount, std::array<UntypedDirectedEdge, EdgeCount> Structure>
    class StaticPointerListGraph {
    };

}

#endif //TEMPLATE_GRAPHS_LIBRARY_H
