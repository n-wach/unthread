def make_fuzz_script(name):
    native.sh_binary(
        name = name + "_fuzz_schedule",
        srcs = ["fuzz.sh"],
        data = [
            "@com_google_fuzztest//centipede:centipede",
            "//targets:" + name + "_fuzz_schedule",
        ],
        args = [
            "$(location @com_google_fuzztest//centipede:centipede)",
            "$(location //targets:" + name + "_fuzz_schedule)",
        ],
    )
    native.sh_binary(
        name = name + "_fuzz_seed",
        srcs = ["fuzz.sh"],
        data = [
            "@com_google_fuzztest//centipede:centipede",
            "//targets:" + name + "_fuzz_seed",
        ],
        args = [
            "$(location @com_google_fuzztest//centipede:centipede)",
            "$(location //targets:" + name + "_fuzz_seed)",
        ],
    )


def make_fuzz_scripts(names):
    for name in names:
        make_fuzz_script(name)
