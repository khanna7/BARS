#ifndef SRC_NETWORKSTATS_H_
#define SRC_NETWORKSTATS_H_

#include <map>
#include <utility>
#include <algorithm>

#include "Network.h"
#include "Person.h"

#include "igraph/igraph.h"

namespace TransModel {

template <typename T, typename U>
struct Comp {

    std::map<unsigned int, U> scores;
    Comp();
    bool operator()(const std::shared_ptr<T>& p1, const std::shared_ptr<T>& p2); 
};

template <typename T, typename U>
Comp<T, U>::Comp() : scores() {}

template <typename T, typename U>
bool Comp<T, U>::operator()(const std::shared_ptr<T>& p1, const std::shared_ptr<T>& p2) {
    return scores[p1->id()] > scores[p2->id()];
}

template <typename T>
class NetworkStats {

  private:
    igraph_t graph;
    std::map<long, std::shared_ptr<T>> g_to_p;

    void degree(std::vector<std::shared_ptr<T>> &results, std::map<unsigned int, long>& scores);
    void eigenCentrality(std::vector<std::shared_ptr<T>> &results, std::map<unsigned int, double>& scores);

  public:
    NetworkStats(Network<T> &network);
    ~NetworkStats();

    void degree(std::vector<std::shared_ptr<T>>& result);
    void degree(std::vector<std::pair<std::shared_ptr<T>, long>>& result);
    void degree(std::vector<std::pair<std::shared_ptr<T>, long>>& result, size_t top_n);

    void eigenCentrality(std::vector<std::shared_ptr<T>>& result);
    void eigenCentrality(std::vector<std::pair<std::shared_ptr<T>, double>>& result);
    void eigenCentrality(std::vector<std::pair<std::shared_ptr<T>, double>>& result, size_t top_n);
};

template <typename T>
NetworkStats<T>::NetworkStats(Network<T> &network) : graph(), g_to_p() {
    std::map<int, long> n_to_g;
    long i = 0;
    for (auto iter = network.verticesBegin(); iter != network.verticesEnd();
         ++iter) {
        n_to_g[(*iter)->id()] = i;
        g_to_p[i] = (*iter);
        ++i;
    }

    igraph_vector_t edges;
    igraph_vector_init(&edges, network.edgeCount() * 2);

    i = 0;
    for (auto iter = network.edgesBegin(); iter != network.edgesEnd(); ++iter) {
        EdgePtr<T> edge = (*iter);
        VECTOR(edges)[i] = n_to_g.at(edge->v1()->id());
        ++i;
        VECTOR(edges)[i] = n_to_g.at(edge->v2()->id());
        ++i;
    }
    igraph_create(&graph, &edges, network.vertexCount(), 0);
    igraph_vector_destroy(&edges);
}

template <typename T>
void NetworkStats<T>::degree(std::vector<std::pair<std::shared_ptr<T>, long>> &results) {
    igraph_vector_t degs;
    igraph_vector_init(&degs, igraph_vcount(&graph));
    igraph_degree(&graph, &degs, igraph_vss_all(), IGRAPH_ALL, 0);

    for (long i = 0, n = igraph_vector_size(&degs); i < n;  ++i) {
        results.push_back({g_to_p[i], (long int) VECTOR(degs)[i]});
    }
    igraph_vector_destroy(&degs);
}

template<typename T, typename U>
bool rcomp(const std::pair<std::shared_ptr<T>, U>& p1, const std::pair<std::shared_ptr<T>, U>& p2) {
    return p1.second > p2.second;
}

template <typename T>
void NetworkStats<T>::degree(std::vector<std::shared_ptr<T>> &results, std::map<unsigned int, long>& scores) {
    igraph_vector_t degs;
    igraph_vector_init(&degs, igraph_vcount(&graph));
    igraph_degree(&graph, &degs, igraph_vss_all(), IGRAPH_ALL, 0);

    for (long i = 0, n = igraph_vector_size(&degs); i < n;  ++i) {
        std::shared_ptr<T>& p = g_to_p[i];
        results.push_back(p);
        scores.emplace(p->id(), (long) VECTOR(degs)[i]);
    }
    igraph_vector_destroy(&degs);
}

template <typename T>
void NetworkStats<T>::degree(std::vector<std::shared_ptr<T>>& results) {
    Comp<T, long> comp;
    degree(results, comp.scores);
    // shuffle in case of ties
    std::random_shuffle(results.begin(), results.end(), &repast::uni_random);
    std::sort(results.begin(), results.end(), comp);
}

template <typename T>
void NetworkStats<T>::eigenCentrality(std::vector<std::shared_ptr<T>> &results, std::map<unsigned int, double>& scores) {
    igraph_vector_t eigens;
    igraph_vector_init(&eigens, igraph_vcount(&graph));

    igraph_arpack_options_t options;
    igraph_arpack_options_init(&options);
    igraph_real_t value;

    // directed, scale, weights,
    igraph_eigenvector_centrality(&graph, &eigens, &value, 0, 0, 0, &options);

    for (long i = 0, n = igraph_vector_size(&eigens); i < n;  ++i) {
        std::shared_ptr<T>& p = g_to_p[i];
        results.push_back(p);
        scores.emplace(p->id(), (double) VECTOR(eigens)[i]);
    }
    igraph_vector_destroy(&eigens);
}

template <typename T>
void NetworkStats<T>::eigenCentrality(std::vector<std::shared_ptr<T>>& results) {
    Comp<T, double> comp;
    eigenCentrality(results, comp.scores);
    // shuffle in case of ties
    std::random_shuffle(results.begin(), results.end(), &repast::uni_random);
    std::sort(results.begin(), results.end(), comp);
}

template <typename T>
void NetworkStats<T>::degree(std::vector<std::pair<std::shared_ptr<T>, long>>& results, size_t top_n) {
    if (top_n >= g_to_p.size()) {
        degree(results);
        std::sort(results.begin(), results.end(), rcomp<T, long>);
    } else {
        std::vector<std::pair<std::shared_ptr<T>, long>> v;
        degree(v);
        // shuffle in case of ties
        std::random_shuffle(v.begin(), v.end(), &repast::uni_random);
        std::sort(v.begin(), v.end(), rcomp<T, long>);
        results.resize(top_n);
        std::copy_n(v.begin(), top_n, results.begin());
    }
}



template <typename T>
void NetworkStats<T>::eigenCentrality(std::vector<std::pair<std::shared_ptr<T>, double>>& results) {
    igraph_vector_t eigens;
    igraph_vector_init(&eigens, igraph_vcount(&graph));

    igraph_arpack_options_t options;
    igraph_arpack_options_init(&options);
    igraph_real_t value;

    // directed, scale, weights,
    igraph_eigenvector_centrality(&graph, &eigens, &value, 0, 0, 0, &options);

    for (long i = 0, n = igraph_vector_size(&eigens); i < n;  ++i) {
        results.push_back({g_to_p[i], (double) VECTOR(eigens)[i]});
    }
    igraph_vector_destroy(&eigens);
}

template <typename T>
void NetworkStats<T>::eigenCentrality(std::vector<std::pair<std::shared_ptr<T>, double>>& results, size_t top_n) {
    if (top_n >= g_to_p.size()) {
        eigenCentrality(results);
        std::sort(results.begin(), results.end(), rcomp<T, double>);
    } else {
        std::vector<std::pair<std::shared_ptr<T>, double>> v;
        eigenCentrality(v);
        // shuffle in case of ties
        std::random_shuffle(v.begin(), v.end(), &repast::uni_random);
        std::sort(v.begin(), v.end(), rcomp<T, double>);
        results.resize(top_n);
        std::copy_n(v.begin(), top_n, results.begin());
    }
}

template <typename T> 
NetworkStats<T>::~NetworkStats() {
    igraph_destroy(&graph);
}

} // namespace TransModel

#endif