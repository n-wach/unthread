cc_binary(
    name = "interleave",
    srcs = ["interleave.c"],
    deps = [
        "//src:unthread",
    ],
)

load("@rules_fuzzing//fuzzing:cc_defs.bzl", "cc_fuzz_test")

cc_fuzz_test(
    name = "fuzz_test",
    srcs = ["fuzz_test.cc"],
)

cc_fuzz_test(
    name = "interleave_fuzz_test",
    srcs = ["interleave_fuzz_test.c"],
    deps = [
        "//src:unthread",
    ],
)
