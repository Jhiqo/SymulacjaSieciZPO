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
        mocks/nodes_mocks.hpp
        mocks/global_functions_mock.hpp
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/reports.cpp
        src/simulation.cpp
        src/storage_types.cpp test/test_package.cpp main.cpp
        test/main_gtest.cpp
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_nodes.cpp
        test/test_package.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        test/test_storage_types.cpp)
set(SOURCES_FILES_TESTS
        test/main_gtest.cpp
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_nodes.cpp
        test/test_package.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        test/test_storage_types.cpp
        )


include(SetCommonConfig.cmake)