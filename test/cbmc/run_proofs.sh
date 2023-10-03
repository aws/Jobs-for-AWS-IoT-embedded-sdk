#!/bin/sh

coreJSONDir="coreJSON"

UNWIND_COUNT=${UNWIND_COUNT:-10}

#If coreJSON not found, clone it
if [ ! -d "$coreJSONDir" ]; then
    git clone https://github.com/FreeRTOS/coreJSON.git --depth 1
fi

cd "$(dirname -- "$0")/../../" || exit 1

exec cbmc test/cbmc/proofs.c source/jobs.c test/cbmc/coreJSON/source/core_json.c \
     test/cbmc/stubs/strnlen.c test/cbmc/stubs/JSON_Validate.c \
     test/cbmc/stubs/JSON_SearchT.c \
     -Itest/cbmc/coreJSON/source/include -Isource/include -Itest/cbmc/include \
     --unwindset strnAppend.0:15 --unwindset strnEq.0:26 \
     --unwindset matchIdApi.0:84 --unwindset isValidID.0:65 \
     --unwindset strlen.0:36 \
     --bounds-check --pointer-check --memory-cleanup-check --div-by-zero-check \
     --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check \
     --conversion-check --undefined-shift-check --enum-range-check \
     --pointer-primitive-check --drop-unused-functions --nondet-static \
     --unwinding-assertions --c99 --trace "$@" --unwind "$UNWIND_COUNT" \
     -DUNWIND_COUNT="$UNWIND_COUNT" >&1 | tee test/cbmc/output/cbmcOutput.txt
