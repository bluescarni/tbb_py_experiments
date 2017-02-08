def func(n):
    import time, random
    time.sleep(random.uniform(0,1))
    print("Processing item {}".format(n))

def func2():
    import time, random
    time.sleep(random.uniform(0,1))
    print("Done!")

import pb_tg

tg = pb_tg.task_group()
for i in range(100):
    tg.run(func2)

print("Waiting now!!!")

tg.wait()

import bp_tg

tg = bp_tg.task_group()
for i in range(100):
    tg.run(func2)

print("Waiting now!!!")

tg.wait()

import bp_parfor

bp_parfor.parallel_for(0,100,func)

import pb_parfor

pb_parfor.parallel_for(0,100,func)
