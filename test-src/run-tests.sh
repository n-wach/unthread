runTest () {
    python test.py  --bin ../bin/$1 --src $1.c --seeds $1.seeds
}

runTest cancel
runTest cond
runTest cond_timedwait
runTest mutex
runTest mutex_timedlock
runTest yield
runTest yield_single
