#ifndef TEMPLATE_GRAPHS_GRAPHS_H
#define TEMPLATE_GRAPHS_GRAPHS_H

#include <algorithm>
#include <array>
#include <optional>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>

namespace template_graphs {

    using size_t = std::size_t;

    template<class VertexT, class EdgeT>
    class DynamicAdjacencyMatrixGraph {
    public:
        using VertexType = VertexT;
        using EdgeType = EdgeT;
    private:
        using ArrowType = std::optional<EdgeType>;
        std::vector<VertexType> vertices{};
        std::vector<std::vector<ArrowType>> edges{};

    public:
        size_t VertexCount() { return vertices.size(); }

        VertexType &Vertex(size_t i) { return vertices[i]; }

        auto Neighbors(size_t i) {
            return std::views::iota(0u, vertices.size()) | std::views::filter(
                    [this, i](size_t j) { return bool(edges[i][j]); }
            );
        }

        bool HasEdge(size_t x, size_t y) { return bool(edges[x][y]); }

        EdgeType &Edge(size_t x, size_t y) { return *(edges[x][y]); }

        EdgeType &AddEdge(EdgeType e, size_t x, size_t y) {
            edges[x][y] = std::make_optional(std::move(e));
            return *(edges[x][y]);
        }

        void RemoveEdge(size_t x, size_t y) {
            edges[x][y] = std::nullopt;
        }

        VertexType &AddVertex(VertexType v, size_t i) {
            vertices.insert(vertices.begin() + i, std::move(v));
            for (auto &li : edges) li.insert(li.begin() + i, std::nullopt);

            std::vector<ArrowType> li(vertices.size(), std::nullopt);
            edges.insert(edges.begin() + i, li);

            return vertices[i];
        }

        void RemoveVertex(size_t i) {
            vertices.erase(vertices.begin() + i);
            edges.erase(edges.begin() + i);
            for (auto &li : edges) li.erase(li.begin() + i);
        }
    };

    template<class VertexT, class EdgeT>
    class DynamicAdjacencyListGraph {
    public:
        using VertexType = VertexT;
        using EdgeType = EdgeT;

    private:
        using ArrowType = std::pair<size_t, EdgeType>;
        std::vector<VertexType> vertices;
        std::vector<std::vector<ArrowType>> edges;

    public:
        size_t VertexCount() { return vertices.size(); }

        VertexType &Vertex(size_t i) { return vertices[i]; }

        auto Neighbors(size_t i) { return edges[i] | std::views::transform([](auto i) { return i.first; }); }

        bool HasEdge(size_t x, size_t y) {
            return std::ranges::find_if(edges[x], [y](const ArrowType &a) { return a.first == y; }) != edges[x].end();
        }

        EdgeType &Edge(size_t x, size_t y) {
            return std::ranges::find_if(edges[x], [y](const ArrowType &a) { return a.first == y; })->second;
        }

        EdgeType &AddEdge(EdgeType e, size_t x, size_t y) {
            edges[x].push_back({y, std::move(e)});
            return edges[x].back().second;
        }

        void RemoveEdge(size_t x, size_t y) {
            edges[x].erase(std::ranges::find_if(edges[x], [y](const ArrowType &a) { return a.first == y; }));
        }

        VertexType &AddVertex(VertexType v, size_t i) {
            vertices.insert(vertices.begin() + i, std::move(v));
            std::vector<ArrowType> li{};
            edges.insert(edges.begin() + i, li);
            return vertices[i];
        }

        void RemoveVertex(size_t i) {
            vertices.erase(vertices.begin() + i);
            edges.erase(edges.begin() + i);
            for (auto &li : edges) {
                auto it = std::ranges::find_if(li, [i](const ArrowType &a) { return a.first == i; });
                if (it != li.end()) li.erase(it);
            }
        }
    };


    template<class VertexT, class EdgeT, size_t VertexC>
    class StaticAdjacencyMatrixGraph {
    public:
        using VertexType = VertexT;
        using EdgeType = EdgeT;

    private:
        using ArrowType = std::optional<EdgeT>;
        std::array<VertexType, VertexC> vertices;
        std::array<std::array<ArrowType, VertexC>, VertexC> edges;

    public:
        size_t VertexCount() { return vertices.size(); }

        VertexType &Vertex(size_t i) { return vertices[i]; }

        auto Neighbors(size_t i) {
            return std::views::iota(0u, vertices.size()) | std::views::filter(
                    [this, i](size_t j) { return bool(edges[i][j]); }
            );
        }

        bool HasEdge(size_t x, size_t y) { return bool(edges[x][y]); }

        EdgeType &Edge(size_t x, size_t y) { return *(edges[x][y]); }

        EdgeType &AddEdge(EdgeType e, size_t x, size_t y) {
            edges[x][y] = std::make_optional(std::move(e));
            return *(edges[x][y]);
        }

        void RemoveEdge(size_t x, size_t y) {
            edges[x][y] = std::nullopt;
        }
    };

}

#endif //TEMPLATE_GRAPHS_GRAPHS_H
