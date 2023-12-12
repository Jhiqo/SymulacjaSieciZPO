//
// Created by julia on 30.11.2023.
//

#ifndef SYMULACJASIECIZPO_TYPES_HPP
#define SYMULACJASIECIZPO_TYPES_HPP

#include <functional>

//definicja aliasu ElementID - do 8 grudnia (done)
//definicja aliasów Time, TimeOffset i ProbabilityGenerator - do 15 grudnia (done)

using ElementID = unsigned int;
using Time = int;
using TimeOffset = int;
using ProbabilityGenerator = std::function<double()>;

#endif //SYMULACJASIECIZPO_TYPES_HPP
