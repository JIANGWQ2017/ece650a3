cmake_minimum_required(VERSION 2.8)
project(a3-ece650)

## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)
add_definitions(-std=c++11)
# create the main executable
## add additional .cpp files if needed
add_executable(a2-ece650 a2-ece650.cpp )
add_executable(a3-ece650 a3-ece650.cpp )
add_executable(rgen rgen.cpp)
# create the executable for tests
install(PROGRAMS a1-ece650.py input_parser.py graph_generator.py line_intersect_discriminator.py DESTINATION bin)
install(TARGETS rgen a2-ece650 a3-ece650 RUNTIME DESTINATION bin)

