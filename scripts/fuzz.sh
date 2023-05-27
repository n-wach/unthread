
CENTIPEDE_BIN=$1
TARGET_BIN=$2

${CENTIPEDE_BIN} --binary="${TARGET_BIN}" --dry_run

echo "==================================="
echo "DRY RUN COMPLETE! FUZZING FOR REAL!"
echo "==================================="

time ${CENTIPEDE_BIN} --binary="${TARGET_BIN}" --exit_on_crash=true

