#ifndef TBB_PY_UTILS_HPP
#define TBB_PY_UTILS_HPP

#include <Python.h>

// This RAII struct ensures that the Python interpreter
// can be used from a thread created from outside Python
// (e.g., a pthread/std::thread/etc. created from C/C++).
// On creation, it will register the C/C++ thread with
// the Python interpreter and lock the GIL. On destruction,
// it will release any resource acquired on construction
// and unlock the GIL.
//
// See: https://docs.python.org/2/c-api/init.html
struct gil_thread_ensurer
{
    gil_thread_ensurer()
    {
        m_state = ::PyGILState_Ensure();
    }
    ~gil_thread_ensurer()
    {
        ::PyGILState_Release(m_state);
    }
    ::PyGILState_STATE m_state;
};

// This RAII struct will unlock the GIL on construction,
// and lock it again on destruction.
//
// See: https://docs.python.org/2/c-api/init.html
struct gil_releaser
{
    gil_releaser()
    {
        m_thread_state = ::PyEval_SaveThread();
    }
    ~gil_releaser()
    {
        ::PyEval_RestoreThread(m_thread_state);
    }
    ::PyThreadState *m_thread_state;
};

// Return 1 if the current thread is holding the GIL and 0 otherwise.
inline int GILState_Check()
{
#if PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION >= 4
    return ::PyGILState_Check();
#else
    // Python < 3.4 implementation:
    // http://stackoverflow.com/questions/11366556/how-can-i-check-whether-a-thread-currently-holds-the-gil
    ::PyThreadState *tstate = ::_PyThreadState_Current;
    return tstate && (tstate == ::PyGILState_GetThisThreadState());
#endif
}

#endif
