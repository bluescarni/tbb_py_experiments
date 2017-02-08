#include <Python.h>
#include <iostream>
#include <memory>
#include <tbb/task_group.h>

#include <pybind11/pybind11.h>

#include "utils.hpp"

namespace py = pybind11;

struct runner
{
	mutable std::shared_ptr<py::object> m_ptr;
};

struct deleter
{
    deleter(const runner &r):m_runner(r) {}
    ~deleter()
    {
        m_runner.m_ptr.reset();
    }
    runner const &m_runner;
};

PYBIND11_PLUGIN(pb_tg) {
    std::cout << "Thread status: " << ::PyEval_ThreadsInitialized() << '\n';
    ::PyEval_InitThreads();
    std::cout << "Thread status after init: " << ::PyEval_ThreadsInitialized() << '\n';

    py::module m("pb_tg", "");

    py::class_<tbb::task_group> tg_class(m, "task_group");
    tg_class.def(py::init<>());
    tg_class.def("run",[](tbb::task_group &tg, py::object o) {
         runner r{std::make_shared<py::object>(o)};
         tg.run([r]() {
             gil_thread_ensurer gte;
             deleter d{r};
             (*r.m_ptr)();
         });
    });
    tg_class.def("wait",[](tbb::task_group &tg) {
        gil_releaser gr;
        tg.wait();
    });

    return m.ptr();
}
