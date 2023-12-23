//
// Created by julia on 30.11.2023.
//

#ifndef SYMULACJASIECIZPO_TYPES_HPP
#define SYMULACJASIECIZPO_TYPES_HPP

#include <functional>

//definicja aliasu ElementID - do 8 grudnia (done)
//definicja aliasów Time, TimeOffset i ProbabilityGenerator - do 15 grudnia (done)

using ElementID = int;
using Time = unsigned int;
using TimeOffset = unsigned int;
using ProbabilityGenerator = std::function<double()>;

#endif //SYMULACJASIECIZPO_TYPES_HPP
