
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "jobs.h"
#include "core_json.h"
#include "jobs_annex.h"

#ifndef __CPROVER__
bool __CPROVER_assume( bool );
bool __CPROVER_r_ok( const void *, ... );
bool __CPROVER_rw_ok( const void *, ... );
#endif

#ifndef UNWIND_COUNT
    #define UNWIND_COUNT 20
#endif

#define CBMC_OBJECT_BITS 8
#define CBMC_MAX_OBJECT_SIZE (SIZE_MAX>>(CBMC_OBJECT_BITS+1))
#define CBMC_MAX_BUFSIZE 50

void proof_strnEq( void ){
    char * bufferA, * bufferB;
    size_t length;
    JobsStatus_t ret;

    /* length is the buffer length which must not exceed unwindings. */
    __CPROVER_assume( length < CBMC_MAX_BUFSIZE );

    /* bufferA must not be NULL. */
    bufferA = malloc( length );
    __CPROVER_assume( bufferA != NULL );

    /* bufferB must not be NULL. */
    bufferB = malloc( length );
    __CPROVER_assume( bufferB != NULL );

    ret = strnEq( bufferA,
                  bufferB,
                  length );

    __CPROVER_assert( strnEqEnum( ret ), "The return value is a subset of JobsStatus_t." );
}

void proof_strnAppend( void ){
    char * dest, * src;
    size_t start, max, length;
    JobsStatus_t ret;

    /* max is the destination buffer length which must not exceed unwindings. */
    __CPROVER_assume( max < CBMC_MAX_BUFSIZE );

    /* destination buffer must not be NULL. */
    dest = malloc( max );
    __CPROVER_assume( dest != NULL );

    /* length is the source buffer length which must not exceed unwindings. */
    __CPROVER_assume( length < CBMC_MAX_BUFSIZE );

    /* source buffer must not be NULL. */
    src = malloc( length );
    __CPROVER_assume( src != NULL );

    ret = strnAppend( dest,
                      &start,
                      max,
                      src,
                      length );

    __CPROVER_assert( strnAppendEnum( ret ), "The return value is a subset of JobsStatus_t." );

    if( ret == JobsSuccess )
    {
        __CPROVER_assert( start < max,
                          "The buffer start index is less than the buffer length." );
    }
}

void proof_Jobs_Describe( void )
{
    char * buffer;
    size_t bufferLength;
    const char * thingName;
    uint16_t thingNameLength;
    const char * jobId;
    uint16_t jobIdLength;
    size_t * outLength;
    JobsStatus_t ret;

    /* The buffer length must not exceed the maximum object size supported by CBMC. */
    __CPROVER_assume( bufferLength < CBMC_MAX_OBJECT_SIZE );
    buffer = malloc( bufferLength );

    /* The thing name length must not exceed unwindings. */
    __CPROVER_assume( thingNameLength <= THINGNAME_MAX_LENGTH );
    thingName = malloc( thingNameLength );

    /* The job ID length must not exceed unwindings. */
    __CPROVER_assume( jobIdLength <= JOBID_MAX_LENGTH );
    jobId = malloc( jobIdLength );

    outLength = malloc( sizeof( *outLength ) );

    ret = Jobs_Describe( buffer,
                         bufferLength,
                         thingName,
                         thingNameLength,
                         jobId,
                         jobIdLength,
                         outLength );

    __CPROVER_assert( jobsDescribeEnum( ret ), "The return value is a subset of JobsStatus_t." );

    if( ( ret != JobsBadParameter ) && ( outLength != NULL ) )
    {
        __CPROVER_assert( ( *outLength < bufferLength ), "Buffer writes do not exceed buffer length." );

        __CPROVER_assert( ( buffer[ *outLength ] == '\0' ), "Buffer is NULL terminated." );
    }   
}

void proof_Jobs_GetPending( void ){
    char * buffer;
    size_t bufferLength;
    const char * thingName;
    uint16_t thingNameLength;
    size_t * outLength;
    JobsStatus_t ret;

    /* The buffer length must not exceed the maximum object size supported by CBMC. */
    __CPROVER_assume( bufferLength < CBMC_MAX_OBJECT_SIZE );
    buffer = malloc( bufferLength );

    /* The thing name length must not exceed unwindings. */
    __CPROVER_assume( thingNameLength <= THINGNAME_MAX_LENGTH );
    thingName = malloc( thingNameLength );

    outLength = malloc( sizeof( *outLength ) );

    ret = Jobs_GetPending( buffer,
                           bufferLength,
                           thingName,
                           thingNameLength,
                           outLength );

    __CPROVER_assert( jobsGetPendingEnum( ret ), "The return value is a subset of JobsStatus_t." );

    if( ( ret != JobsBadParameter ) && ( outLength != NULL ) )
    {
        __CPROVER_assert( ( *outLength < bufferLength ), "Buffer writes do not exceed buffer length." );

        __CPROVER_assert( ( buffer[ *outLength ] == '\0' ), "Buffer is NUL terminated." );
    }
}

void proof_Jobs_GetTopic( void )
{
    char * buffer;
    size_t bufferLength;
    const char * thingName;
    uint16_t thingNameLength;
    JobsTopic_t api;
    size_t * outLength;
    JobsStatus_t ret;

    /* The buffer length must not exceed the maximum object size supported by CBMC. */
    __CPROVER_assume( bufferLength < CBMC_MAX_OBJECT_SIZE );
    buffer = malloc( bufferLength );

    /* The thing name length must not exceed unwindings. */
    __CPROVER_assume( thingNameLength <= THINGNAME_MAX_LENGTH );
    thingName = malloc( thingNameLength );

    outLength = malloc( sizeof( *outLength ) );

    ret = Jobs_GetTopic( buffer,
                         bufferLength,
                         thingName,
                         thingNameLength,
                         api,
                         outLength );

    __CPROVER_assert( jobsGetTopicEnum( ret ), "The return value is a subset of JobsStatus_t." );

    if( ( ret != JobsBadParameter ) && ( outLength != NULL ) )
    {
        __CPROVER_assert( ( *outLength < bufferLength ), "Buffer writes do not exceed buffer length." );

        __CPROVER_assert( ( buffer[ *outLength ] == '\0' ), "Buffer is NUL terminated." );
    }
}

void proof_Jobs_MatchTopic( void )
{
    char * topic;
    size_t topicLength;
    const char * thingName;
    uint16_t thingNameLength;
    JobsTopic_t * outApi;
    char ** outJobId;
    uint16_t * outJobIdLength;
    JobsStatus_t ret;

    /* The buffer length must not exceed the maximum object size supported by CBMC. */
    __CPROVER_assume( topicLength < CBMC_MAX_BUFSIZE );
    topic = malloc( topicLength );

    /* The thing name length must not exceed unwindings. */
    __CPROVER_assume( thingNameLength <= THINGNAME_MAX_LENGTH );
    thingName = malloc( thingNameLength );

    outApi = malloc( sizeof( *outApi ) );
    outJobId = malloc( sizeof( *outJobId ) );
    outJobIdLength = malloc( sizeof( *outJobIdLength ) );

    ret = Jobs_MatchTopic( topic,
                           topicLength,
                           thingName,
                           thingNameLength,
                           outApi,
                           outJobId,
                           outJobIdLength );

    __CPROVER_assert( jobsMatchTopicEnum( ret ), "The return value is a subset of JobsStatus_t." );

    if( ret == JobsSuccess )
    {
        if( outApi != NULL )
        {
            __CPROVER_assert( jobsTopicEnum( *outApi ), "The API value is a JobsTopic_t enum." );
        }

        if( ( outJobId != NULL ) && ( *outJobId != NULL ) )
        {
            __CPROVER_assert( ( ( *outJobId > topic ) && ( *outJobId < ( topic + topicLength ) ) ),
                              "The output parameter for jobId points within the topic string." );
        }

        if( ( outJobIdLength != NULL ) && ( *outJobIdLength > 0 ) )
        {
            __CPROVER_assert( ( *outJobIdLength < topicLength ),
                              "The length of the jobId part of the topic is less than the length of the topic." );
        }
    }
}

void proof_Jobs_StartNext( void ) 
{
    char * buffer;
    size_t bufferLength;
    const char * thingName;
    uint16_t thingNameLength;
    size_t * outLength;
    JobsStatus_t ret;

    /* The buffer length must not exceed the maximum object size supported by CBMC. */
    __CPROVER_assume( bufferLength < CBMC_MAX_OBJECT_SIZE );
    buffer = malloc( bufferLength );

    /* The thing name length must not exceed unwindings. */
    __CPROVER_assume( thingNameLength <= THINGNAME_MAX_LENGTH );
    thingName = malloc( thingNameLength );

    outLength = malloc( sizeof( *outLength ) );

    ret = Jobs_StartNext( buffer,
                          bufferLength,
                          thingName,
                          thingNameLength,
                          outLength );

    __CPROVER_assert( jobsStartNextEnum( ret ), "The return value is a subset of JobsStatus_t." );

    if( ( ret != JobsBadParameter ) && ( outLength != NULL ) )
    {
        __CPROVER_assert( ( *outLength < bufferLength ), "Buffer writes do not exceed buffer length." );

        __CPROVER_assert( ( buffer[ *outLength ] == '\0' ), "Buffer is NULL terminated." );
    }
}

void proof_Jobs_Update( void ){
    char * buffer;
    size_t bufferLength;
    const char * thingName;
    uint16_t thingNameLength;
    const char * jobId;
    uint16_t jobIdLength;
    size_t * outLength;
    JobsStatus_t ret;

    /* The buffer length must not exceed the maximum object size supported by CBMC. */
    __CPROVER_assume( bufferLength < CBMC_MAX_OBJECT_SIZE );
    buffer = malloc( bufferLength );

    /* The thing name length must not exceed unwindings. */
    __CPROVER_assume( thingNameLength <= THINGNAME_MAX_LENGTH );
    thingName = malloc( thingNameLength );

    /* The job ID length must not exceed unwindings. */
    __CPROVER_assume( jobIdLength <= JOBID_MAX_LENGTH );
    jobId = malloc( jobIdLength );

    outLength = malloc( sizeof( *outLength ) );

    ret = Jobs_Update( buffer,
                       bufferLength,
                       thingName,
                       thingNameLength,
                       jobId,
                       jobIdLength,
                       outLength );

    __CPROVER_assert( jobsUpdateEnum( ret ), "The return value is a subset of JobsStatus_t." );

    if( ( ret != JobsBadParameter ) && ( outLength != NULL ) )
    {
        __CPROVER_assert( ( *outLength < bufferLength ), "Buffer writes do not exceed buffer length." );

        __CPROVER_assert( ( buffer[ *outLength ] == '\0' ), "Buffer is NUL terminated." );
    }
}

int main()
{
    proof_strnAppend();
    proof_strnEq();
    proof_Jobs_Describe();
    proof_Jobs_GetPending();
    proof_Jobs_GetTopic();
    proof_Jobs_MatchTopic();
    proof_Jobs_StartNext();
    proof_Jobs_Update();
}
