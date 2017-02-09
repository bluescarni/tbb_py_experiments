#include <Python.h>
#include <iostream>
#include <tbb/parallel_for.h>

#include <pybind11/pybind11.h>

#include "utils.hpp"

namespace py = pybind11;

PYBIND11_PLUGIN(pb_tbb_parfor) {
    std::cout << "Thread status: " << ::PyEval_ThreadsInitialized() << '\n';
    // This function needs to be called before doing anything with threads.
    ::PyEval_InitThreads();
    std::cout << "Thread status after init: " << ::PyEval_ThreadsInitialized() << '\n';

    py::module m("pb_tbb_parfor", "");

    m.def("parallel_for",[](long first, long last, py::object Func) {
        tbb::parallel_for(first, last, [&Func](long n) {
            gil_thread_ensurer gte;
            Func(n);
        });
    });

    return m.ptr();
}
