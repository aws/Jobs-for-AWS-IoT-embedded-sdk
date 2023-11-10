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

/**
 * @ingroup jobs_structs
 * @brief struct containing the fields of an AFR OTA Job Document
 */
typedef struct
{
    const char * signature;  /**< @brief Code Signing Signature */
    size_t signatureLen;     /**< @brief Length of signature */
    const char * filepath;   /**< @brief File path to store OTA Update on device */
    size_t filepathLen;      /**< @brief Length of filepath */
    const char * certfile;   /**< @brief Path to Code Signing Certificate on Device */
    size_t certfileLen;      /**< @brief Length of certfile */
    const char * authScheme; /**< @brief Authentication Scheme for HTTP URL ( null for MQTT  )*/
    size_t authSchemeLen;    /**< @brief Length of authScheme */
    const char * imageRef;   /**< @brief MQTT Stream or HTTP URL */
    size_t imageRefLen;      /**< @brief Length of imageRef */
    uint32_t fileId;         /**< @brief File ID */
    uint32_t fileSize;       /**< @brief Size of the OTA Update */
    uint32_t fileType;       /**< @brief FIle Type */
} AfrOtaJobDocumentFields_t;

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
/* @[declare_populatejobdocfields] */
bool populateJobDocFields( const char * jobDoc,
                           const size_t jobDocLength,
                           int32_t fileIndex,
                           AfrOtaJobDocumentFields_t * result );
/* @[declare_populatejobdocfields] */

#endif /* JOB_PARSER_H */
