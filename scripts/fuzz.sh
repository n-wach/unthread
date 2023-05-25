set -e

rm -r bin
mkdir -p bin

TARGET=interleave_fuzz_seed

bazel build --config=centipede //targets:${TARGET}

# We need to copy the executable because starlark doesn't allow
# providing an executable not created by the rule
cp bazel-bin/targets/${TARGET} bin/

bazel build -c opt @com_google_fuzztest//centipede:centipede
cp bazel-bin/external/com_google_fuzztest/centipede/centipede bin/

cd bin/
./centipede --binary=./${TARGET} --dry_run

echo "==================================="
echo "DRY RUN COMPLETE! FUZZING FOR REAL!"
echo "==================================="

time ./centipede --binary=./${TARGET} --exit_on_crash=true
