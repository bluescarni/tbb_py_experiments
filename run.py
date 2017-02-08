def func(n):
    import time, random
    time.sleep(random.uniform(0,1))
    print("Processing item {}".format(n))

import bp_parfor

bp_parfor.parallel_for(0,100,func)

import pb_parfor

pb_parfor.parallel_for(0,100,func)
