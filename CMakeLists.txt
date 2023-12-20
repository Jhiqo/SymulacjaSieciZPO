cmake_minimum_required(VERSION 3.24)
project(SymulacjaSieciZPO)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(SOURCE_FILES
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        #src/reports.cpp
        #src/simulation.cpp
        src/storage_types.cpp
        )

add_executable(
        main
        ${SOURCE_FILES} main.cpp
)



# == Unit testing using Google Testing Framework ==

set(SOURCES_FILES_TESTS
        #            test/test_Factory.cpp
        #            test/test_factory_io.cpp
                    test/test_nodes.cpp
                    test/test_package.cpp
        #            test/test_reports.cpp
        #            test/test_simulate.cpp
                    test/test_storage_types.cpp
        )

set(EXEC_TEST ${PROJECT_ID}__test)
add_executable(${EXEC_TEST} ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

set(GTEST_ROOT googletest-master)
target_include_directories(${EXEC_TEST} PUBLIC
        ${GTEST_ROOT}/googlemock/include
        ${GTEST_ROOT}/googletest/include
        )

add_subdirectory(${GTEST_ROOT} googletest-main)

target_link_libraries(${EXEC_TEST} gmock)