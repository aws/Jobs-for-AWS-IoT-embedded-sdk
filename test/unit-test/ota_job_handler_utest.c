/*
 * Copyright Amazon.com, Inc. and its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: MIT
 *
 * Licensed under the MIT License. See the LICENSE accompanying this file
 * for the specific language governing permissions and limitations under
 * the License.
 */

#include <string.h>

#include "unity.h"

#include "mock_job_parser.h"
#include "ota_job_processor.h"

#define JOB_DOC_ID              "jobDocId"
#define JOB_DOC_ID_LEN          8U
#define AFR_OTA_DOCUMENT        "{\"afr_ota\":{\"files\":[{\"filesize\":123456789}]}}"
#define AFR_OTA_DOCUMENT_LENGTH ( sizeof( AFR_OTA_DOCUMENT ) - 1U )
#define MULTI_FILE_OTA_DOCUMENT                                    \
    "{\"afr_ota\":{\"files\":[{\"filesize\":1},{\"filesize\":2},{" \
    "\"filesize\":3}]}}"
#define MULTI_FILE_OTA_DOCUMENT_LENGTH \
    ( sizeof( MULTI_FILE_OTA_DOCUMENT ) - 1U )
#define TOO_MANY_FILES_OTA_DOCUMENT                                        \
    "{\"afr_ota\":{\"files\":[{\"filesize\":1},{\"filesize\":2},{"         \
    "\"filesize\":3},{\"filesize\":4},{\"filesize\":5},{\"filesize\":6},{" \
    "\"filesize\":7},{\"filesize\":8},{\"filesize\":9},{\"filesize\":10}]}}"
#define TOO_MANY_FILES_OTA_DOCUMENT_LENGTH \
    ( sizeof( TOO_MANY_FILES_OTA_DOCUMENT ) - 1U )
#define CUSTOM_DOCUMENT        "{\"custom_job\":\"test\"}"
#define CUSTOM_DOCUMENT_LENGTH ( sizeof( CUSTOM_DOCUMENT ) - 1U )

AfrOtaJobDocumentFields parsedFields;

/* ===========================   UNITY FIXTURES ============================ */

/* Called before each test method. */
void setUp()
{
}

/* Called after each test method. */
void tearDown()
{
}

/* Called at the beginning of the whole suite. */
void suiteSetUp()
{
}

/* Called at the end of the whole suite. */
int suiteTearDown( int numFailures )
{
    return numFailures;
}

/* ===============================   TESTS   =============================== */

void test_parseJobDocFile_returnsZero_whenSingleFileJob( void )
{
    populateJobDocFields_ExpectAndReturn( AFR_OTA_DOCUMENT,
                                          AFR_OTA_DOCUMENT_LENGTH,
                                          0,
                                          parsedFields,
                                          true );

    int8_t result = otaParser_parseJobDocFile( AFR_OTA_DOCUMENT,
                                          AFR_OTA_DOCUMENT_LENGTH,
                                          0U,
                                          parsedFields);

    TEST_ASSERT_EQUAL( 0, result );
}

void test_parseJobDocFile_returnsNextIndex_whenMultiFileIOTOtaJob( void )
{
    populateJobDocFields_ExpectAndReturn( MULTI_FILE_OTA_DOCUMENT,
                                          MULTI_FILE_OTA_DOCUMENT_LENGTH,
                                          0,
                                          parsedFields,
                                          true );

    populateJobDocFields_ExpectAndReturn( MULTI_FILE_OTA_DOCUMENT,
                                          MULTI_FILE_OTA_DOCUMENT_LENGTH,
                                          1,
                                          parsedFields,
                                          true );

    int8_t result = otaParser_parseJobDocFile( MULTI_FILE_OTA_DOCUMENT,
                                             MULTI_FILE_OTA_DOCUMENT_LENGTH,
                                             0U,
                                             parsedFields );

    TEST_ASSERT_EQUAL( 1, result );

    result = otaParser_parseJobDocFile( MULTI_FILE_OTA_DOCUMENT,
                                            MULTI_FILE_OTA_DOCUMENT_LENGTH,
                                            1U,
                                            parsedFields );

    TEST_ASSERT_EQUAL( 2, result );
}

void test_parseJobDocFile_returnsZero_whenLastFileIndex( void )
{
    populateJobDocFields_ExpectAndReturn( MULTI_FILE_OTA_DOCUMENT,
                                          MULTI_FILE_OTA_DOCUMENT_LENGTH,
                                          2,
                                          parsedFields,
                                          true );

    int8_t result = otaParser_parseJobDocFile( MULTI_FILE_OTA_DOCUMENT,
                                             MULTI_FILE_OTA_DOCUMENT_LENGTH,
                                             2U,
                                             parsedFields);

    TEST_ASSERT_EQUAL( 0, result );
}

void test_parseJobDocFile_returnsNegativeOne_whenIndexOutOfRange( void )
{
    int8_t result = otaParser_parseJobDocFile( AFR_OTA_DOCUMENT,
                                       AFR_OTA_DOCUMENT_LENGTH,
                                             1U,
                                             parsedFields);

    TEST_ASSERT_EQUAL( -1, result );
}

void test_parseJobDocFile_returnsNegativeOne_whenParsingFails( void )
{
    populateJobDocFields_ExpectAndReturn( AFR_OTA_DOCUMENT,
                                          AFR_OTA_DOCUMENT_LENGTH,
                                          0,
                                          NULL,
                                          false );
    populateJobDocFields_IgnoreArg_result();

    int8_t result = otaParser_parseJobDocFile( AFR_OTA_DOCUMENT,
                                             AFR_OTA_DOCUMENT_LENGTH,
                                             0U,
                                             parsedFields );

    TEST_ASSERT_EQUAL( -1, result );
}

void test_parseJobDocFile_returnsNegativeOne_whenMultiFileParsingFails( void )
{
    populateJobDocFields_ExpectAndReturn( MULTI_FILE_OTA_DOCUMENT,
                                          MULTI_FILE_OTA_DOCUMENT_LENGTH,
                                          0,
                                          NULL,
                                          false );
    populateJobDocFields_IgnoreArg_result();

    int8_t result = otaParser_parseJobDocFile( MULTI_FILE_OTA_DOCUMENT,
                                             MULTI_FILE_OTA_DOCUMENT_LENGTH,
                                             0,
                                             parsedFields );

    TEST_ASSERT_EQUAL( -1, result );
}

void test_parseJobDocFile_returnsNegativeOne_whenCustomJob( void )
{
    int8_t result = otaParser_parseJobDocFile( CUSTOM_DOCUMENT,
                                             CUSTOM_DOCUMENT_LENGTH,
                                             0U,
                                             parsedFields );

    TEST_ASSERT_EQUAL( -1, result );
}

void test_parseJobDocFile_returnsFalse_givenNullJobDocument( void )
{
    int8_t result = otaParser_parseJobDocFile( NULL,
                                            CUSTOM_DOCUMENT_LENGTH,
                                            0U,
                                            parsedFields );

    TEST_ASSERT_EQUAL( -1, result );
}

void test_parseJobDocFile_returnsFalse_givenZeroDocumentLength( void )
{
    int8_t result = otaParser_parseJobDocFile( AFR_OTA_DOCUMENT,
                                          0U,
                                          0U,
                                          parsedFields  );

    TEST_ASSERT_EQUAL( -1, result );
}
