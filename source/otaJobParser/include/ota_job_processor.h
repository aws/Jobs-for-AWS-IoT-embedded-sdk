/*
 * Copyright Amazon.com, Inc. and its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: MIT
 *
 * Licensed under the MIT License. See the LICENSE accompanying this file
 * for the specific language governing permissions and limitations under
 * the License.
 */

#ifndef OTA_JOB_PROCESSOR_H
#define OTA_JOB_PROCESSOR_H

#include <stdint.h>

#include <stddef.h>
#include <stdint.h>

#include "job_parser.h"

int8_t otaParser_parseJobDocFile( const char * jobDoc,
                                   const size_t jobDocLength,
                                   const uint8_t fileIndex,
                                   AfrOtaJobDocumentFields fields );

#endif /*OTA_JOB_PROCESSOR_H*/
