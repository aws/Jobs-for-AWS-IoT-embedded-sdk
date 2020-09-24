# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

HARNESS_FILE=Jobs_GetTopic_harness

THINGNAME_MAX_LENGTH=4
EXCEED_THINGNAME_MAX_LENGTH=5

REMOVE_FUNCTION_BODY +=
UNWINDSET += isValidThingName.0:$(EXCEED_THINGNAME_MAX_LENGTH)

PROOF_SOURCES += $(PROOF_STUB)/strnAppend.c
SED_EXPR = s/JobsStatus_t strnAppend\b/&_/

include ../Makefile-jobs.common
