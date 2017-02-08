#include <Python.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <memory>
#include <tbb/task_group.h>

#include <boost/python.hpp>

#include "utils.hpp"

namespace py = boost::python;

BOOST_PYTHON_MODULE(bp_tg) {
    std::cout << "Thread status: " << ::PyEval_ThreadsInitialized() << '\n';
    ::PyEval_InitThreads();
    std::cout << "Thread status after init: " << ::PyEval_ThreadsInitialized() << '\n';

    py::class_<tbb::task_group, boost::shared_ptr<tbb::task_group>, boost::noncopyable> tg_class("task_group", "", py::init<>());
    tg_class.def("run",+[](tbb::task_group &tg, py::object o) {
        auto ptr = std::make_shared<py::object>(o);
         tg.run([ptr](){
             gil_thread_ensurer gte;
             (*ptr)();
         });
    });
    tg_class.def("wait",&tbb::task_group::wait);
}
