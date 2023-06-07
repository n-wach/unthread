def make_fuzz_target_library(name, src):
    native.cc_library(
        name = name,
        srcs = [src],
        visibility = ["//visibility:public"],
        deps = [
            "//src:unthread",
        ],
        copts = [
            "-I src/include",
            "-fsanitize-coverage=trace-pc-guard,pc-table,trace-cmp,control-flow",
            "-O2",
            "-gline-tables-only",
        ],
    )
    native.cc_binary(
        name = name + "_main",
        srcs = [
            src,
            "main.c",
        ],
        deps = [
            "//src:unthread",
        ],
        visibility = ["//visibility:public"],
        copts = [
            "-I src/include",
        ]
    )

def make_fuzz_target_fuzzers(name):
    native.cc_binary(
        name = name + "_fuzz_schedule",
        srcs = ["fuzz_schedule.c"],
        deps = [
            ":" + name,
            "@com_google_fuzztest//centipede:centipede_runner",
        ],
        visibility = ["//visibility:public"],
        copts = [
            "-I src/include",
        ]
    )
    native.cc_binary(
        name = name + "_fuzz_seed",
        srcs = ["fuzz_seed.c"],
        deps = [
            ":" + name,
            "@com_google_fuzztest//centipede:centipede_runner",
        ],
        visibility = ["//visibility:public"],
        copts = [
            "-I src/include",
        ]
    )


def make_fuzz_target(name):
    make_fuzz_target_library(name, "target_" + name + ".c")
    make_fuzz_target_fuzzers(name)


def make_fuzz_targets(names):
    for name in names:
        make_fuzz_target(name)


def make_fuzz_targets_automatically():
    for name in native.glob(["target_*.c"]):
        make_fuzz_target(name[7:-2])

