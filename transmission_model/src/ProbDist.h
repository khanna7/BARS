/*
 * ProbDist.h
 *
 *  Created on: Feb 2, 2017
 *      Author: nick
 */

#ifndef SRC_PROBDIST_H_
#define SRC_PROBDIST_H_

#include <vector>
#include <utility>

namespace TransModel {

const double EPSILON = .00001;

template<typename T>
class ProbDistCreator;

template<typename T>
class ProbDist {
private:
	friend class ProbDistCreator<T>;

	std::vector<std::pair<double, T>> bins;
	ProbDist(std::vector<std::pair<double, T>> items);

public:
	T draw(double val);
};

template<typename T>
class ProbDistCreator {
private:
	std::vector<std::pair<double, T>> bins;

public:
	ProbDistCreator() {}
	~ProbDistCreator() {}

	void addItem(double probability, T item);
	ProbDist<T> createProbDist();

	void clear() {
		bins.clear();
	}
};


template<typename T>
void ProbDistCreator<T>::addItem(double probability, T item) {
	if (probability > 0) {
		bins.push_back(std::make_pair(probability, item));
	}
}

template<typename T>
ProbDist<T> ProbDistCreator<T>::createProbDist() {
	double sum = 0;
	for (auto& bin : bins) {
		sum += bin.first;
		bin.first = sum;
	}

	if (sum < 1.0 - EPSILON || sum > 1.0 + EPSILON) {
		throw std::domain_error("Invalid value used to initialize ProbDist. Sum of probabilities must equal 1.");
	}
	bins[bins.size() - 1].first = 1.0;

	return ProbDist<T>(bins);
}

template<typename T>
ProbDist<T>::ProbDist(std::vector<std::pair<double, T>> items) : bins{items} {
}

template<typename T>
T ProbDist<T>::draw(double val) {
	for (auto& bin : bins) {
		if (val <= bin.first) {
			return bin.second;
		}
	}
	return bins[bins.size() - 1].second;
}


}




#endif /* SRC_PROBDIST_H_ */
