cmake_minimum_required(VERSION 3.24)
project(SymulacjaSieciZPO)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(SymulacjaSieciZPO
        include/factory.hpp
        include/helpers.hpp
        include/nodes.hpp
        include/package.hpp
        include/reports.hpp
        include/simulation.hpp
        include/storage_types.hpp
        include/types.hpp
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/reports.cpp
        src/simulation.cpp
        src/storage_types.cpp)
