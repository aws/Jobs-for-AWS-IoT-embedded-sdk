# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

HARNESS_FILE=Jobs_Update_harness

# Limit thing name and job id maximums to reduce unwindings.
THINGNAME_MAX_LENGTH=6
EXCEED_THINGNAME_MAX_LENGTH=7
JOBID_MAX_LENGTH=4
EXCEED_JOBID_MAX_LENGTH=5

REMOVE_FUNCTION_BODY +=
# Use the largest of the above
UNWINDSET += isValidID.0:$(EXCEED_THINGNAME_MAX_LENGTH)

# Use a stub in place of the original function.
PROOF_SOURCES += $(PROOF_STUB)/strnAppend.c
SED_EXPR = s/JobsStatus_t strnAppend\b/&_/

include ../Makefile-jobs.common
