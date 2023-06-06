def make_fuzz_target(name):
    native.cc_library(
        name = name,
        srcs = ["target_" + name + ".c"],
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
            "target_" + name + ".c",
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
