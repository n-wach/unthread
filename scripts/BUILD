load("//scripts:scripts.bzl", "make_fuzz_scripts")

make_fuzz_scripts(
    names = [
        "interleave",
        "checkpoints",
        "bounded_buffer",
        "workstealqueue_mutex_1",
        "workstealqueue_mutex_3",
        "bigshot_p",
        "lazy",
        "singleton_b",
        "triangular",
        "stringbuffer",
        
        "phase_1", # deadlocks, detected.
        "deadlock_1", # deadlocks, detected.

        # "szymanski", # timeout. (infinite loop)
        # "safestack_relacy", # timeout. (infinite loop)

        # "indexer", # I'm not sure what the bug is... not finding anything
        # "stack_2", # I'm not sure what the bug is... not finding anything
        # "workstealqueue_mutex_2", # I'm not sure what the bug is...
        # "elimination_backoff_stack", # crashing.
        # "queue", # dumb test. trivial bug that triggers on every run.
        # "queue_ok", # queue but patched to remove bug.
    ]
)
