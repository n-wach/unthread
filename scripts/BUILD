load("//scripts:scripts.bzl", "make_fuzz_scripts")

make_fuzz_scripts(
    names = [
        "interleave",
        "checkpoints",
        "bounded_buffer",
        # "elimination_backoff_stack", // spurious exit 11
        # "safestack_relacy", // timeouts
        "workstealqueue_mutex_1",
    ]
)
