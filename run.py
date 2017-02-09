from time import sleep

def func(n):
    import random
    sleep(random.uniform(0,1))
    print("Processing item {}".format(n))

def func2():
    import time, random
    sleep(random.uniform(0,1))
    print("Done!")

print("pybind11, tbb::task_group test")
print("==============================")
sleep(1)
import pb_tg

tg = pb_tg.task_group()
for i in range(100):
    tg.run(func2)

print("Waiting now!!!")

tg.wait()

print("boost.python, tbb::task_group test")
print("==================================")
sleep(1)
import bp_tg

tg = bp_tg.task_group()
for i in range(100):
    tg.run(func2)

print("Waiting now!!!")

tg.wait()

print("pybind11, c++11 parallel_for test")
print("=================================")
sleep(1)
import pb_parfor

pb_parfor.parallel_for(0,100,func)

print("boost.python, c++11 parallel_for test")
print("=====================================")
sleep(1)
import bp_parfor

bp_parfor.parallel_for(0,100,func)

print("pybind11, TBB parallel_for test")
print("===============================")
sleep(1)
import pb_tbb_parfor

pb_parfor.parallel_for(0,100,func)

print("boost.python, TBB parallel_for test")
print("===================================")
sleep(1)
import bp_tbb_parfor

bp_parfor.parallel_for(0,100,func)
