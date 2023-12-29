//
// Created by julia on 30.11.2023.
//

#include "simulation.hpp"
#include "reports.hpp"
#include <stdexcept>

SpecificTurnsReportNotifier::should_generate_report(Time t) {
	return turns_.find(t) != turns_.end();
}

IntervalReportNotifier::should_generate_report(Time t) {
	return (t - 1) % to_ == 0;
}

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> r) {
	if (!f.isconsistent()) {
		throw std::logic_error("Inconsistent graph");
	}

	Time t = 1;

	while (t <= d) {

		f.do_delivery(t);
		f.do_package_passing();
		f.do_work(t);
		r(f, t);

		t++;
	}
}