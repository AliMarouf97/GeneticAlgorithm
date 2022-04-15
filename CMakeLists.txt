cmake_minimum_required(VERSION 3.0)

# set the project name
project(GeneticAlgorithm)

include_directories(${GeneticAlgorithm_SOURCE_DIR}/inc)
add_library(GeneticAlgorithm inc/Individual.h inc/GeneticAlgorithm.h inc/TerminationConditions.h src/TerminationConditions.cpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable

add_executable(traveling_salesman examples/traveling_salesman/traveling_salesman.cpp)
target_link_libraries(traveling_salesman GeneticAlgorithm)

add_executable(multi_peak_function_float examples/multi_peak_function/multi_peak_function_float.cpp)
target_link_libraries(multi_peak_function_float GeneticAlgorithm)

add_executable(multi_peak_function_float2 examples/multi_peak_function/multi_peak_function_float2.cpp)
target_link_libraries(multi_peak_function_float2 GeneticAlgorithm)

add_executable(multi_peak_function_int examples/multi_peak_function/multi_peak_function_int.cpp)
target_link_libraries(multi_peak_function_int GeneticAlgorithm)

add_executable(furniture_manufacturer examples/furniture_manufacturer/furniture_manufacturer.cpp)
target_link_libraries(furniture_manufacturer GeneticAlgorithm)

add_executable(housing_development examples/housing_development/housing_development.cpp)
target_link_libraries(housing_development GeneticAlgorithm)

add_executable(find_password_decoder examples/find_password/find_password_decoder.cpp)
target_link_libraries(find_password_decoder GeneticAlgorithm)

add_executable(find_password examples/find_password/find_password.cpp)
target_link_libraries(find_password GeneticAlgorithm)


# testing binary
add_executable(cpp_test1 test/test.cpp)
target_link_libraries(cpp_test1 PRIVATE GeneticAlgorithm)

add_executable(cpp_test2 test/test_macros.cpp)
target_link_libraries(cpp_test2 PRIVATE GeneticAlgorithm)

# enable testing functionality
enable_testing()

# define tests
add_test(
    NAME cpp_test1
    COMMAND $<TARGET_FILE:cpp_test1>
    )

add_test(
    NAME cpp_test2
    COMMAND $<TARGET_FILE:cpp_test2>
    )