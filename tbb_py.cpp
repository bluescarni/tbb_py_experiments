#include <memory>
#include <chrono>
#include <iostream>
#include <future>
#include <vector>

#include <boost/python.hpp>
//#include <pybind11/pybind11.h>

//#include <tbb/parallel_for.h>

//namespace py = pybind11;
namespace py = boost::python;

// class scoped_gil_release
// {
//     public:
//         scoped_gil_release()
//         {
//             m_thread_state = PyEval_SaveThread();
//         }
//         ~scoped_gil_release()
//         {
//             PyEval_RestoreThread(m_thread_state);
//             m_thread_state = NULL;
//         }
//     private:
//         PyThreadState *m_thread_state;
// };

struct gil_ensurer
{
    gil_ensurer()
    {
        m_state = ::PyGILState_Ensure();
    }
    ~gil_ensurer()
    {
        ::PyGILState_Release(m_state);
    }
    ::PyGILState_STATE m_state;
};

BOOST_PYTHON_MODULE(tbb_py) {
    ::PyEval_InitThreads();
    //py::module m("tbb_py", "TBB Py playground.");

    py::def("parallel_for", +[](unsigned long long first, unsigned long long last, py::object Func) {
        std::cout << "MAIN PyGILState_Check(): " << PyGILState_Check() << '\n';
        PyThreadState *thread_state = PyEval_SaveThread();
        std::cout << "MAIN PyGILState_Check(): " << PyGILState_Check() << '\n';
        std::vector<std::future<void>> vf(last - first);
        auto counter = first;
        for (auto &f: vf) {
            f = std::async(std::launch::async, [&Func,counter]() {
                std::cout << "PyGILState_Check(): " << PyGILState_Check() << '\n';
// std::cout << "PyGILState_GetThisThreadState(): " << PyGILState_GetThisThreadState() << '\n';
//                 scoped_gil_release sgr;
                gil_ensurer ge;
                Func(counter);
            });
            ++counter;
        }
        for (auto &f: vf) {
            f.wait();
        }
        PyEval_RestoreThread(thread_state);
    });

    // py::class_<tbb::task_group> tg_class(m, "task_group");
    // tg_class.def(py::init<>());
    // tg_class.def("run",[](tbb::task_group &tg, const py::object &o) {
    //     tg.run([o](){
    //         gil_ensurer ge;
    //         o();
    //     });
    //     //tg.wait();
    // });

    //return m.ptr();
}
