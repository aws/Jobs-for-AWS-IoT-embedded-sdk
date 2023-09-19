/*
 * AWS IoT Jobs v1.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
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

#include <ctype.h>
#include <string.h>

#include "unity.h"
#include "catch_assert.h"

/* Include paths for public enums, structures, and macros. */
#include "jobs.h"
#include "jobs_annex.h"

/* thing name */
#define name_           "foobar"
#define nameLength_     ( sizeof( name_ ) - 1 )

/* job ID */
#define jobId_          "1234"
#define jobIdLength_    ( sizeof( jobId_ ) - 1 )

/* Common prefix of Jobs topics. */
#define PREFIX          JOBS_API_PREFIX name_ JOBS_API_BRIDGE

jmp_buf CATCH_JMPBUF;

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
 * @brief Test that the topic generating macros are valid.
 */
void test_JobsPUBLISH__API_macros_are_v( void )
{
    char * expectedTopic = NULL;

    /* Test for NextJobExecutionChanged API topic. */
    expectedTopic = PREFIX JOBS_API_NEXTJOBCHANGED;
    TEST_ASSERT_EQUAL_STRING( expectedTopic, JOBS_API_SUBSCRIBE_NEXTJOBCHANGED( name_ ) );

    /* Test for JobExecutionsChanged API topic. */
    expectedTopic = PREFIX JOBS_API_JOBSCHANGED;
    TEST_ASSERT_EQUAL_STRING( expectedTopic, JOBS_API_SUBSCRIBE_JOBSCHANGED( name_ ) );

    /* Test for StartNextPendingJobExecution API topic. */
    expectedTopic = PREFIX JOBS_API_STARTNEXT;
    TEST_ASSERT_EQUAL_STRING( expectedTopic, JOBS_API_PUBLISH_STARTNEXT( name_ ) );

    /* Test for GetPendingJobExecutions API topic. */
    expectedTopic = PREFIX JOBS_API_GETPENDING;
    TEST_ASSERT_EQUAL_STRING( expectedTopic, JOBS_API_PUBLISH_GETPENDING( name_ ) );

    /* Test for DescribeJobExecution API topic for the "$next" job ID. */
    expectedTopic = PREFIX JOBS_API_JOBID_NEXT "/" JOBS_API_DESCRIBE;
    TEST_ASSERT_EQUAL_STRING( expectedTopic, JOBS_API_PUBLISH_DESCRIBENEXTJOB( name_ ) );
}


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
    char buf[ JOBS_API_MAX_LENGTH( nameLength_ ) ];
    JobsTopic_t outApi;
    char * outJobId;
    uint16_t outJobIdLength;
    size_t outLength;

#define TEST_BAD_PARAMETER( x ) \
    TEST_ASSERT_EQUAL( JobsBadParameter, ( x ) )

    /* buf is NULL */
    TEST_BAD_PARAMETER( Jobs_GetTopic( NULL, sizeof( buf ), name_, nameLength_, JobsUpdateSuccess, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_MatchTopic( NULL, sizeof( buf ), name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_BAD_PARAMETER( Jobs_GetPending( NULL, sizeof( buf ), name_, nameLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_StartNext( NULL, sizeof( buf ), name_, nameLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Describe( NULL, sizeof( buf ), name_, nameLength_, jobId_, jobIdLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Update( NULL, sizeof( buf ), name_, nameLength_, jobId_, jobIdLength_, &outLength ) );

    /* length is 0 */
    TEST_BAD_PARAMETER( Jobs_GetTopic( buf, 0, name_, nameLength_, JobsUpdateSuccess, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_MatchTopic( buf, 0, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_BAD_PARAMETER( Jobs_GetPending( buf, 0, name_, nameLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_StartNext( buf, 0, name_, nameLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Describe( buf, 0, name_, nameLength_, jobId_, jobIdLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Update( buf, 0, name_, nameLength_, jobId_, jobIdLength_, &outLength ) );

    /* name is NULL */
    TEST_BAD_PARAMETER( Jobs_GetTopic( buf, sizeof( buf ), NULL, nameLength_, JobsUpdateSuccess, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_MatchTopic( buf, sizeof( buf ), NULL, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_BAD_PARAMETER( Jobs_GetPending( buf, sizeof( buf ), NULL, nameLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_StartNext( buf, sizeof( buf ), NULL, nameLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Describe( buf, sizeof( buf ), NULL, nameLength_, jobId_, jobIdLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Update( buf, sizeof( buf ), NULL, nameLength_, jobId_, jobIdLength_, &outLength ) );

    /* bad thing name - variations tested separately */
    TEST_BAD_PARAMETER( Jobs_GetTopic( buf, sizeof( buf ), "!", nameLength_, JobsUpdateSuccess, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_MatchTopic( buf, sizeof( buf ), "!", nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_BAD_PARAMETER( Jobs_GetPending( buf, sizeof( buf ), "!", nameLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_StartNext( buf, sizeof( buf ), "!", nameLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Describe( buf, sizeof( buf ), "!", nameLength_, jobId_, jobIdLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Update( buf, sizeof( buf ), "!", nameLength_, jobId_, jobIdLength_, &outLength ) );

    /* bad thing name length */
    TEST_BAD_PARAMETER( Jobs_GetTopic( buf, sizeof( buf ), name_, 0, JobsUpdateSuccess, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_MatchTopic( buf, sizeof( buf ), name_, 0, &outApi, &outJobId, &outJobIdLength ) );
    TEST_BAD_PARAMETER( Jobs_GetPending( buf, sizeof( buf ), name_, 0, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_StartNext( buf, sizeof( buf ), name_, 0, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Describe( buf, sizeof( buf ), name_, 0, jobId_, jobIdLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Update( buf, sizeof( buf ), name_, 0, jobId_, jobIdLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_GetTopic( buf, sizeof( buf ), name_, ( THINGNAME_MAX_LENGTH + 1 ), JobsUpdateSuccess, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_MatchTopic( buf, sizeof( buf ), name_, ( THINGNAME_MAX_LENGTH + 1 ), &outApi, &outJobId, &outJobIdLength ) );
    TEST_BAD_PARAMETER( Jobs_GetPending( buf, sizeof( buf ), name_, ( THINGNAME_MAX_LENGTH + 1 ), &outLength ) );
    TEST_BAD_PARAMETER( Jobs_StartNext( buf, sizeof( buf ), name_, ( THINGNAME_MAX_LENGTH + 1 ), &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Describe( buf, sizeof( buf ), name_, ( THINGNAME_MAX_LENGTH + 1 ), jobId_, jobIdLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Update( buf, sizeof( buf ), name_, ( THINGNAME_MAX_LENGTH + 1 ), jobId_, jobIdLength_, &outLength ) );

    /* job ID is NULL */
    TEST_BAD_PARAMETER( Jobs_Describe( buf, sizeof( buf ), name_, nameLength_, NULL, jobIdLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Update( buf, sizeof( buf ), name_, nameLength_, NULL, jobIdLength_, &outLength ) );

    /* bad job ID - variations tested separately */
    TEST_BAD_PARAMETER( Jobs_Describe( buf, sizeof( buf ), name_, nameLength_, "!", jobIdLength_, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Update( buf, sizeof( buf ), name_, nameLength_, "!", jobIdLength_, &outLength ) );

    /* bad job ID length */
    TEST_BAD_PARAMETER( Jobs_Describe( buf, sizeof( buf ), name_, nameLength_, jobId_, 0, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Update( buf, sizeof( buf ), name_, nameLength_, jobId_, 0, &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Describe( buf, sizeof( buf ), name_, nameLength_, jobId_, ( JOBID_MAX_LENGTH + 1 ), &outLength ) );
    TEST_BAD_PARAMETER( Jobs_Update( buf, sizeof( buf ), name_, nameLength_, jobId_, ( JOBID_MAX_LENGTH + 1 ), &outLength ) );

    /* outAPI is NULL */
    TEST_BAD_PARAMETER( Jobs_MatchTopic( buf, sizeof( buf ), name_, nameLength_, NULL, &outJobId, &outJobIdLength ) );

    /* bad API value */
    TEST_BAD_PARAMETER( Jobs_GetTopic( buf, sizeof( buf ), name_, nameLength_, ( JobsInvalidTopic - 1 ), &outLength ) );
    TEST_BAD_PARAMETER( Jobs_GetTopic( buf, sizeof( buf ), name_, nameLength_, JobsMaxTopic, &outLength ) );
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
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
            "abcdefghijklmnopqrstuvwxyz"

    char testChars[] = commonChars;

    ret = Jobs_GetPending( buf, sizeof( buf ), testChars, ( sizeof( testChars ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );

    ret = Jobs_Describe( buf, sizeof( buf ), name_, nameLength_, testChars, ( sizeof( testChars ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );

    /* thing name allows colon. */
    testChars[ 0 ] = ':';
    ret = Jobs_GetPending( buf, sizeof( buf ), testChars, ( sizeof( testChars ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );

    /* job ID disallows colon. */
    ret = Jobs_Describe( buf, sizeof( buf ), name_, nameLength_, testChars, ( sizeof( testChars ) - 1 ), &outLength );
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

    /* Test additional valid jobId, $next */
    ret = Jobs_Describe( buf, sizeof( buf ), name_, nameLength_, JOBS_API_JOBID_NEXT, JOBS_API_JOBID_NEXT_LENGTH, &outLength );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
}

/**
 * @brief Test many buffer sizes for correct behavior
 *
 * The string append code is shared by all functions that write a buffer.
 * This code guarantees that any non-zero length buffer will be NUL terminated.
 */
void test_Jobs_buffer_lengths( void )
{
    char buf[ JOBS_API_MAX_LENGTH( nameLength_ ) ];
    JobsStatus_t ret;
    size_t len;
    unsigned i = 0;
    char expected1[] = JOBS_API_SUBSCRIBE_JOBSCHANGED( name_ );
    char expected2[] = JOBS_API_PREFIX name_ JOBS_API_BRIDGE "+/" JOBS_API_UPDATE JOBS_API_SUCCESS;

    ret = Jobs_GetTopic( buf, sizeof( buf ), name_, nameLength_, JobsJobsChanged, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL_STRING( expected1, buf );

    ret = Jobs_GetTopic( buf, sizeof( buf ), name_, nameLength_, JobsUpdateSuccess, NULL );
    TEST_ASSERT_EQUAL( JobsSuccess, ret );
    TEST_ASSERT_EQUAL_STRING( expected2, buf );

    for( i = 1; i < sizeof( expected2 ); i++ )
    {
        ret = Jobs_GetTopic( buf, i, name_, nameLength_, JobsUpdateSuccess, &len );
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
    char buf[ JOBS_API_MAX_LENGTH( nameLength_ ) ];
    size_t outLength;

    char prefix[] = PREFIX;

#define TEST_SUCCESS( x )                          \
    do {                                           \
        TEST_ASSERT_EQUAL( JobsSuccess, ( x ) );   \
        TEST_ASSERT_EQUAL_STRING( expected, buf ); \
    } while( 0 )

#define TEST_TOO_SMALL( x )                             \
    do {                                                \
        TEST_ASSERT_EQUAL( JobsBufferTooSmall, ( x ) ); \
        TEST_ASSERT_EQUAL_STRING( prefix, buf );        \
    } while( 0 )

#define TEST_OUTLENGTH( x ) \
    TEST_ASSERT_EQUAL( ( sizeof( x ) - 1 ), outLength )

    {
        char expected[] = JOBS_API_PUBLISH_GETPENDING( name_ );

        TEST_SUCCESS( Jobs_GetPending( buf, sizeof( buf ), name_, nameLength_, NULL ) );
        TEST_SUCCESS( Jobs_GetPending( buf, sizeof( buf ), name_, nameLength_, &outLength ) );
        TEST_OUTLENGTH( expected );

        TEST_TOO_SMALL( Jobs_GetPending( buf, sizeof( prefix ), name_, nameLength_, NULL ) );
        TEST_TOO_SMALL( Jobs_GetPending( buf, sizeof( prefix ), name_, nameLength_, &outLength ) );
        TEST_OUTLENGTH( prefix );
    }

    {
        char expected[] = JOBS_API_PUBLISH_STARTNEXT( name_ );

        TEST_SUCCESS( Jobs_StartNext( buf, sizeof( buf ), name_, nameLength_, NULL ) );
        TEST_SUCCESS( Jobs_StartNext( buf, sizeof( buf ), name_, nameLength_, &outLength ) );
        TEST_OUTLENGTH( expected );

        TEST_TOO_SMALL( Jobs_StartNext( buf, sizeof( prefix ), name_, nameLength_, NULL ) );
        TEST_TOO_SMALL( Jobs_StartNext( buf, sizeof( prefix ), name_, nameLength_, &outLength ) );
        TEST_OUTLENGTH( prefix );
    }

    {
        char expected[] = PREFIX jobId_ "/" JOBS_API_DESCRIBE;

        TEST_SUCCESS( Jobs_Describe( buf, sizeof( buf ), name_, nameLength_, jobId_, jobIdLength_, NULL ) );
        TEST_SUCCESS( Jobs_Describe( buf, sizeof( buf ), name_, nameLength_, jobId_, jobIdLength_, &outLength ) );
        TEST_OUTLENGTH( expected );

        TEST_TOO_SMALL( Jobs_Describe( buf, sizeof( prefix ), name_, nameLength_, jobId_, jobIdLength_, NULL ) );
        TEST_TOO_SMALL( Jobs_Describe( buf, sizeof( prefix ), name_, nameLength_, jobId_, jobIdLength_, &outLength ) );
        TEST_OUTLENGTH( prefix );
    }

    {
        char expected[] = PREFIX jobId_ "/" JOBS_API_UPDATE;

        TEST_SUCCESS( Jobs_Update( buf, sizeof( buf ), name_, nameLength_, jobId_, jobIdLength_, NULL ) );
        TEST_SUCCESS( Jobs_Update( buf, sizeof( buf ), name_, nameLength_, jobId_, jobIdLength_, &outLength ) );
        TEST_OUTLENGTH( expected );

        TEST_TOO_SMALL( Jobs_Update( buf, sizeof( prefix ), name_, nameLength_, jobId_, jobIdLength_, NULL ) );
        TEST_TOO_SMALL( Jobs_Update( buf, sizeof( prefix ), name_, nameLength_, jobId_, jobIdLength_, &outLength ) );
        TEST_OUTLENGTH( prefix );
    }
#undef TEST_SUCCESS
}

/**
 * @brief Test the full range of topic matching
 *
 * @note Certain invalid topics are provided solely for branch coverage.
 */
void test_Jobs_match_topic( void )
{
    char * topic, * jobId;
    size_t topicLength, jobIdLength;
    JobsTopic_t api, outApi;
    char * outJobId;
    uint16_t outJobIdLength;

#define setVars( x, y, z )                                     \
    do {                                                       \
        api = ( x );                                           \
        topic = ( y );                                         \
        topicLength = sizeof( y ) - 1;                         \
        jobId = ( z );                                         \
        jobIdLength = ( ( z ) == NULL ) ? 0 : sizeof( z ) - 1; \
    } while( 0 )

#define TEST_SUCCESS( x )                        \
    do {                                         \
        TEST_ASSERT_EQUAL( JobsSuccess, ( x ) ); \
        TEST_ASSERT_EQUAL( api, outApi );        \
    } while( 0 )

#define TEST_NOMATCH( x )                        \
    do {                                         \
        TEST_ASSERT_EQUAL( JobsNoMatch, ( x ) ); \
        TEST_ASSERT_EQUAL( api, outApi );        \
    } while( 0 )

#define TEST_JOBID()                                                  \
    do {                                                              \
        TEST_ASSERT_EQUAL( jobIdLength, outJobIdLength );             \
        if( jobId == NULL ) {                                         \
            TEST_ASSERT_EQUAL( outJobId, NULL );                      \
        }                                                             \
        else                                                          \
        {                                                             \
            TEST_ASSERT_EQUAL_MEMORY( jobId, outJobId, jobIdLength ); \
        }                                                             \
    } while( 0 )

    setVars( JobsJobsChanged, PREFIX "notify", NULL );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsNextJobChanged, PREFIX "notify-next", NULL );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsGetPendingSuccess, PREFIX "get/accepted", NULL );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsGetPendingFailed, PREFIX "get/rejected", NULL );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsStartNextSuccess, PREFIX "start-next/accepted", NULL );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsStartNextFailed, PREFIX "start-next/rejected", NULL );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsDescribeSuccess, PREFIX "12345/get/accepted", "12345" );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsDescribeFailed, PREFIX "ab224-z/get/rejected", "ab224-z" );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsDescribeSuccess, PREFIX "$next/get/accepted", "$next" );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsDescribeFailed, PREFIX "$next/get/rejected", "$next" );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsUpdateSuccess, PREFIX "12345/update/accepted", "12345" );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsUpdateFailed, PREFIX "ab224-z/update/rejected", "ab224-z" );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_SUCCESS( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsInvalidTopic, PREFIX "notifyzz", NULL );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsInvalidTopic, PREFIX "ab224-z/update/rejectedzz", NULL );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsInvalidTopic, JOBS_API_PREFIX name_ "zz", NULL );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsInvalidTopic, JOBS_API_PREFIX name_ "/jobz/" "notify", NULL );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsInvalidTopic, JOBS_API_PREFIX "FOOBAR" JOBS_API_BRIDGE "notify", NULL );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsInvalidTopic, "$awz/thingz/" name_ JOBS_API_BRIDGE "notify", NULL );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_NOMATCH( Jobs_MatchTopic( topic, topicLength, name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();

    setVars( JobsInvalidTopic, PREFIX "notify", NULL );
    TEST_NOMATCH( Jobs_MatchTopic( topic, ( topicLength * 10 ), name_, nameLength_, &outApi, NULL, NULL ) );
    TEST_NOMATCH( Jobs_MatchTopic( topic, ( topicLength * 10 ), name_, nameLength_, &outApi, &outJobId, &outJobIdLength ) );
    TEST_JOBID();
}

/**
 * @brief Test asserts
 */

void test_Jobs_asserts( void )
{
    char bufA[ 1 ], bufB[ 1 ], * p;
    size_t x = 1, y = 1, z = 1;
    uint16_t j = 1;
    JobsTopic_t api;

    catch_assert( strnAppend( NULL, &x, y, bufB, z ) );
    catch_assert( strnAppend( bufA, NULL, y, bufB, z ) );
    catch_assert( strnAppend( bufA, &x, y, NULL, z ) );

    catch_assert( strnEq( NULL, bufB, x ) );
    catch_assert( strnEq( bufA, NULL, x ) );

    catch_assert( matchIdApi( NULL, x, &api, &p, &j ) );
    catch_assert( matchIdApi( bufA, x, NULL, &p, &j ) );
    catch_assert( matchIdApi( bufA, x, &api, NULL, &j ) );
    catch_assert( matchIdApi( bufA, x, &api, &p, NULL ) );

    catch_assert( matchApi( NULL, x, &api, &p, &j ) );
    catch_assert( matchApi( bufA, x, NULL, &p, &j ) );
    catch_assert( matchApi( bufA, x, &api, NULL, &j ) );
    catch_assert( matchApi( bufA, x, &api, &p, NULL ) );
}

//Tests for Jobs_isStartNextAccepted
void test_isStartNextAccepted_isStartNextMsg( void )
{
    char * thingName = "thingname";
    char topic[] = "$aws/things/thingname/jobs/start-next/accepted";
    size_t topicLength = strlen(topic);
    size_t thingNameLength = strlen(thingName);

    bool result = Jobs_isStartNextAccepted(topic, topicLength,thingName, thingNameLength);

    TEST_ASSERT_TRUE(result);

}

void test_isStartNextAccepted_isNotStartNextMsg( void )
{
    char * thingName = "thingname";
    char topic[] = "thingname/random/topic";
    size_t topicLength = strlen(topic);
    size_t thingNameLength = strlen(thingName);
    

    bool result = Jobs_isStartNextAccepted(topic, topicLength, thingName, thingNameLength);

    TEST_ASSERT_FALSE(result);
}

void test_isStartNextAccepted_isStartNextMsgForAnotherThing( void )
{
    char * thingName = "thingname";
    char topic[] = "$aws/things/differntThignName/jobs/start-next/accepted";
    size_t topicLength = strlen(topic);
    size_t thingNameLength = strlen(thingName);

    bool result = Jobs_isStartNextAccepted(topic, topicLength, thingName, thingNameLength);

    TEST_ASSERT_FALSE(result);
}

void test_isStartNextAccepted_isStartNextMsgForSameLengthThing( void )
{
    char * thingName = "thingname";
    char topic[] = "$aws/things/different/jobs/start-next/accepted";
    size_t topicLength = strlen(topic);
    size_t thingNameLength = strlen(thingName);

    bool result = Jobs_isStartNextAccepted(topic, topicLength,thingName, thingNameLength);

    TEST_ASSERT_FALSE(result);
}

void test_isStartNextAccepted_nullTopic( void )
{
    char * thingName = "thingname";
    size_t thingNameLength = strlen(thingName);

    bool result = Jobs_isStartNextAccepted(NULL, 1U, thingName, thingNameLength);

    TEST_ASSERT_FALSE(result);
}

void test_isStartNextAccepted_zeroTopicLength( void )
{
    char * thingName = "thingname";
    char topic[] = "$aws/things/differntThignName/jobs/start-next/accepted";
    size_t thingNameLength = strlen(thingName);

    bool result = Jobs_isStartNextAccepted(topic, 0U, thingName, thingNameLength);

    TEST_ASSERT_FALSE(result);
}

//Tests for Jobs_getJobID()

void test_getJobId_returnsJobId( void )
{
    char * message = "{\"execution\":{\"jobId\":\"identification\",\"jobDocument\":\"document\"}}";
    char * jobId = NULL;

    size_t result = Jobs_getJobId(message, strlen(message), &jobId);

    TEST_ASSERT_EQUAL(strlen("identification"), result);
    TEST_ASSERT_EQUAL_MEMORY("identification", jobId, result);
}

void test_getJobId_cannotFindJobId( void )
{
    char * message = "{\"execution\":{\"jobDocument\":\"document\"}}";
    char * jobId = NULL;

    size_t result = Jobs_getJobId(message, strlen(message), &jobId);

    TEST_ASSERT_EQUAL(0U, result);
    TEST_ASSERT_NULL(jobId);
}

void test_getJobId_malformedJson( void )
{
    char * message = "clearlyNotJson";
    char * jobId = NULL;

    size_t result = Jobs_getJobId(message, strlen(message), &jobId);

TEST_ASSERT_EQUAL(0U, result);
    TEST_ASSERT_NULL(jobId);
}

void test_getJobId_returnsZeroLengthJob_givenNullMessage( void )
{
    char * jobId = NULL;

    size_t result = Jobs_getJobId(NULL, 10U, &jobId);

    TEST_ASSERT_EQUAL(0U, result);
    TEST_ASSERT_NULL(jobId);

}

void test_getJobId_returnsZeroLengthJob_givenZeroMessageLength( void )
{
    char * message = "{\"execution\":{\"jobId\":\"identification\",\"jobDocument\":\"document\"}}";
    char * jobId = NULL;

    size_t result = Jobs_getJobId(message, 0U, &jobId);

    TEST_ASSERT_EQUAL(0U, result);
    TEST_ASSERT_NULL(jobId);
}

//Tests for Jobs_getJobDocument

void test_getJobDocument_returnsDoc( void )
{
    char * message = "{\"execution\":{\"jobId\":\"identification\",\"jobDocument\":\"document\"}}";
    char * jobDocument = NULL;

    size_t result = Jobs_getJobDocument(message, strlen(message), &jobDocument);

    TEST_ASSERT_EQUAL(strlen("document"), result);
    TEST_ASSERT_EQUAL_MEMORY("document", jobDocument, result);
}

void test_getJobDocument_cannotFindDoc( void )
{
    char * message = "{\"execution\":{\"jobId\":\"identification\"}}";
    char * jobDocument = NULL;

    size_t result = Jobs_getJobDocument(message, strlen(message), &jobDocument);

    TEST_ASSERT_EQUAL(0U, result);
    TEST_ASSERT_NULL(jobDocument);
}

void test_getJobDocument_malformedJson( void )
{
    char * message = "clearlyNotJson";
    char * jobDocument = NULL;

    size_t result = Jobs_getJobDocument(message, strlen(message), &jobDocument);

    TEST_ASSERT_EQUAL(0U, result);
    TEST_ASSERT_NULL(jobDocument);
}

void test_getJobDocument_returnsZeroLengthJob_givenNullMessage( void )
{
    char * jobDocument = NULL;

    size_t result = Jobs_getJobDocument(NULL, 10U, &jobDocument);

    TEST_ASSERT_EQUAL(0U, result);
    TEST_ASSERT_NULL(jobDocument);
}

void test_getJobDocument_returnsZeroLengthJob_givenZeroMessageLength( void )
{
    char * message = "{\"execution\":{\"jobId\":\"identification\",\"jobDocument\":\"document\"}}";
    char * jobDocument = NULL;

    size_t result = Jobs_getJobDocument(message, 0U, &jobDocument);

    TEST_ASSERT_EQUAL(0U, result);
    TEST_ASSERT_NULL(jobDocument);
}

//Tests for Jobs_isJobUpdateStatus

void test_isJobUpdateStatus_isUpdateAcceptedMsg()
{
    char * thingName = "thingname";
    size_t thingNameLength = strlen(thingName);
    char * jobId = "job-id";
    size_t jobIdLength = strlen("job-id");
    char topic[] = "$aws/things/thingname/jobs/job-id/update/accepted";
    size_t topicLength = strlen(topic);

    bool result = Jobs_isJobUpdateStatus(topic, topicLength, jobId, jobIdLength, JobUpdateStatus_Accepted, thingName, thingNameLength);

    TEST_ASSERT_TRUE(result);
}

void test_isJobUpdateStatus_isUpdateRejectedMsg()
{
    char * thingName = "thingname";
    size_t thingNameLength = strlen(thingName);
    char * jobId = "job-id";
    size_t jobIdLength = strlen("job-id");
    char topic[] = "$aws/things/thingname/jobs/job-id/update/rejected";
    size_t topicLength = strlen(topic);

    bool result = Jobs_isJobUpdateStatus(topic, topicLength, jobId, jobIdLength, JobUpdateStatus_Rejected, thingName, thingNameLength);

    TEST_ASSERT_TRUE(result);
}

void test_isJobUpdateStatus_isUpdateMsg_notForCurrentJob()
{
    char * thingName = "thingname";
    size_t thingNameLength = strlen(thingName);
    char * jobId = "job-id";
    size_t jobIdLength = strlen("job-id");
    /* Note: topic length remains the same length */
    char topic[100] = "$aws/things/thingname/jobs/jobtwo/update/accepted";
    size_t topicLength = strlen(topic);

    bool result = Jobs_isJobUpdateStatus(topic, topicLength, jobId, jobIdLength, JobUpdateStatus_Accepted, thingName, thingNameLength);

    TEST_ASSERT_FALSE(result);

    strcpy(topic, "$aws/things/thingname/jobs/different-length/update/accepted");
    topicLength = strlen(topic);

    result = Jobs_isJobUpdateStatus(topic, topicLength, jobId, jobIdLength, JobUpdateStatus_Accepted, thingName, thingNameLength);

    TEST_ASSERT_FALSE(result);
}

void test_isJobUpdateStatus_isNotUpdateAcceptedMsg()
{
    char * thingName = "thingname";
    size_t thingNameLength = strlen(thingName);
    char * jobId = "job-id";
    size_t jobIdLength = strlen("job-id");
    char topic[] = "$aws/things/thingname/jobs/some-other-topic";
    size_t topicLength = strlen(topic);

    bool result = Jobs_isJobUpdateStatus(topic, topicLength, jobId, jobIdLength, JobUpdateStatus_Accepted, thingName, thingNameLength);

    TEST_ASSERT_FALSE(result);
}

void test_isJobUpdateStatus_hasNullTopic()
{
    char * thingName = "thingname";
    size_t thingNameLength = strlen(thingName);
    char * jobId = "job-id";
    size_t jobIdLength = strlen("job-id");

    bool result = Jobs_isJobUpdateStatus(NULL, 1U, jobId, jobIdLength, JobUpdateStatus_Accepted, thingName, thingNameLength);

    TEST_ASSERT_FALSE(result);
}

void test_isJobUpdateStatus_hasZeroTopicLength()
{
    char * thingName = "thingname";
    size_t thingNameLength = strlen(thingName);
    char * jobId = "job-id";
    size_t jobIdLength = strlen("job-id");

    bool result = Jobs_isJobUpdateStatus("$aws/things/thingname/jobs/start-next/accepted", 0U, jobId, jobIdLength, JobUpdateStatus_Accepted, thingName, thingNameLength);

    TEST_ASSERT_FALSE(result);
    
}
