//
// Created by Conor Pappas on 1/12/24.
//

#pragma once

#include <map>
#include <queue>

#include "graph/graph_traits.hpp"
#include "range_concepts.hpp"

namespace support::graph::algorithms {


    struct unimplemented{};
    using distance_t = size_t;

    template<typename Graph>
    using vertex_descriptor = typename graph_traits<Graph>::vertex_descriptor;

    // TODO: This should be a concept for a map instead of std::map concretely
    template<typename Graph>
    using DistanceMap = std::map<vertex_descriptor<Graph>, distance_t>;

    // TODO: This should be a concept for a map instead of std::map concretely
    template<typename Graph>
    using ShortestPathsMap = std::map<vertex_descriptor<Graph>, vertex_descriptor<Graph>>;

    enum class VertexStatus {
        Undiscovered,
        Queued,
        Finsihed
    };

    template<typename Graph>
    using VertexPriorityQueue = std::priority_queue<vertex_descriptor<Graph>, std::vector<vertex_descriptor<Graph>>, std::greater<distance_t>>;

    template <concepts::Graph Graph, range_of<typename graph_traits<Graph>::vertex_descriptor> VertexRange>
    void dijkstra_shortest_paths(
        const Graph& graph,
        VertexRange start_nodes,
        std::function<distance_t(typename graph_traits<Graph>::arc_descriptor)> edge_weight,
        DistanceMap<Graph>& distances,
        ShortestPathsMap<Graph>& shortest_paths)
    {
        const auto order = num_vertices(graph);
        std::vector<VertexStatus> node_status(order, VertexStatus::Undiscovered);
        const auto compare = [&distances](const auto& lhs, const auto& rhs) {
            return distances[lhs] > distances[rhs];
        };
        using vertex_descriptor = typename graph_traits<Graph>::vertex_descriptor;
        auto vertex_queue = std::priority_queue<vertex_descriptor, std::vector<vertex_descriptor>, decltype(compare)>(compare);
        for(const auto& start_node : start_nodes) {
            vertex_queue.push(start_node);
            node_status[start_node] = VertexStatus::Queued;
            distances[start_node] = 0;
            shortest_paths[start_node] = start_node;
        }

        while(!vertex_queue.empty()) {
            const auto current = vertex_queue.top();
            vertex_queue.pop();
            for(const auto& arc : out_arcs(graph, current)) {
                const auto neighbor = target(graph, arc);
                if(node_status[neighbor] == VertexStatus::Finsihed) continue;
                const auto new_distance = distances[current] + edge_weight(arc);
                if(!distances.contains(neighbor) || (new_distance < distances[neighbor])) {
                    distances[neighbor] = new_distance;
                    shortest_paths[neighbor] = current;
                }
                if(node_status[neighbor] == VertexStatus::Undiscovered) {
                    vertex_queue.push(neighbor);
                    node_status[neighbor] = VertexStatus::Queued;
                }
            }
            node_status[current] = VertexStatus::Finsihed;
        }
    }

    template <concepts::Graph Graph>
    void dijkstra_shortest_paths(
        const Graph& graph,
        typename graph_traits<Graph>::vertex_descriptor start_node,
        std::function<distance_t(typename graph_traits<Graph>::arc_descriptor)> edge_weight,
        DistanceMap<Graph>& distances,
        ShortestPathsMap<Graph>& shortest_paths)
    {
        auto start_nodes = { start_node };
        dijkstra_shortest_paths(graph, start_nodes, edge_weight, distances, shortest_paths);
    }

    template <concepts::Graph Graph, range_of<typename graph_traits<Graph>::vertex_descriptor> VertexRange>
    std::tuple<DistanceMap<Graph>, ShortestPathsMap<Graph>> dijkstra_shortest_paths(
        const Graph& graph,
        VertexRange start_nodes,
        std::function<distance_t(typename graph_traits<Graph>::arc_descriptor)> edge_weight)
    {
        DistanceMap<Graph> distances;
        ShortestPathsMap<Graph> shortest_paths;
        dijkstra_shortest_paths(graph, start_nodes, edge_weight, distances, shortest_paths);
        return {distances, shortest_paths};
    }

    template <concepts::Graph Graph>
    std::tuple<DistanceMap<Graph>, ShortestPathsMap<Graph>> dijkstra_shortest_paths(
        const Graph& graph,
        typename graph_traits<Graph>::vertex_descriptor start,
        std::function<distance_t(typename graph_traits<Graph>::arc_descriptor)> edge_weight)
    {
        DistanceMap<Graph> distances;
        ShortestPathsMap<Graph> shortest_paths;
        dijkstra_shortest_paths(graph, start, edge_weight, distances, shortest_paths);
        return {distances, shortest_paths};
    }
}
