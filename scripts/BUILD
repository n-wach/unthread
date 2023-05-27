sh_binary(
    name = "fuzz_schedule",
    srcs = ["fuzz.sh"],
    data = [
        "@com_google_fuzztest//centipede:centipede",
        "//targets:interleave_fuzz_schedule",
    ],
    args = [
        "$(location @com_google_fuzztest//centipede:centipede)",
        "$(location //targets:interleave_fuzz_schedule)",
    ],
)

sh_binary(
    name = "fuzz_seed",
    srcs = ["fuzz.sh"],
    data = [
        "@com_google_fuzztest//centipede:centipede",
        "//targets:interleave_fuzz_seed",
    ],
    args = [
        "$(location @com_google_fuzztest//centipede:centipede)",
        "$(location //targets:interleave_fuzz_seed)",
    ],
)

