import subprocess

TIMEOUT_SECONDS = 60 * 15
MAX_RUNS = 100

def run_script(script):
    print("Timing " + script + "...\r", end="")
    raw_output = subprocess.check_output(["bazel", "run", script], timeout=TIMEOUT_SECONDS, stderr=subprocess.DEVNULL)
    output = raw_output.decode("utf-8")

    real_time = output.split("\n")[-4]
    assert real_time.startswith("real\t")
    real_time = real_time[len("real\t"):]
    real_time = real_time.split("m")
    minutes = int(real_time[0])
    seconds = float(real_time[1][:-1])
    total_seconds = minutes * 60 + seconds
    return total_seconds


TIMES = {}


def time_scripts():
    raw_scripts = subprocess.check_output(["bazel", "query", "//scripts:*"], stderr=subprocess.DEVNULL)
    all_scripts = raw_scripts.decode("utf-8").split("\n")

    fuzz_scripts = [s for s in all_scripts if "_fuzz_" in s]
    for script in fuzz_scripts:
        # format: //scripts:bounded_buffer_fuzz_schedule
        name, fuzzer = script.split(":")[1].split("_fuzz_")
        if name not in TIMES:
            TIMES[name] = {}
        TIMES[name][fuzzer] = 0

    print("Timing " + str(len(fuzz_scripts)) + " scripts")

    for script in fuzz_scripts:
        name, fuzzer = script.split(":")[1].split("_fuzz_")
        allotted_time = TIMEOUT_SECONDS
        try:
            runs = 0
            total_seconds = 0
            while total_seconds < allotted_time and runs < MAX_RUNS:
                total_seconds += run_script(script)
                runs += 1
            average_time = total_seconds / runs
            rounded_time_ms = int(average_time * 1000)
            TIMES[name][fuzzer] = rounded_time_ms
            print(script + ": " + str(rounded_time_ms) + "ms over " + str(runs) + " runs")

        except subprocess.TimeoutExpired:
            TIMES[name][fuzzer] = "timeout"
            print(script + ": timeout")

        except subprocess.CalledProcessError as e:
            TIMES[name][fuzzer] = "error"
            print(script + ": error (" + str(e) + ")")

    print("")
    print("Results:")
    results_str = "name pthread schedule seed\n"
    for name in TIMES:
        out = name
        out += " " + str(TIMES[name]["pthread"])
        out += " " + str(TIMES[name]["schedule"])
        out += " " + str(TIMES[name]["seed"])
        results_str += out + "\n"
    print(results_str)
    with open("results.txt", "w") as f:
        f.write(results_str)


if __name__ == "__main__":
    time_scripts()
