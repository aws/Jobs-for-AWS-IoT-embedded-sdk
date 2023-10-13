#!/bin/sh

UNWIND_COUNT=${UNWIND_COUNT:-10}

JobsSourceDir="../../source"
coreJSONSourceDir="../../source/dependency/coreJSON/source"

if [ ! -d "$coreJSONSourceDir" ]; then
    git submodule update --checkout --init --recursive -- ../../source/dependency/coreJSON
fi

exec cbmc proofs.c "$JobsSourceDir/jobs.c"  stubs/strnlen.c \
     stubs/JSON_Validate.c stubs/JSON_SearchT.c \
     -I $JobsSourceDir/include -I $coreJSONSourceDir/include -I include  \
     --unwindset strnAppend.0:26 --unwindset strnEq.0:26 \
     --unwindset matchIdApi.0:84 --unwindset isValidID.0:65 \
     --unwindset strlen.0:51 \
     --bounds-check --pointer-check --memory-cleanup-check --div-by-zero-check \
     --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check \
     --conversion-check --undefined-shift-check --enum-range-check \
     --pointer-primitive-check --drop-unused-functions --nondet-static \
     --unwinding-assertions --c99 "$@" --unwind "$UNWIND_COUNT" --json-ui  \
     -DUNWIND_COUNT="$UNWIND_COUNT" >&1 | tee output/latest/html/run.json
