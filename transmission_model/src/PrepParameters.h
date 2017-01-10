/*
 * PrepParameters.h
 *
 *  Created on: Jan 10, 2017
 *      Author: nick
 */

#ifndef SRC_PREPPARAMETERS_H_
#define SRC_PREPPARAMETERS_H_

namespace TransModel {

enum class PrepStatus { OFF, OFF_INFECTED, ON};

class PrepParameters {

private:
	PrepStatus status_;
	double start_time_, stop_time_;
public:
	PrepParameters(PrepStatus status, double start_time, double stop_time);
	virtual ~PrepParameters();

	const PrepStatus status() const {
		return status_;
	}

	double stopTime() const {
		return stop_time_;
	}

	double startTime() const {
		return start_time_;
	}

	void on(double start_time, double stop_time);

	void off() {
		status_ = PrepStatus::OFF;
		start_time_ = -1;
		stop_time_ = -1;
	}

	void offInfected() {
		status_ = PrepStatus::OFF_INFECTED;
	}
};

} /* namespace TransModel */

#endif /* SRC_PREPPARAMETERS_H_ */
