# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

HARNESS_FILE=Jobs_MatchTopic_harness

# Limit thing name and job id maximums to reduce unwindings.
THINGNAME_MAX_LENGTH=4
EXCEED_THINGNAME_MAX_LENGTH=5
JOBID_MAX_LENGTH=4
EXCEED_JOBID_MAX_LENGTH=5

# This size holds the longest topic, including thing name and job id
CBMC_MAX_BUFSIZE=50

# Provide sufficient unwindings to iterate through the API enums
APIS_WITHOUT_JOBID=7
APIS_WITH_JOBID=6

REMOVE_FUNCTION_BODY +=
UNWINDSET += isValidThingName.0:$(EXCEED_THINGNAME_MAX_LENGTH)
UNWINDSET += isValidJobId.0:$(EXCEED_JOBID_MAX_LENGTH)
UNWINDSET += matchApi.0:$(APIS_WITHOUT_JOBID)
UNWINDSET += matchIdApi.0:$(CBMC_MAX_BUFSIZE)
UNWINDSET += matchIdApi.1:$(APIS_WITH_JOBID)

# Use a stub in place of the original function.
PROOF_SOURCES += $(PROOF_STUB)/strnEq.c
SED_EXPR = s/JobsStatus_t strnEq\b/&_/

include ../Makefile-jobs.common
