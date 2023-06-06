load("//scripts:scripts.bzl", "make_fuzz_scripts")

make_fuzz_scripts(
    names = [
        "interleave",
        "checkpoints",
        "bounded_buffer",
        # "elimination_backoff_stack", # spurious exit 11
        # "safestack_relacy", # timeouts
        "workstealqueue_mutex_1",
        # "workstealqueue_mutex_2", # not finding bug...
        "workstealqueue_mutex_3",
        "bigshot_p",
        # "indexer", # timeouts
        "lazy",
        "queue",
        # "queue_ok", # i think this is a patched version of queue?
        "singleton_b",
        "stack_2",
        # "triangular", # spurious exit 4
        # "szymanski", # deadlocks. (infinite loop)
        "phase_1", # deadlocks, detected.
        "deadlock_1", # deadlocks, detected.
    ]
)
