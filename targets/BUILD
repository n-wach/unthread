load("//targets:targets.bzl", "make_fuzz_targets_automatically", "make_fuzz_target_library", "make_fuzz_target_fuzzers")

make_fuzz_targets_automatically()


make_fuzz_target_library("stringbuffer", "target_stringbuffer.cc")
make_fuzz_target_fuzzers("stringbuffer")
