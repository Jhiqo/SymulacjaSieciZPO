//
// Created by julia on 30.11.2023.
//

#ifndef SYMULACJASIECIZPO_SIMULATION_HPP
#define SYMULACJASIECIZPO_SIMULATION_HPP

#include <functional>
#include "factory.hpp"
#include "types.hpp"

class IntervalReportNotifier {
public:
	IntervalReportNotifier(TimeOffset to) = default;
	bool should_generate_report(Time t);
private:
	TimeOffset to_;
};

class SpecificTurnsReportNotifier {
public:
	SpecificTurnsReportNotifier(std::set<Time> turns) = default;
	bool should_generate_report(Time t);
private:
	std::set<Time> turns_;
};

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> r);

#endif //SYMULACJASIECIZPO_SIMULATION_HPP
