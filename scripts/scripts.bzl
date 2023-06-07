def make_fuzz_script(name, fuzz_type):
    native.sh_binary(
        name = name + "_" + fuzz_type,
        srcs = ["fuzz.sh"],
        data = [
            "@com_google_fuzztest//centipede:centipede",
            "//targets:" + name + "_" + fuzz_type,
        ],
        args = [
            "$(location @com_google_fuzztest//centipede:centipede)",
            "$(location //targets:" + name +"_" + fuzz_type + ")",
        ],
    )

def make_fuzz_scripts_for_specific(name):
    make_fuzz_script(name, "fuzz_schedule")
    make_fuzz_script(name, "fuzz_seed")
    make_fuzz_script(name, "fuzz_pthread")


def make_fuzz_scripts(names):
    for name in names:
        make_fuzz_scripts_for_specific(name)

