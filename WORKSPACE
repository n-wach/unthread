workspace(
    name = "unthread",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/ed994039a951b736091776d677f324b3903ef939.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-ed994039a951b736091776d677f324b3903ef939",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
    sha256 = "085bde6c5212c8c1603595341ffe7133108034808d8c819f8978b2b303afc9e7",
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()
# Run with: bazel run @hedron_compile_commands//:refresh_all


# Fuzzing!
http_archive(
    name = "rules_fuzzing",
    sha256 = "d9002dd3cd6437017f08593124fdd1b13b3473c7b929ceb0e60d317cb9346118",
    strip_prefix = "rules_fuzzing-0.3.2",
    urls = ["https://github.com/bazelbuild/rules_fuzzing/archive/v0.3.2.zip"],
)

load("@rules_fuzzing//fuzzing:repositories.bzl", "rules_fuzzing_dependencies")
rules_fuzzing_dependencies()

load("@rules_fuzzing//fuzzing:init.bzl", "rules_fuzzing_init")
rules_fuzzing_init()

