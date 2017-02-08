cmake_minimum_required(VERSION 3.2.0)

project(tbb_py VERSION 0.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#find_package(TBB REQUIRED)
find_package(Boost REQUIRED COMPONENTS python)
find_package(PythonLibs REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG YES)
find_package(Threads REQUIRED)
unset(THREADS_PREFER_PTHREAD_FLAG)

#include(pybind11Tools)

#pybind11_add_module(tbb_py tbb_py.cpp)
add_library(tbb_py SHARED tbb_py.cpp)
set_target_properties(tbb_py PROPERTIES PREFIX "")
target_include_directories(tbb_py PRIVATE "${PYTHON_INCLUDE_DIRS}")
target_link_libraries(tbb_py ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARIES} Threads::Threads)
