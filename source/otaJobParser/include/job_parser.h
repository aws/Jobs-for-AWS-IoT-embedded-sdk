/*
 * Copyright Amazon.com, Inc. and its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: MIT
 *
 * Licensed under the MIT License. See the LICENSE accompanying this file
 * for the specific language governing permissions and limitations under
 * the License.
 */

#ifndef JOB_PARSER_H
#define JOB_PARSER_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct AfrOtaJobDocumentFields_t * AfrOtaJobDocumentFields;

/**
 * @brief Populate the fields of 'result', returning
 * true if successful.
 *
 * @param jobDoc FreeRTOS OTA job document
 * @param jobDocLength OTA job document length
 * @param fileIndex The index of the file to use properties of
 * @param result Job document structure to populate
 * @return true Job document fields were parsed from the document
 * @return false Job document fields were not parsed from the document
 */
bool populateJobDocFields( const char * jobDoc,
                           const size_t jobDocLength,
                           int32_t fileIndex,
                           AfrOtaJobDocumentFields result );

#endif /* JOB_PARSER_H */
