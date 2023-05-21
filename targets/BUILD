load("@rules_fuzzing//fuzzing:cc_defs.bzl", "cc_fuzz_test")

cc_library(
    name = "interleave",
    srcs = ["interleave.c"],
    visibility = ["//visibility:public"],
    deps = [
        "//src:unthread",
    ],
)

cc_binary(
    name = "interleave_main",
    srcs = ["interleave_main.c"],
    deps = [
        ":interleave",
    ],
    visibility = ["//visibility:public"],
)

cc_fuzz_test(
    name = "interleave_fuzz_schedule",
    srcs = ["interleave_fuzz_schedule.c"],
    deps = [
        ":interleave",
    ],
)

cc_fuzz_test(
    name = "interleave_fuzz_seed",
    srcs = ["interleave_fuzz_seed.c"],
    deps = [
        ":interleave",
    ],
)

# The same thing, but with function "checkpoints"
cc_library(
    name = "checkpoints",
    srcs = ["interleave_checkpoints.c"],
    visibility = ["//visibility:public"],
    deps = [
        "//src:unthread",
    ],
)

cc_binary(
    name = "checkpoints_main",
    srcs = ["interleave_main.c"],
    deps = [
        ":checkpoints",
    ],
    visibility = ["//visibility:public"],
)

cc_fuzz_test(
    name = "checkpoints_fuzz_schedule",
    srcs = ["interleave_fuzz_schedule.c"],
    deps = [
        ":checkpoints",
    ],
)

cc_fuzz_test(
    name = "checkpoints_fuzz_seed",
    srcs = ["interleave_fuzz_seed.c"],
    deps = [
        ":checkpoints",
    ],
)

