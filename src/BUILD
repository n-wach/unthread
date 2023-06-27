cc_library(
    name = "unthread",
    srcs = [
        "unthread.c",
    ],
    hdrs = [
        "include/bits/pthreadtypes.h",
        "include/pthread.h",
    ],
    copts = [
        "-std=gnu17",
        "-I src/include",
        "-fno-omit-frame-pointer",
        
        # Uncomment to let unthread provide centipede user 
        # features on any context switch (broken!).
        #"-dCENTIPEDE_USER_FEATURES"
    ],
    visibility = [
        "//visibility:public",
    ],
)
