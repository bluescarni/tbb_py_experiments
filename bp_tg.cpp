#include <Python.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <memory>
#include <tbb/task_group.h>

#include <boost/python.hpp>

#include "utils.hpp"

namespace py = boost::python;

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

BOOST_PYTHON_MODULE(bp_tg) {
    std::cout << "Thread status: " << ::PyEval_ThreadsInitialized() << '\n';
    ::PyEval_InitThreads();
    std::cout << "Thread status after init: " << ::PyEval_ThreadsInitialized() << '\n';

    py::class_<tbb::task_group, boost::shared_ptr<tbb::task_group>, boost::noncopyable> tg_class("task_group", "", py::init<>());
    tg_class.def("run",+[](tbb::task_group &tg, py::object o) {
        runner r{std::make_shared<py::object>(o)};
         tg.run([r](){
             gil_thread_ensurer gte;
             deleter d{r};
             (*r.m_ptr)();
         });
    });
    tg_class.def("wait",+[](tbb::task_group &tg) {
        gil_releaser gr;
        tg.wait();
    });
}
