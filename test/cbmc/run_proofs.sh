#!/bin/sh

coreJSONDir="coreJSON"

UNWIND_COUNT=${UNWIND_COUNT:-10}

#If coreJSON not found, clone it
if [ ! -d "$coreJSONDir" ]; then
    exec git clone https://github.com/FreeRTOS/coreJSON.git
fi

cd "$(dirname -- "$0")/../../" || exit 1

# goto-cc  -Isource/include -Itest/cbmc/coreJSON/source/include \
#     -Itest/cbmc/include test/cbmc/proofs.c -o proofs.goto

# goto-cc -Isource/include -Itest/cbmc/coreJSON/source/include \
#     -Itest/cbmc/include source/jobs.c -o jobs.goto

# goto-cc -Isource/include -Itest/cbmc/coreJSON/source/include \
#     -Itest/cbmc/include  test/cbmc/coreJSON/source/core_json.c \
#     -o core_json.goto

# goto-cc -Isource/include -Itest/cbmc/coreJSON/source/include \
#     -Itest/cbmc/include test/cbmc/stubs/strnlen.c -o strnlen.goto

# goto-instrument --nondet-static-exclude jobs.c:jobStatusString \
#     --nondet-static-exclude jobs.c:jobStatusStringLengths \
#     --nondet-static-exclude jobs.c:jobUpdateStatusString \
#     --nondet-static-exclude jobs.c:jobUpdateStatusStringLengths jobs.goto jobs2.goto

# cbmc proofs.goto jobs2.goto core_json.goto strnlen.goto \
#      --unwindset strnAppend.0:15 \
#      --unwindset strnEq.0:26 \
#      --unwindset matchIdApi.0:84 \
#      --unwindset isValidID.0:65 \
#      --unwindset strlen.0:36 \
#      --bounds-check --pointer-check --memory-cleanup-check --div-by-zero-check \
#      --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check \
#      --conversion-check --undefined-shift-check --enum-range-check \
#      --pointer-primitive-check \
#      --drop-unused-functions --nondet-static --unwinding-assertions \
#      "$@" --unwind "$UNWIND_COUNT" -DUNWIND_COUNT="$UNWIND_COUNT" \
#      --c99  --trace >&1 | tee test/cbmc/output/cbmcOutput.txt


exec cbmc test/cbmc/proofs.c source/jobs.c test/cbmc/coreJSON/source/core_json.c \
     test/cbmc/stubs/strnlen.c test/cbmc/stubs/JSON_Validate.c \
     test/cbmc/stubs/JSON_SearchT.c \
     -Itest/cbmc/coreJSON/source/include -Isource/include -Isource \
     -Itest/cbmc/include \
     --unwindset strnAppend.0:15 \
     --unwindset strnEq.0:26 \
     --unwindset matchIdApi.0:84 \
     --unwindset isValidID.0:65 \
     --unwindset strlen.0:36 \
     --bounds-check --pointer-check --memory-cleanup-check --div-by-zero-check \
     --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check \
     --conversion-check --undefined-shift-check --enum-range-check \
     --pointer-primitive-check \
     --drop-unused-functions --unwinding-assertions \
     "$@" --unwind "$UNWIND_COUNT" -DUNWIND_COUNT="$UNWIND_COUNT" \
     --c99  --trace >&1 | tee test/cbmc/output/cbmcOutput.txt
