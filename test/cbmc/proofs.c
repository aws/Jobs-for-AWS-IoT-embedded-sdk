
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "jobs.h"
#include "core_json.h"
#include "jobs_annex.h"

#ifndef UNWIND_COUNT
    #define UNWIND_COUNT 10
#endif

#define CBMC_OBJECT_BITS 8
#define CBMC_MAX_OBJECT_SIZE (SIZE_MAX>>(CBMC_OBJECT_BITS+1))
#define CBMC_MAX_BUFSIZE ( UNWIND_COUNT )
#define CBMC_THINGNAME_MAX_LEN (UNWIND_COUNT -1)
#define CBMC_JOBID_MAX_LEN (UNWIND_COUNT -1)

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
    __CPROVER_assume( thingNameLength <= CBMC_THINGNAME_MAX_LEN );
    thingName = malloc( thingNameLength );

    /* The job ID length must not exceed unwindings. */
    __CPROVER_assume( jobIdLength <= CBMC_JOBID_MAX_LEN );
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
    __CPROVER_assume( thingNameLength <= CBMC_THINGNAME_MAX_LEN );
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
    JobsTopic_t api = 0;
    size_t * outLength;
    JobsStatus_t ret;

    /* The buffer length must not exceed the maximum object size supported by CBMC. */
    __CPROVER_assume( bufferLength < CBMC_MAX_OBJECT_SIZE );
    buffer = malloc( bufferLength );

    /* The thing name length must not exceed unwindings. */
    __CPROVER_assume( thingNameLength <= CBMC_THINGNAME_MAX_LEN );
    thingName = malloc( thingNameLength );

    outLength = malloc( sizeof( *outLength ) );

    __CPROVER_assume(api>=-1 && api<=10);

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
    __CPROVER_assume( topicLength < CBMC_MAX_OBJECT_SIZE );
    topic = malloc( topicLength );

    /* The thing name length must not exceed unwindings. */
    __CPROVER_assume( thingNameLength <= CBMC_THINGNAME_MAX_LEN );
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
    __CPROVER_assume( thingNameLength <= CBMC_THINGNAME_MAX_LEN );
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
    __CPROVER_assume( thingNameLength <= CBMC_THINGNAME_MAX_LEN );
    thingName = malloc( thingNameLength );

    /* The job ID length must not exceed unwindings. */
    __CPROVER_assume( jobIdLength <= CBMC_JOBID_MAX_LEN );
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

void proof_Jobs_IsStartNextAccepted( void ){
    bool ret;
    const char * topic;
    const size_t topicLength;
    const char* thingName;
    const size_t thingNameLength;

    __CPROVER_assume(topicLength < CBMC_MAX_BUFSIZE);
    topic = malloc(topicLength);

    __CPROVER_assume(thingNameLength < CBMC_THINGNAME_MAX_LEN);
    thingName = malloc(thingNameLength);


    ret = Jobs_isStartNextAccepted(topic,
                                   topicLength,
                                   thingName,
                                   thingNameLength);

    __CPROVER_assert( (ret == 0 || ret == 1), "Return value is bool");

}

void proof_Jobs_IsJobUpdateStatus( void ){
    bool ret;
    const char * topic;
    const size_t topicLength;
    const char* thingName;
    const size_t thingNameLength;
    const char * jobId;
    const size_t jobIdLength;
    JobUpdateStatus_t expectedStatus;

    __CPROVER_assume(topicLength < CBMC_MAX_BUFSIZE);
    topic = malloc(topicLength);

    __CPROVER_assume(thingNameLength < CBMC_THINGNAME_MAX_LEN);
    thingName = malloc(thingNameLength);

    __CPROVER_assume(jobIdLength < CBMC_JOBID_MAX_LEN);
    jobId = malloc(jobIdLength);

    ret = Jobs_isJobUpdateStatus(topic,
                                 topicLength,
                                 jobId,
                                 jobIdLength,
                                 expectedStatus,
                                 thingName,
                                 thingNameLength);

    __CPROVER_assert( (ret == 0 || ret == 1), "Return value is bool");
}

void proof_Jobs_getJobId( void ){
    const char * message;
    const size_t messageLength;
    char ** jobId;
    size_t ret;

    __CPROVER_assume(messageLength <= CBMC_MAX_OBJECT_SIZE);
    message = malloc(messageLength);

    ret = Jobs_getJobId(message,
                        messageLength,
                        jobId);

    __CPROVER_assert(ret == strlen(jobId), "Returned value matches JobID length.");
}

void proof_Jobs_getJobDocument( void ){
    const char * message;
    size_t messageLength;
    char ** jobdoc;
    size_t ret;

    __CPROVER_assume(messageLength <= CBMC_MAX_OBJECT_SIZE);
    message = malloc(messageLength);

    ret = Jobs_getJobDocument(message,
                              messageLength,
                              jobdoc);
    
    __CPROVER_assert(ret == strlen(jobdoc), "Returned value matches jobDoc length.");
}

void proof_Jobs_getStartNextPendingJobExecutionTopic( void ){
    const char * thingName;
    size_t thingNameLength;
    const char * buffer;
    size_t bufferLength;
    size_t ret;

    __CPROVER_assume(thingNameLength <= CBMC_THINGNAME_MAX_LEN);
    thingName = malloc(thingNameLength);

    __CPROVER_assume(bufferLength <= CBMC_MAX_OBJECT_SIZE);
    buffer = malloc(bufferLength);

    ret = Jobs_getStartNextPendingJobExecutionTopic(thingName,
                                                    thingNameLength,
                                                    buffer,
                                                    bufferLength);

    __CPROVER_assert(ret == strlen(buffer), "Returned value is equal to buffer length.");
}

void proof_Jobs_getStartNextPendingJobExecutionMsg( void ){
    const char * clientToken;
    size_t clientTokenLength;
    const char * buffer;
    size_t bufferLength;
    size_t ret;

    __CPROVER_assume( clientTokenLength <= CBMC_MAX_BUFSIZE);
    clientToken = malloc(clientTokenLength);

    __CPROVER_assume(bufferLength <= CBMC_MAX_OBJECT_SIZE);
    buffer = malloc(bufferLength);

    ret = Jobs_getStartNextPendingJobExecutionMsg(clientToken,
                                                  clientTokenLength,
                                                  buffer,
                                                  bufferLength);

    __CPROVER_assert(ret == strlen(buffer), "Returned value is equal to buffer length.");
}

void proof_Jobs_getUpdateJobExecutionTopic( void ){
    const char * thingName;
    size_t thingNameLength;
    char * jobId;
    size_t jobIdLength;
    char * buffer;
    size_t bufferLength;
    size_t ret;

    __CPROVER_assume(thingNameLength <= CBMC_THINGNAME_MAX_LEN);
    thingName = malloc(thingNameLength);

    __CPROVER_assume(jobIdLength <= CBMC_JOBID_MAX_LEN);
    jobId = malloc(jobId);

    __CPROVER_assume(bufferLength <= CBMC_MAX_OBJECT_SIZE);
    buffer = malloc(bufferLength);

    ret = Jobs_getUpdateJobExecutionTopic(thingName,
                                          thingNameLength,
                                          jobId,
                                          jobIdLength,
                                          buffer,
                                          bufferLength);

    __CPROVER_assert(ret == strlen(buffer), "Returned vallue matches buffer length");
}

void proof_Jobs_getUpdateJobExecutionMsg( void ){
    JobCurrentStatus_t status;
    char * expectedVersion;
    size_t expectedVersionLength;
    char * buffer;
    size_t bufferLength;
    size_t ret;

    __CPROVER_assume(expectedVersionLength <= CBMC_MAX_BUFSIZE);
    expectedVersion = malloc(expectedVersionLength);

    __CPROVER_assume(bufferLength <= CBMC_MAX_OBJECT_SIZE);
    buffer = malloc(bufferLength);

    ret = Jobs_getUpdateJobExecutionMsg(status,
                                        expectedVersion,
                                        expectedVersionLength,
                                        buffer,
                                        bufferLength);
    

    __CPROVER_assert(ret == strlen(buffer), "Returned value matches buffer length.");
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
    // proof_Jobs_IsStartNextAccepted();
    // proof_Jobs_IsJobUpdateStatus();
    // proof_Jobs_getJobId();
    // proof_Jobs_getJobDocument();
    // proof_Jobs_getStartNextPendingJobExecutionTopic();
    // proof_Jobs_getStartNextPendingJobExecutionMsg();
    // proof_Jobs_getUpdateJobExecutionTopic();
    // proof_Jobs_getUpdateJobExecutionMsg();
}
