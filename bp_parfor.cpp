#include <Python.h>
#include <future>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <boost/python.hpp>

#include "utils.hpp"

namespace py = boost::python;

BOOST_PYTHON_MODULE(bp_parfor) {
    std::cout << "Thread status: " << ::PyEval_ThreadsInitialized() << '\n';
    // This function needs to be called before doing anything with threads.
    ::PyEval_InitThreads();
    std::cout << "Thread status after init: " << ::PyEval_ThreadsInitialized() << '\n';

    py::def("parallel_for", +[](unsigned long long first, unsigned long long last, py::object Func) {
        if (first > last) {
            throw std::invalid_argument("first > last");
        }

        std::cout << "parallel_for() entry, GIL state: " << GILState_Check() << '\n';
        // Let's release the GIL. We need to do this because we will be waiting
        // for all threads to finish before returning from this function. If we didn't
        // release the GIL, the functors executed by the threads will attempt themselves
        // to lock the GIL, leading to a deadlock. If this function returned immediately,
        // rather than waiting for the threads, we would not need to unlock the GIL here
        // (the GIL would be unlocked upon returning from parallel_for()).
        gil_releaser gr;
        std::cout << "After GIL release, GIL state: " << GILState_Check() << '\n';

        std::vector<std::future<void>> vf(last - first);
        auto counter = first;
        for (auto &f: vf) {
            // NOTE: force async launch in a separate thread via std::launch::async.
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
}
