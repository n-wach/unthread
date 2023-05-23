load("@rules_fuzzing//fuzzing:cc_defs.bzl", "cc_fuzz_test")

cc_library(
    name = "interleave",
    srcs = ["interleave.c"],
    visibility = ["//visibility:public"],
    deps = [
        "//src:unthread",
    ],
    copts = ["-I src/include"],
)

cc_binary(
    name = "interleave_main",
    srcs = ["interleave_main.c"],
    deps = [
        ":interleave",
    ],
    visibility = ["//visibility:public"],
    copts = ["-I src/include"],
)

cc_fuzz_test(
    name = "interleave_fuzz_schedule",
    srcs = ["interleave_fuzz_schedule.c"],
    deps = [
        ":interleave",
    ],
    copts = ["-I src/include"],
)

cc_fuzz_test(
    name = "interleave_fuzz_seed",
    srcs = ["interleave_fuzz_seed.c"],
    deps = [
        ":interleave",
    ],
    copts = ["-I src/include"],
)

# The same thing, but with function "checkpoints"
cc_library(
    name = "checkpoints",
    srcs = ["interleave_checkpoints.c"],
    visibility = ["//visibility:public"],
    deps = [
        "//src:unthread",
    ],
    copts = ["-I src/include"],
)

cc_binary(
    name = "checkpoints_main",
    srcs = ["interleave_main.c"],
    deps = [
        ":checkpoints",
    ],
    visibility = ["//visibility:public"],
    copts = ["-I src/include"],
)

cc_fuzz_test(
    name = "checkpoints_fuzz_schedule",
    srcs = ["interleave_fuzz_schedule.c"],
    deps = [
        ":checkpoints",
    ],
    copts = ["-I src/include"],
)

cc_fuzz_test(
    name = "checkpoints_fuzz_seed",
    srcs = ["interleave_fuzz_seed.c"],
    deps = [
        ":checkpoints",
    ],
    copts = ["-I src/include"],
)

