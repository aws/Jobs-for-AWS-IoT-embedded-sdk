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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "unity.h"

/* Include paths for public enums, structures, and macros. */
#include "jobs.h"


/* thing name */
#define name "foobar"
#define nameLen ( sizeof( name ) - 1 )


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

#if 0
    ret = Jobs_GetTopic( buf, sizeof( buf ), name, nameLen, JobsUpdateSuccess, &outLength );
    ret = Jobs_MatchTopic( topic, sizeof( topic ), name, nameLen, &outApi, &outJobId, &outJobIdLength );
    ret = Jobs_GetPending( buf, sizeof( buf ), name, nameLen, &outLength );
    ret = Jobs_StartNext( buf, sizeof( buf ), name, nameLen, &outLength );
    ret = Jobs_Describe( buf, sizeof( buf ), name, nameLen, jobId, jobIdLength, &outLength );
    ret = Jobs_Update( buf, sizeof( buf ), name, nameLen, jobId, jobIdLength, &outLength );
#endif

/**
 * @brief Test blahblahblah
 */
void test_Jobs_bad_parameters( void )
{
    JobsStatus_t ret;
    char buf[JOBS_API_MAX_LENGTH( nameLen )];
    char jobId[] = "1234";
    JobsTopic_t outApi;
    char * outJobId;
    uint16_t outJobIdLength;
    size_t outLength;

    /* buf is NULL */
    ret = Jobs_GetTopic( NULL, sizeof( buf ), name, nameLen, JobsUpdateSuccess, &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_MatchTopic( NULL, sizeof( buf ), name, nameLen, &outApi, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_GetPending( NULL, sizeof( buf ), name, nameLen, &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_StartNext( NULL, sizeof( buf ), name, nameLen, &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_Describe( NULL, sizeof( buf ), name, nameLen, jobId, ( sizeof( jobId ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_Update( NULL, sizeof( buf ), name, nameLen, jobId, ( sizeof( jobId ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );

    /* length is 0 */
    ret = Jobs_GetTopic( buf, 0, name, nameLen, JobsUpdateSuccess, &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_MatchTopic( buf, 0, name, nameLen, &outApi, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_GetPending( buf, 0, name, nameLen, &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_StartNext( buf, 0, name, nameLen, &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_Describe( buf, 0, name, nameLen, jobId, ( sizeof( jobId ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_Update( buf, 0, name, nameLen, jobId, ( sizeof( jobId ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );

    /* bad thing name */
    ret = Jobs_GetTopic( buf, sizeof( buf ), "!", nameLen, JobsUpdateSuccess, &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_MatchTopic( buf, sizeof( buf ), "!", nameLen, &outApi, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_GetPending( buf, sizeof( buf ), "!", nameLen, &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_StartNext( buf, sizeof( buf ), "!", nameLen, &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_Describe( buf, sizeof( buf ), "!", nameLen, jobId, ( sizeof( jobId ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_Update( buf, sizeof( buf ), "!", nameLen, jobId, ( sizeof( jobId ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );

    /* bad job ID */
    ret = Jobs_Describe( buf, sizeof( buf ), name, nameLen, "!", ( sizeof( jobId ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_Update( buf, sizeof( buf ), name, nameLen, "!", ( sizeof( jobId ) - 1 ), &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );

    /* outAPI is NULL */
    ret = Jobs_MatchTopic( buf, sizeof( buf ), name, nameLen, NULL, &outJobId, &outJobIdLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );

    /* bad API value */
    ret = Jobs_GetTopic( buf, sizeof( buf ), name, nameLen, ( JobsInvalidTopic - 1 ), &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
    ret = Jobs_GetTopic( buf, sizeof( buf ), name, nameLen, JobsMaxTopic, &outLength );
    TEST_ASSERT_EQUAL( ret, JobsBadParameter );
}

/**
 * @brief Test blahblahblah
 */
void test_Jobs_buffer_lengths( void )
{
    char buf[JOBS_API_MAX_LENGTH( nameLen )];
    char expected[] = JOBS_API_PREFIX name JOBS_API_BRIDGE "+/" JOBS_API_UPDATE JOBS_API_SUCCESS;
    JobsStatus_t ret;
    size_t len;
    int i = 0;

    ret = Jobs_GetTopic(buf, sizeof( buf ), name, nameLen, JobsUpdateSuccess, NULL);
    TEST_ASSERT_EQUAL( ret, JobsSuccess );
    TEST_ASSERT_EQUAL_STRING( buf, expected );

    for( i = 1; i < sizeof( expected ); i++ )
    {
        ret = Jobs_GetTopic(buf, i, name, nameLen, JobsUpdateSuccess, &len);
        TEST_ASSERT_EQUAL( ret, JobsBufferTooSmall );
        TEST_ASSERT_EQUAL( buf[ len ], '\0' );
    }
}
