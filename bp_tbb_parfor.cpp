#include <Python.h>
#include <iostream>
#include <tbb/parallel_for.h>

#include <boost/python.hpp>

#include "utils.hpp"

namespace py = boost::python;

BOOST_PYTHON_MODULE(bp_tbb_parfor) {
    std::cout << "Thread status: " << ::PyEval_ThreadsInitialized() << '\n';
    ::PyEval_InitThreads();
    std::cout << "Thread status after init: " << ::PyEval_ThreadsInitialized() << '\n';

    py::def("parallel_for",+[](long first, long last, py::object Func) {
        tbb::parallel_for(first, last, [&Func](long n) {
            gil_thread_ensurer gte;
            Func(n);
        });
    });
}
