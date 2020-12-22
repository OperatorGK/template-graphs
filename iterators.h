#ifndef TEMPLATE_GRAPHS_ITERATORS_H
#define TEMPLATE_GRAPHS_ITERATORS_H

#include <algorithm>
#include <concepts>
#include <functional>
#include <iterator>
#include <numeric>
#include <ranges>
#include <memory>
#include <queue>
#include <stack>

#include "concepts.h"


namespace template_graphs {

    using size_t = std::size_t;

    template<typename GraphType> requires Graph<GraphType>
    class VertexRange {
    private:
        GraphType &graph_ = nullptr;
        std::vector<size_t> indices_{};

    public:
        class Iterator {
        private:
            VertexRange *parent_ = nullptr;
            size_t pos_ = 0;

            Iterator(VertexRange *parent, size_t pos) : parent_(parent), pos_(pos) {};

            friend Iterator VertexRange::begin();

            friend Iterator VertexRange::end();

        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = typename GraphType::VertexType;
            using reference_type = typename GraphType::VertexType &;
            using difference_type = long;
            using pointer_type = typename GraphType::VertexType *;

            Iterator() {};

            bool operator==(const Iterator &other) const {
                return pos_ == other.pos_ && parent_ == other.parent_;
            }

            reference_type operator*() {
                return parent_->graph_.Vertex(parent_->indices_[pos_]);
            }

            pointer_type operator->() {
                return &parent_->graph_.Vertex(parent_->indices_[pos_]);
            }

            Iterator &operator++() {
                ++pos_;
                return *this;
            }

            Iterator operator++(int) {
                auto ret = *this;
                ++pos_;
                return ret;
            }

            Iterator &operator--() {
                --pos_;
                return *this;
            }

            Iterator operator--(int) {
                auto ret = *this;
                --pos_;
                return ret;
            }

            Iterator &operator+=(long offset) {
                pos_ += offset;
                return *this;
            }

            Iterator &operator-=(long offset) {
                pos_ -= offset;
                return *this;
            }

            friend Iterator operator+(Iterator a, long n) {
                a += n;
                return a;
            }

            friend Iterator operator+(long n, Iterator a) {
                a += n;
                return a;
            }

            friend Iterator operator-(Iterator a, long n) {
                a -= n;
                return a;
            }

            friend Iterator operator-(long n, Iterator a) {
                a -= n;
                return a;
            }

            friend long operator-(Iterator a, Iterator b) {
                return a.pos_ - b.pos_;
            }

            reference_type operator[](long n) {
                auto a = this + n;
                return *a;
            }


        };

    private:
        VertexRange(GraphType &graph, std::vector<size_t> indices) : graph_(graph), indices_(std::move(indices)) {}

    public:
        VertexRange() {};

        Iterator begin() {
            return Iterator(this, 0);
        }

        Iterator end() {
            return Iterator(this, indices_.size());
        }

        friend VertexRange IndexRange<>(GraphType &graph);

        friend VertexRange DFSRange<>(GraphType &graph, size_t start);

        friend VertexRange BFSRange<>(GraphType &graph, size_t start);

        friend VertexRange BranchRange<>(GraphType &graph, size_t start);

    };

    template<typename GraphType>
    requires Graph<GraphType>
    VertexRange<GraphType> IndexRange(GraphType &graph) {
        std::vector<size_t> indices(graph.VertexCount());
        std::iota(indices.begin(), indices.end(), 0);
        return {graph, indices};
    }

    template<typename GraphType>
    requires Graph<GraphType>
    VertexRange<GraphType> DFSRange(GraphType &graph, size_t start) {
        std::vector<size_t> indices;

        std::vector<bool> visited(graph.VertexCount());
        std::stack<size_t> next;
        next.push(start);

        while (!next.empty()) {
            auto v = next.top();
            next.pop();

            if (visited[v]) continue;
            visited[v] = true;
            indices.push_back(v);

            for (auto n : graph.Neighbors(v))
                next.push(n);
        }

        return {graph, indices};
    }


    template<typename GraphType>
    requires Graph<GraphType>
    VertexRange<GraphType> BFSRange(GraphType &graph, size_t start) {
        std::vector<size_t> indices;

        std::vector<bool> visited(graph.VertexCount());
        std::queue<size_t> next;
        next.push(start);

        while (!next.empty()) {
            auto v = next.front();
            next.pop();

            if (visited[v]) continue;
            visited[v] = true;
            indices.push_back(v);

            for (auto n : graph.Neighbors(v))
                next.push(n);
        }

        return {graph, indices};
    }

    template<typename GraphType>
    requires Graph<GraphType>
    VertexRange<GraphType> BranchRange(GraphType &graph, size_t start) {
        std::vector<size_t> indices;
        indices.push_back(start);
        auto rn = graph.Neighbors(start);

        while (rn.begin() != rn.end()) {
            indices.push_back(*rn.begin());
            rn = graph.Neighbors(*rn.begin());
        }

        return {graph, indices};
    }
}

#endif //TEMPLATE_GRAPHS_ITERATORS_H
