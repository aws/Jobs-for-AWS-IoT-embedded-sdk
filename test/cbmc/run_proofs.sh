#!/bin/sh

coreJSONDir="coreJSON"

UNWIND_COUNT=${UNWIND_COUNT:-10}

#If coreJSON not found, clone it
if [ ! -d "$coreJSONDir" ]; then
    exec git clone https://github.com/FreeRTOS/coreJSON.git
fi

#Create a copy of jobs.c in cbmc/sources, then remove the static keywords in the copy
if [ ! -d "sources/jobs.c" ]; then 
    cp ../../source/jobs.c sources
    sed -I -E '1s/^/\#include \"jobs_annex.h\"\n /; s/^static //g' sources/jobs.c
fi

cd "$(dirname -- "$0")/../../" || exit 1

exec cbmc test/cbmc/proofs.c test/cbmc/sources/jobs.c -Isource/include -Isource \
     -Itest/cbmc/coreJSON/source/include -Itest/cbmc/coreJSON/source \
     -Itest/cbmc/include \
     --unwindset strnAppend.0:15 \
     --bounds-check --pointer-check --memory-cleanup-check --div-by-zero-check \
     --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check \
     --conversion-check --undefined-shift-check --enum-range-check \
     --pointer-primitive-check \
     --drop-unused-functions --nondet-static --unwinding-assertions \
     "$@" --unwind "$UNWIND_COUNT" -DUNWIND_COUNT="$UNWIND_COUNT" \
     --c99 >&1 | tee test/cbmc/output/cbmcOutput.txt
