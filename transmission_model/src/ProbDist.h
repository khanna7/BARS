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

	using PtrT = std::shared_ptr<T>;

	std::vector<std::pair<double, PtrT>> bins;
	ProbDist(std::vector<std::pair<double, PtrT>> items);

public:

	~ProbDist();
	PtrT draw(double val);
};

template<typename T>
class ProbDistCreator {
private:
	using PtrT = std::shared_ptr<T>;
	std::vector<std::pair<double, PtrT>> bins;

public:
	ProbDistCreator() {}
	~ProbDistCreator() {}

	void addItem(double probability, PtrT item);
	ProbDist<T> createProbDist();

	void clear() {
		bins.clear();
	}
};


template<typename T>
void ProbDistCreator<T>::addItem(double probability, PtrT item) {
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
		std::cout << sum << std::endl;
		throw std::domain_error("Invalid value used to initialize ProbDist. Sum of probabilities must equal 1.");
	}
	bins[bins.size() - 1].first = 1.0;

	return ProbDist<T>(bins);
}

template<typename T>
ProbDist<T>::ProbDist(std::vector<std::pair<double, PtrT>> items) : bins{items} {}

template<typename T>
ProbDist<T>::~ProbDist() {}

template<typename T>
std::shared_ptr<T> ProbDist<T>::draw(double val) {
	for (auto& bin : bins) {
		if (bin.first <= val) {
			return bin.second;
		}
	}
	return bins[bins.size() - 1].second;
}


}




#endif /* SRC_PROBDIST_H_ */
