cc_library(
    name = "interleave",
    srcs = ["interleave.c"],
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

cc_binary(
    name = "interleave_main",
    srcs = ["interleave_main.c"],
    deps = [
        ":interleave",
    ],
    copts = ["-I src/include"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "interleave_fuzz_schedule",
    srcs = ["interleave_fuzz_schedule.c"],
    deps = [
        ":interleave",
        "@com_google_fuzztest//centipede:centipede_runner",
    ],
    copts = [
        "-I src/include",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "interleave_fuzz_seed",
    srcs = ["interleave_fuzz_seed.c"],
    deps = [
        ":interleave",
        "@com_google_fuzztest//centipede:centipede_runner",
    ],
    copts = [
        "-I src/include",
    ],
    visibility = ["//visibility:public"],
)
