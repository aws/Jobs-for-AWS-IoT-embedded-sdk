#!/bin/sh

coreJSONDir="coreJSON"

UNWIND_COUNT=${UNWIND_COUNT:-20}

#If coreJSON not found, clone it
if [ ! -d "$coreJSONDir" ]; then
    exec git clone https://github.com/FreeRTOS/coreJSON.git
fi

cd "$(dirname -- "$0")/../../" || exit 1

exec cbmc test/cbmc/proofs.c source/jobs.c -Isource/include -Isource \
     -Itest/cbmc/coreJSON/source/include -Itest/cbmc/coreJSON/source \
     -Itest/cbmc/include \
     --bounds-check --pointer-check --memory-cleanup-check --div-by-zero-check \
     --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check \
     --conversion-check --undefined-shift-check --enum-range-check \
     --pointer-primitive-check \
     --drop-unused-functions --nondet-static --unwinding-assertions \
     "$@" --unwind "$UNWIND_COUNT" -DUNWIND_COUNT="$UNWIND_COUNT"
