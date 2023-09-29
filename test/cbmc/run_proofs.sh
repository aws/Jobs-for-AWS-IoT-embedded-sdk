#!/bin/sh

coreJSONDir="coreJSON"

UNWIND_COUNT=${UNWIND_COUNT:-10}

#If coreJSON not found, clone it
if [ ! -d "$coreJSONDir" ]; then
    exec git clone https://github.com/FreeRTOS/coreJSON.git
fi

cd "$(dirname -- "$0")/../../" || exit 1

#test/cbmc/coreJSON/source/core_json.c
exec cbmc test/cbmc/proofs.c source/jobs.c test/cbmc/coreJSON/source/core_json.c \
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
     --drop-unused-functions --nondet-static --unwinding-assertions \
     "$@" --unwind "$UNWIND_COUNT" -DUNWIND_COUNT="$UNWIND_COUNT" \
     --c99  >&1 | tee test/cbmc/output/cbmcOutput.txt
