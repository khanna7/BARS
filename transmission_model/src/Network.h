/*
 * Network.h
 *
 *  Created on: Nov 2, 2015
 *      Author: nick
 */

#ifndef SRC_NETWORK_H_
#define SRC_NETWORK_H_

#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>

#include "boost/iterator/transform_iterator.hpp"

#include "Edge.h"

namespace TransModel {

template<typename V>
using EdgePtr = std::shared_ptr<Edge<V>>;

template<typename V>
using VertexPtr = std::shared_ptr<V>;

template<typename T>
struct GetVal {

    T operator()(const std::pair<unsigned int, T>& pair) const {
        return pair.second;
    }
};

template<typename V>
using EdgeMap = std::map<unsigned int, EdgePtr<V>>;

template<typename V>
using EdgeIter = boost::transform_iterator<GetVal<EdgePtr<V>>, typename EdgeMap<V>::iterator>;

template<typename V>
using VertexMap = std::map<unsigned int, VertexPtr<V>>;

template<typename V>
using VertexIter = boost::transform_iterator<GetVal<VertexPtr<V>>, typename VertexMap<V>::iterator>;

struct EdgeListData {
    // edge id
    std::set<unsigned int> edge_idxs;
    std::map<int, unsigned int> edge_counts;

    EdgeListData(unsigned int idx, int edge_type) : edge_idxs{idx}, edge_counts{} {
        edge_counts[edge_type] = 1;
    }
};

using EdgeList = std::map<unsigned int, EdgeListData>;

template<typename V>
class Network {

private:
    unsigned int edge_idx;

    VertexMap<V> vertices;
    // all edges: key -> edge id, value -> Edge
    EdgeMap<V> edges;
    // EdgeList is map: key -> vertex id, value -> EdgeListData
    // oel: outgoing edge list, iel: incoming edge list
    EdgeList oel, iel;
    std::map<int, unsigned int> edge_counts;

    EdgePtr<V> doAddEdge(const std::shared_ptr<V>& source, const std::shared_ptr<V>& target, int type);
    EdgePtr<V> doRemoveEdge(unsigned int v1_idx, unsigned int v2_idx, int type);
    bool doHasEdge(unsigned int v1_idx, unsigned int v2_idx, int type);

    void removeEdges(unsigned int idx, EdgeList& el, std::vector<EdgePtr<V>>& removed_edges);


public:

    Network();
    virtual ~Network();

    void addVertex(const std::shared_ptr<V>& vertex);
    bool removeVertex(const std::shared_ptr<V>& vertex);
    bool removeVertex(unsigned int id);

    VertexIter<V> removeVertex(VertexIter<V> iter);

    EdgePtr<V> addEdge(const std::shared_ptr<V>& source, const std::shared_ptr<V>& target, int type = 0);
    EdgePtr<V> addEdge(unsigned int v1_idx, unsigned int v2_idx, int type = 0);
    EdgePtr<V> removeEdge(unsigned int v1_idx, unsigned int v2_idx, int type = 0);
    bool hasEdge(VertexPtr<V> v1, VertexPtr<V> v2, int type = 0);
    bool hasEdge(unsigned int v1_idx, unsigned int v2_idx, int type = 0);

    EdgeIter<V> edgesBegin();
    EdgeIter<V> edgesEnd();

    VertexIter<V> verticesBegin();
    VertexIter<V> verticesEnd();

    unsigned int vertexCount() const {
        return vertices.size();
    }

    unsigned int edgeCount() const {
        return edges.size();
    }

    unsigned int edgeCount(int edge_type) {
        return edge_counts[edge_type];
    }

    /**
     * Gets whether or not the network contains the specified vertex.
     */
    bool contains(const std::shared_ptr<V>& vertex);

    /**
     * Gets all the edges in which that specified vertex participates and put them
     * the specified vector.
     */
    void getEdges(const VertexPtr<V>& vert, std::vector<EdgePtr<V>>& vec);

    /**
     * Gets the number of edges into the specified vertex.
     */
    unsigned int inEdgeCount(const VertexPtr<V>& vert);

    /**
     * Gets the number of edges out from the specified vertex.
     */
    unsigned int outEdgeCount(const VertexPtr<V>& vert);

    /**
     * Gets the number of edges into the specified vertex.
     */
    unsigned int inEdgeCount(const VertexPtr<V>& vert, int edge_type);

    /**
     * Gets the number of edges out from the specified vertex.
     */
    unsigned int outEdgeCount(const VertexPtr<V>& vert, int edge_type);

    void clearEdges() {
        edges.clear();
        oel.clear();
        iel.clear();
    }
};

template<typename V>
Network<V>::Network() : edge_idx(0), vertices(), edges(), oel(), iel(), edge_counts() {
}

template<typename V>
Network<V>::~Network() {
}

template<typename V>
EdgeIter<V> Network<V>::edgesBegin() {
    GetVal<EdgePtr<V>> gv;
    return EdgeIter<V>(edges.begin(), gv);
}

template<typename V>
EdgeIter<V> Network<V>::edgesEnd() {
    GetVal<EdgePtr<V>> gv;
    return EdgeIter<V>(edges.end(), gv);
}

template<typename V>
VertexIter<V> Network<V>::verticesBegin() {
    GetVal<VertexPtr<V>> gv;
    return VertexIter<V>(vertices.begin(), gv);
}

template<typename V>
VertexIter<V> Network<V>::verticesEnd() {
    GetVal<VertexPtr<V>> gv;
    return VertexIter<V>(vertices.end(), gv);
}

template<typename V>
void Network<V>::getEdges(const VertexPtr<V>& vertex, std::vector<EdgePtr<V>>& vec) {
    auto iiter = iel.find(vertex->id());
    if (iiter != iel.end()) {
        for (auto edge_idx : iiter->second.edge_idxs) {
            vec.push_back(edges[edge_idx]);
        }
    }

    auto oiter = oel.find(vertex->id());
    if (oiter != oel.end()) {
        for (auto edge_idx : oiter->second.edge_idxs) {
            vec.push_back(edges[edge_idx]);
        }
    }
}

template<typename V>
unsigned int Network<V>::inEdgeCount(const VertexPtr<V>& vertex) {
    auto iter = iel.find(vertex->id());
    return iter == iel.end() ? 0 : iter->second.edge_idxs.size();
}

template<typename V>
unsigned int Network<V>::outEdgeCount(const VertexPtr<V>& vertex) {
    auto iter = oel.find(vertex->id());
    return iter == oel.end() ? 0 : iter->second.edge_idxs.size();
}

template<typename V>
unsigned int Network<V>::inEdgeCount(const VertexPtr<V>& vertex, int edge_type) {
    auto iter = iel.find(vertex->id());
    return iter == iel.end() ? 0 : iter->second.edge_counts[edge_type];
}

template<typename V>
unsigned int Network<V>::outEdgeCount(const VertexPtr<V>& vertex, int edge_type) {
    auto iter = oel.find(vertex->id());
    return iter == oel.end() ? 0 : iter->second.edge_counts[edge_type];
}

template<typename V>
void Network<V>::addVertex(const std::shared_ptr<V>& vertex) {
    vertices.emplace(std::make_pair(vertex->id(), vertex));
}

template<typename V>
bool Network<V>::contains(const std::shared_ptr<V>& vertex) {
    return vertices.find(vertex->id()) != vertices.end();
}

template<typename V>
bool Network<V>::hasEdge(VertexPtr<V> v1, VertexPtr<V> v2, int type) {
    return hasEdge(v1->id(), v2->id(), type);
}

template<typename V>
bool Network<V>::hasEdge(unsigned int v1_idx, unsigned int v2_idx, int type) {
    bool has_edge = doHasEdge(v1_idx, v2_idx, type);
    if (!has_edge) {
        has_edge = doHasEdge(v2_idx, v1_idx, type);
    }
    return has_edge;
}

template<typename V>
bool Network<V>::doHasEdge(unsigned int v1_idx, unsigned int v2_idx, int type) {
    auto iter = oel.find(v1_idx);
    if (iter == oel.end()) return false;

    auto type_iter = iter->second.edge_counts.find(type);
    if (type_iter != iter->second.edge_counts.end() && type_iter->second > 0) {
        for (auto edge_idx : iter->second.edge_idxs) {
            auto edge_iter = edges.find(edge_idx);
            if (edge_iter == edges.end())
                throw std::invalid_argument(
                        "Unexpectedly missing edge in hasEdge(v1, v2, type): edge " + std::to_string(edge_idx) + " does not exist.");
            EdgePtr<V> edge = edge_iter->second;
            if (edge->type() == type && edge->v2()->id() == v2_idx) {
                return true;
            }
        }
    }

    return false;
}

template<typename V>
EdgePtr<V> Network<V>::removeEdge(unsigned int v1_idx, unsigned int v2_idx, int type) {
    EdgePtr<V> edge = doRemoveEdge(v1_idx, v2_idx, type);
    if (!edge) {
       edge = doRemoveEdge(v2_idx, v1_idx, type);
    }

    return edge;
}


template<typename V>
EdgePtr<V> Network<V>::doRemoveEdge(unsigned int v1_idx, unsigned int v2_idx, int type) {
    // defaults to nullptr
    EdgePtr<V> edge;
    auto iter = oel.find(v1_idx);
    if (iter == oel.end()) {
        return edge;
    }

    for (auto edge_idx : iter->second.edge_idxs) {
        auto edge_iter = edges.find(edge_idx);
        if (edge_iter == edges.end())
            throw std::invalid_argument(
                    "Unable to delete edge: edge " + std::to_string(edge_idx) + " does not exist.");
        EdgePtr<V> edge = edge_iter->second;
        if (edge->v2()->id() == v2_idx && edge->type() == type) {
            --edge_counts[edge_iter->second->type()];
            edges.erase(edge_iter);
            EdgeListData& data = iter->second;
            data.edge_idxs.erase(edge_idx);
            --data.edge_counts[type];

            EdgeListData& d2 = iel.at(v2_idx);
            d2.edge_idxs.erase(edge_idx);
            --d2.edge_counts[type];
            
            return edge;
        }
    }

    // defaults to nullptr

    return edge;
}

template<typename V>
void Network<V>::removeEdges(unsigned int idx, EdgeList& el, std::vector<EdgePtr<V>>& removed_edges) {
    auto iter = el.find(idx);
    if (iter != el.end()) {
        // set of edges
        for (auto edge_idx_iter = iter->second.edge_idxs.begin(); edge_idx_iter != iter->second.edge_idxs.end();) {
            // remove the entry from mel
            auto mel_iter = edges.find(*edge_idx_iter);
            if (mel_iter == edges.end())
                throw std::invalid_argument(
                        "Unable to delete edge: edge " + std::to_string(*edge_idx_iter) + " does not exist.");
            removed_edges.push_back(mel_iter->second);
            --edge_counts[mel_iter->second->type()];
            --(iter->second.edge_counts[mel_iter->second->type()]);
            // erase that edge
            edges.erase(mel_iter);
            // remove the mel edge_id from the edge list set
            edge_idx_iter = iter->second.edge_idxs.erase(edge_idx_iter);
        }
    }
}

template<typename V>
bool Network<V>::removeVertex(const std::shared_ptr<V>& vertex) {
    unsigned int id = vertex->id();
    return removeVertex(id);
}

template<typename V>
bool Network<V>::removeVertex(unsigned int id) {
    auto iter = vertices.find(id);
    if (iter != vertices.end()) {
        vertices.erase(iter);
        std::vector<EdgePtr<V>> removed_edges;
        removeEdges(id, oel, removed_edges);
        // for each edge get v2 id, remove edge id from oel for v2 id
        for (auto& edge : removed_edges) {
            EdgeListData& data = iel.at(edge->v2()->id());
            data.edge_idxs.erase(edge->id());
            --data.edge_counts[edge->type()];
        }

        removed_edges.clear();
        removeEdges(id, iel, removed_edges);
        // for each edge get v1 id, remove edge id from iel for v1 id
        for (auto& edge : removed_edges) {
            EdgeListData& data = oel.at(edge->v1()->id());
            data.edge_idxs.erase(edge->id());
            --data.edge_counts[edge->type()];
        }
        return true;
    }

    return false;
}

template<typename V>
VertexIter<V> Network<V>::removeVertex(VertexIter<V> iter) {
    VertexPtr<V> v = (*iter);
    unsigned int id = v->id();
    auto next_iter = ++iter;
    removeVertex(id);
    return next_iter;
}

template<typename V>
EdgePtr<V> Network<V>::doAddEdge(const std::shared_ptr<V>& source, const std::shared_ptr<V>& target, int type) {
    // assumes sanity checks have already occured
    EdgePtr<V> edge = std::make_shared<Edge<V>>(edge_idx, source, target, type);
    edges.emplace(edge_idx, edge);

    auto out_iter = Network<V>::oel.find(source->id());
    if (out_iter == Network<V>::oel.end()) {
        Network<V>::oel.emplace(source->id(), EdgeListData(edge_idx, type));
    } else {
        EdgeListData& data = out_iter->second;
        data.edge_idxs.emplace(edge_idx);
        ++data.edge_counts[type];
    }

    auto in_iter = Network<V>::iel.find(target->id());
    if (in_iter == Network<V>::iel.end()) {
        Network<V>::iel.emplace(target->id(),  EdgeListData(edge_idx, type));
    } else {
        EdgeListData& data = in_iter->second;
        data.edge_idxs.emplace(edge_idx);
        ++data.edge_counts[type];
    }
    ++edge_idx;
    ++edge_counts[type];
    return edge;
}

template<typename V>
EdgePtr<V> Network<V>::addEdge(unsigned int v1_idx, unsigned int v2_idx, int type) {
    auto v1 = vertices.find(v1_idx);
    if (v1 == vertices.end())
        throw std::invalid_argument(
                "Unable to create edge: vertex " + std::to_string(v1_idx) + " not found in vertex map");
    auto v2 = vertices.find(v2_idx);
    if (v2 == vertices.end())
        throw std::invalid_argument(
                "Unable to create edge: vertex " + std::to_string(v2_idx) + " not found in vertex map");

    return doAddEdge(v1->second, v2->second, type);
}

template<typename V>
EdgePtr<V> Network<V>::addEdge(const std::shared_ptr<V>& source, const std::shared_ptr<V>& target, int type) {
    if (vertices.find(source->id()) == vertices.end()) {
        addVertex(source);
    }

    if (vertices.find(target->id()) == vertices.end()) {
        addVertex(target);
    }

    return doAddEdge(source, target, type);
}

}

/* namespace TransModel */

#endif /* SRC_NETWORK_H_ */
