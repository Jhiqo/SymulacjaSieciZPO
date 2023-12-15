//
// Created by julia on 30.11.2023.
//

#ifndef SYMULACJASIECIZPO_HELPERS_HPP
#define SYMULACJASIECIZPO_HELPERS_HPP
#include <functional>
#include <random>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;
//ewentualne pomocnicze obiekty i funkcje globalne (np. generator liczb losowych)

#endif //SYMULACJASIECIZPO_HELPERS_HPP
