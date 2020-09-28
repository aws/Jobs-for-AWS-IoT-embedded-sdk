/*
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file jobs_utest.c
 * @brief Unit tests for the jobs library.
 */

#include "unity.h"

/* Include paths for public enums, structures, and macros. */
#include "jobs.h"

/* thing name */
#define name           "foobar"
#define nameLength     ( sizeof( name ) - 1 )

/* job ID */
#define jobId          "1234"
#define jobIdLength    ( sizeof( jobId ) - 1 )

/* ============================   UNITY FIXTURES ============================ */

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

/* ========================================================================== */

/**
 * @brief Test that all topic enums are contiguous
 *
 * In the library, a variable of type JobsTopic_t is used for iteration.
 * This test ensures the safety of that use case.
 */
void test_Jobs_topics_are_contiguous( void )
{
    uint16_t bits = 0;

    /* JobsInvalidTopic must be first. */
    bits |= 1 << ( JobsInvalidTopic + 1 );
    TEST_ASSERT_EQUAL( 0x0001, bits );

    /* Topics without a job ID must be grouped next. */
    bits |= 1 << ( JobsJobsChanged + 1 );
    bits |= 1 << ( JobsNextJobChanged + 1 );
    bits |= 1 << ( JobsGetPendingSuccess + 1 );
    bits |= 1 << ( JobsGetPendingFailed + 1 );
    bits |= 1 << ( JobsStartNextSuccess + 1 );
    bits |= 1 << ( JobsStartNextFailed + 1 );
    TEST_ASSERT_EQUAL( 0x007F, bits );

    /* Topics with a job ID must be grouped next. */
    bits |= 1 << ( JobsDescribeSuccess + 1 );
    bits |= 1 << ( JobsDescribeFailed + 1 );
    bits |= 1 << ( JobsUpdateSuccess + 1 );
    bits |= 1 << ( JobsUpdateFailed + 1 );
    TEST_ASSERT_EQUAL( 0x07FF, bits );

    /* JobsMaxTopic must be last. */
    bits |= 1 << ( JobsMaxTopic + 1 );
    TEST_ASSERT_EQUAL( 0x0FFF, bits );
}

/**
 * @brief Test that all parameters are checked for illegal values.
 */
void test_Jobs_bad_parameters( void )
{
    JobsStatus_t ret;
    char buf[ JOBS_API_MAX_LENGTH( nameLength ) ];
    JobsTopic_t outApi;
    char * outJobId;
    uint16_t outJobIdLength;
    size_t outLength;

    /* buf is NULL */
    ret = Jobs_GetTopic( NULL, sizeof( buf ), name, nameLength, JobsUpdateSuccess, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_MatchTopic( NULL, sizeof( buf ), name, nameLength, &outApi, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_GetPending( NULL, sizeof( buf ), name, nameLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_StartNext( NULL, sizeof( buf ), name, nameLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Describe( NULL, sizeof( buf ), name, nameLength, jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Update( NULL, sizeof( buf ), name, nameLength, jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );

    /* length is 0 */
    ret = Jobs_GetTopic( buf, 0, name, nameLength, JobsUpdateSuccess, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_MatchTopic( buf, 0, name, nameLength, &outApi, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_GetPending( buf, 0, name, nameLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_StartNext( buf, 0, name, nameLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Describe( buf, 0, name, nameLength, jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Update( buf, 0, name, nameLength, jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );

    /* name is NULL */
    ret = Jobs_GetTopic( buf, sizeof( buf ), NULL, nameLength, JobsUpdateSuccess, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_MatchTopic( buf, sizeof( buf ), NULL, nameLength, &outApi, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_GetPending( buf, sizeof( buf ), NULL, nameLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_StartNext( buf, sizeof( buf ), NULL, nameLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Describe( buf, sizeof( buf ), NULL, nameLength, jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Update( buf, sizeof( buf ), NULL, nameLength, jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );

    /* bad thing name - variations tested separately */
    ret = Jobs_GetTopic( buf, sizeof( buf ), "!", nameLength, JobsUpdateSuccess, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_MatchTopic( buf, sizeof( buf ), "!", nameLength, &outApi, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_GetPending( buf, sizeof( buf ), "!", nameLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_StartNext( buf, sizeof( buf ), "!", nameLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Describe( buf, sizeof( buf ), "!", nameLength, jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Update( buf, sizeof( buf ), "!", nameLength, jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );

    /* bad thing name length */
    ret = Jobs_GetTopic( buf, sizeof( buf ), name, 0, JobsUpdateSuccess, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_MatchTopic( buf, sizeof( buf ), name, 0, &outApi, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_GetPending( buf, sizeof( buf ), name, 0, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_StartNext( buf, sizeof( buf ), name, 0, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Describe( buf, sizeof( buf ), name, 0, jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Update( buf, sizeof( buf ), name, 0, jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_GetTopic( buf, sizeof( buf ), name, ( THINGNAME_MAX_LENGTH + 1 ), JobsUpdateSuccess, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_MatchTopic( buf, sizeof( buf ), name, ( THINGNAME_MAX_LENGTH + 1 ), &outApi, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_GetPending( buf, sizeof( buf ), name, ( THINGNAME_MAX_LENGTH + 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_StartNext( buf, sizeof( buf ), name, ( THINGNAME_MAX_LENGTH + 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Describe( buf, sizeof( buf ), name, ( THINGNAME_MAX_LENGTH + 1 ), jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Update( buf, sizeof( buf ), name, ( THINGNAME_MAX_LENGTH + 1 ), jobId, jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );

    /* bad job ID - variations tested separately */
    ret = Jobs_Describe( buf, sizeof( buf ), name, nameLength, "!", jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Update( buf, sizeof( buf ), name, nameLength, "!", jobIdLength, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );

    /* bad job ID length */
    ret = Jobs_Describe( buf, sizeof( buf ), name, nameLength, jobId, 0, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Update( buf, sizeof( buf ), name, nameLength, jobId, 0, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Describe( buf, sizeof( buf ), name, nameLength, jobId, ( JOBID_MAX_LENGTH + 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_Update( buf, sizeof( buf ), name, nameLength, jobId, ( JOBID_MAX_LENGTH + 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );

    /* outAPI is NULL */
    ret = Jobs_MatchTopic( buf, sizeof( buf ), name, nameLength, NULL, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );

    /* bad API value */
    ret = Jobs_GetTopic( buf, sizeof( buf ), name, nameLength, ( JobsInvalidTopic - 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
    ret = Jobs_GetTopic( buf, sizeof( buf ), name, nameLength, JobsMaxTopic, &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );
}

/**
 * @brief Test thing name and job ID validation
 *
 * The library validation code is shared for the two types of identifiers.
 */
void test_Jobs_valid_identifiers( void )
{
    JobsStatus_t ret;
    char buf[ JOBS_API_MAX_LENGTH( THINGNAME_MAX_LENGTH ) ];
    size_t outLength;
    char c;

#define commonChars                      \
    "-" "_" "0123456789"                 \
            "ABCDEFGHIJKLMNOPQRSTUVWYXZ" \
            "abcdefghijklmnopqrstuvwyxz"

    char testChars[] = commonChars;

    ret = Jobs_GetPending( buf, sizeof( buf ), testChars, ( sizeof( testChars ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );

    ret = Jobs_Describe( buf, sizeof( buf ), name, nameLength, testChars, ( sizeof( testChars ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );

    /* thing name allows colon. */
    testChars[ 0 ] = ':';
    ret = Jobs_GetPending( buf, sizeof( buf ), testChars, ( sizeof( testChars ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );

    /* job ID disallows colon. */
    ret = Jobs_Describe( buf, sizeof( buf ), name, nameLength, testChars, ( sizeof( testChars ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsBadParameter, ret );

    /* Test all ASCII characters excluding NUL. */
    for( c = 1; c > 0; c++ )
    {
        testChars[ 0 ] = c;
        ret = Jobs_GetPending( buf, sizeof( buf ), testChars, ( sizeof( testChars ) - 1 ), &outLength );

        if( ( c == ':' ) || ( c == '-' ) || ( c == '_' ) || isalnum( c ) )
        {
            TEST_ASSERT_EQUAL( JobsSuccess, ret );
        }
        else
        {
            TEST_ASSERT_EQUAL( JobsBadParameter, ret );
        }
    }
}

/**
 * @brief Test many buffer sizes for correct behavior
 *
 * The string append code is shared by all functions that write a buffer.
 * This code guarantees that any non-zero length buffer will be NUL terminated.
 */
void test_Jobs_buffer_lengths( void )
{
    char buf[ JOBS_API_MAX_LENGTH( nameLength ) ];
    JobsStatus_t ret;
    size_t len;
    int i = 0;
    char expected1[] = JOBS_API_PREFIX name JOBS_API_BRIDGE JOBS_API_JOBSCHANGED;
    char expected2[] = JOBS_API_PREFIX name JOBS_API_BRIDGE "+/" JOBS_API_UPDATE JOBS_API_SUCCESS;

    ret = Jobs_GetTopic( buf, sizeof( buf ), name, nameLength, JobsJobsChanged, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL_STRING( expected1, buf );

    ret = Jobs_GetTopic( buf, sizeof( buf ), name, nameLength, JobsUpdateSuccess, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL_STRING( expected2, buf );

    for( i = 1; i < sizeof( expected2 ); i++ )
    {
        ret = Jobs_GetTopic( buf, i, name, nameLength, JobsUpdateSuccess, &len );
        TEST_ASSERT_EQUAL( JobsBufferTooSmall, ret );
        TEST_ASSERT_EQUAL( '\0', buf[ len ] );
    }
}

/**
 * @brief Test that the written buffers match expectations
 *
 * @note When a buffer is too small, it is still written appropriately
 * to the length specified.
 */
void test_Jobs_happy_path( void )
{
    JobsStatus_t ret;
    char buf[ JOBS_API_MAX_LENGTH( nameLength ) ];
    size_t outLength;

#define PREFIX    JOBS_API_PREFIX name JOBS_API_BRIDGE
    char prefix[] = PREFIX;

    {
        char expected[] = PREFIX JOBS_API_GETPENDING;

        ret = Jobs_GetPending( buf, sizeof( buf ), name, nameLength, &outLength );
        TEST_ASSERT_EQUAL( JobsSuccess, ret );
        TEST_ASSERT_EQUAL_STRING( expected, buf );
        TEST_ASSERT_EQUAL( ( sizeof( expected ) - 1 ), outLength );

        ret = Jobs_GetPending( buf, sizeof( buf ), name, nameLength, NULL );
        TEST_ASSERT_EQUAL( JobsSuccess, ret );
        TEST_ASSERT_EQUAL_STRING( expected, buf );

        ret = Jobs_GetPending( buf, sizeof( prefix ), name, nameLength, NULL );
        TEST_ASSERT_EQUAL( JobsBufferTooSmall, ret );
        TEST_ASSERT_EQUAL_STRING( prefix, buf );
    }

    {
        char expected[] = PREFIX JOBS_API_STARTNEXT;

        ret = Jobs_StartNext( buf, sizeof( buf ), name, nameLength, &outLength );
        TEST_ASSERT_EQUAL( JobsSuccess, ret );
        TEST_ASSERT_EQUAL_STRING( expected, buf );
        TEST_ASSERT_EQUAL( ( sizeof( expected ) - 1 ), outLength );

        ret = Jobs_StartNext( buf, sizeof( buf ), name, nameLength, NULL );
        TEST_ASSERT_EQUAL( JobsSuccess, ret );
        TEST_ASSERT_EQUAL_STRING( expected, buf );

        ret = Jobs_StartNext( buf, sizeof( prefix ), name, nameLength, NULL );
        TEST_ASSERT_EQUAL( JobsBufferTooSmall, ret );
        TEST_ASSERT_EQUAL_STRING( prefix, buf );
    }

    {
        char expected[] = PREFIX jobId "/" JOBS_API_DESCRIBE;

        ret = Jobs_Describe( buf, sizeof( buf ), name, nameLength, jobId, jobIdLength, &outLength );
        TEST_ASSERT_EQUAL( JobsSuccess, ret );
        TEST_ASSERT_EQUAL_STRING( expected, buf );
        TEST_ASSERT_EQUAL( ( sizeof( expected ) - 1 ), outLength );

        ret = Jobs_Describe( buf, sizeof( buf ), name, nameLength, jobId, jobIdLength, NULL );
        TEST_ASSERT_EQUAL( JobsSuccess, ret );
        TEST_ASSERT_EQUAL_STRING( expected, buf );

        ret = Jobs_Describe( buf, sizeof( prefix ), name, nameLength, jobId, jobIdLength, NULL );
        TEST_ASSERT_EQUAL( JobsBufferTooSmall, ret );
        TEST_ASSERT_EQUAL_STRING( prefix, buf );
    }

    {
        char expected[] = PREFIX jobId "/" JOBS_API_UPDATE;

        ret = Jobs_Update( buf, sizeof( buf ), name, nameLength, jobId, jobIdLength, &outLength );
        TEST_ASSERT_EQUAL( JobsSuccess, ret );
        TEST_ASSERT_EQUAL_STRING( expected, buf );
        TEST_ASSERT_EQUAL( ( sizeof( expected ) - 1 ), outLength );

        ret = Jobs_Update( buf, sizeof( buf ), name, nameLength, jobId, jobIdLength, NULL );
        TEST_ASSERT_EQUAL( JobsSuccess, ret );
        TEST_ASSERT_EQUAL_STRING( expected, buf );

        ret = Jobs_Update( buf, sizeof( prefix ), name, nameLength, jobId, jobIdLength, NULL );
        TEST_ASSERT_EQUAL( JobsBufferTooSmall, ret );
        TEST_ASSERT_EQUAL_STRING( prefix, buf );
    }
}

/**
 * @brief Test the full range of topic matching
 *
 * @note Certain invalid topics are provided solely for branch coverage.
 */
void test_Jobs_match_topic( void )
{
    char * topic;
    size_t topicLength;
    JobsTopic_t api, outApi;
    JobsStatus_t ret;

#define setVars( x, y )    do { api = ( x ); topic = ( y ); topicLength = sizeof( y ) - 1; } while( 0 )

    setVars( JobsJobsChanged, PREFIX "notify" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsNextJobChanged, PREFIX "notify-next" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsGetPendingSuccess, PREFIX "get/accepted" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsGetPendingFailed, PREFIX "get/rejected" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsStartNextSuccess, PREFIX "start-next/accepted" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsStartNextFailed, PREFIX "start-next/rejected" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsDescribeSuccess, PREFIX "12345/get/accepted" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsDescribeFailed, PREFIX "ab224-z/get/rejected" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsUpdateSuccess, PREFIX "12345/update/accepted" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsUpdateFailed, PREFIX "ab224-z/update/rejected" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsInvalidTopic, PREFIX "notifyzz" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsNoMatch, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsInvalidTopic, PREFIX "ab224-z/update/rejectedzz" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsNoMatch, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsInvalidTopic, JOBS_API_PREFIX name "zz" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsNoMatch, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsInvalidTopic, JOBS_API_PREFIX name "/jobz/" "notify" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsNoMatch, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsInvalidTopic, JOBS_API_PREFIX "FOOBAR" JOBS_API_BRIDGE "notify" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsNoMatch, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsInvalidTopic, "$awz/thingz/" name JOBS_API_BRIDGE "notify" );
    ret = Jobs_MatchTopic( topic, topicLength, name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsNoMatch, ret );
    TEST_ASSERT_EQUAL( api, outApi );

    setVars( JobsInvalidTopic, PREFIX "notify" );
    ret = Jobs_MatchTopic( topic, ( topicLength * 10 ), name, nameLength, &outApi, NULL, NULL );
    TEST_ASSERT_EQUAL( JobsNoMatch, ret );
    TEST_ASSERT_EQUAL( api, outApi );
}
