#include <Python.h>
#include <future>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <pybind11/pybind11.h>

#include "utils.hpp"

namespace py = pybind11;

PYBIND11_PLUGIN(pb_parfor) {
    std::cout << "Thread status: " << ::PyEval_ThreadsInitialized() << '\n';
    // This function needs to be called before doing anything with threads.
    ::PyEval_InitThreads();
    std::cout << "Thread status after init: " << ::PyEval_ThreadsInitialized() << '\n';

    py::module m("pb_parffor", "");

    m.def("parallel_for", [](unsigned long long first, unsigned long long last, py::object Func) {
        if (first > last) {
            throw std::invalid_argument("first > last");
        }

        std::cout << "parallel_for() entry, GIL state: " << GILState_Check() << '\n';
        // Let's release the GIL.
        gil_releaser gr;
        std::cout << "After GIL release, GIL state: " << GILState_Check() << '\n';

        std::vector<std::future<void>> vf(last - first);
        auto counter = first;
        for (auto &f: vf) {
            f = std::async(std::launch::async, [&Func,counter]() {
                gil_thread_ensurer gte;
                Func(counter);
            });
            ++counter;
        }
        for (auto &f: vf) {
            f.wait();
        }
    });

    return m.ptr();
}
