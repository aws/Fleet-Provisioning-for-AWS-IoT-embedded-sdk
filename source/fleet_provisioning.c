/*
 * AWS IoT Fleet Provisioning Client v1.0.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * @file fleet_provisioning.c
 * @brief Implementation of the AWS IoT Fleet Provisioning Client Library.
 */

/* Standard includes. */
#include <assert.h>
#include <stddef.h>
#include <string.h>

/* Fleet Provisioning API include. */
#include "fleet_provisioning.h"

/**
 * @brief Get the topic length for a given RegisterThing topic.
 *
 * @param[in] templateNameLength the length of the template name registered with
 * AWS IoT.
 * @param[in] format The RegisterThing API format to use.
 * @param[in] topic The RegisterThing API format to use.
 *
 * @return the template length for the given RegisterThing topic.
 */
static uint16_t getRegisterThingTopicLength( uint16_t templateNameLength,
                                             FleetProvisioningFormat_t format,
                                             FleetProvisioningApiTopics_t topic );

/**
 * @brief Write the given piece of the topic to the remaining buffer and advance
 * the remaining buffer pointer.
 *
 * The caller is responsible for assuring that there is enough space remaining
 * in the buffer to write the given string.
 *
 * @param[in, out] pRemainingBuffer Pointer to the remaining buffer
 * @param[in] fragment The piece of the topic string to write.
 * @param[in] length The length of @p fragment.
 */
static void writeTopicFragmentAndAdvance( char ** pRemainingBuffer,
                                          const char * fragment,
                                          uint16_t length );

/**
 * @brief Check the parameters for FleetProvisioning_GetRegisterThingTopic().
 *
 * @param[in] pTopicBuffer The buffer to write the topic string into.
 * @param[in] format The desired RegisterThing format.
 * @param[in] topic The desired RegisterThing topic.
 * @param[in] pTemplateName The name of the provisioning template configured
 *     with AWS IoT.
 * @param[in] templateNameLength The length of @p pTemplateName.
 * @param[in] pOutLength The length of the topic string written to
 * the buffer.
 *
 * @return FleetProvisioningSuccess if no errors are found with the parameters;
 * FleetProvisioningBadParameter otherwise.
 */
static FleetProvisioningStatus_t GetRegisterThingTopicCheckParams( char * pTopicBuffer,
                                                                   FleetProvisioningFormat_t format,
                                                                   FleetProvisioningApiTopics_t topic,
                                                                   const char * pTemplateName,
                                                                   uint16_t templateNameLength,
                                                                   uint16_t * pOutLength );

/*-----------------------------------------------------------*/

static uint16_t getRegisterThingTopicLength( uint16_t templateNameLength,
                                             FleetProvisioningFormat_t format,
                                             FleetProvisioningApiTopics_t topic )
{
    uint16_t topicLength = 0U;

    assert( ( templateNameLength != 0U ) &&
            ( templateNameLength <= FLEET_PROVISIONING_TEMPLATENAME_MAX_LENGTH ) );
    assert( ( format == FleetProvisioningJson ) || ( format == FleetProvisioningCbor ) );
    assert( ( topic >= FleetProvisioningPublish ) && ( topic <= FleetProvisioningRejected ) );

    topicLength = FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_PREFIX +
                  templateNameLength +
                  FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_BRIDGE;

    if( format == FleetProvisioningJson )
    {
        topicLength += FLEET_PROVISIONING_API_LENGTH_JSON_FORMAT;
    }

    if( format == FleetProvisioningCbor )
    {
        topicLength += FLEET_PROVISIONING_API_LENGTH_CBOR_FORMAT;
    }

    if( topic == FleetProvisioningAccepted )
    {
        topicLength += FLEET_PROVISIONING_API_LENGTH_ACCEPTED_SUFFIX;
    }

    if( topic == FleetProvisioningRejected )
    {
        topicLength += FLEET_PROVISIONING_API_LENGTH_REJECTED_SUFFIX;
    }

    return topicLength;
}
/*-----------------------------------------------------------*/

static void writeTopicFragmentAndAdvance( char ** pRemainingBuffer,
                                          const char * fragment,
                                          uint16_t length )
{
    assert( pRemainingBuffer != NULL );
    assert( *pRemainingBuffer != NULL );
    assert( fragment != NULL );

    ( void ) memcpy( ( void * ) *pRemainingBuffer,
                     ( const void * ) fragment,
                     ( size_t ) length );

    *pRemainingBuffer += length;
}
/*-----------------------------------------------------------*/

static FleetProvisioningStatus_t GetRegisterThingTopicCheckParams( char * pTopicBuffer,
                                                                   FleetProvisioningFormat_t format,
                                                                   FleetProvisioningApiTopics_t topic,
                                                                   const char * pTemplateName,
                                                                   uint16_t templateNameLength,
                                                                   uint16_t * pOutLength )
{
    FleetProvisioningStatus_t ret = FleetProvisioningError;

    if( ( pTopicBuffer == NULL ) ||
        ( format < FleetProvisioningJson ) ||
        ( format > FleetProvisioningCbor ) ||
        ( topic < FleetProvisioningPublish ) ||
        ( topic > FleetProvisioningRejected ) ||
        ( pTemplateName == NULL ) ||
        ( templateNameLength == 0U ) ||
        ( templateNameLength > FLEET_PROVISIONING_TEMPLATENAME_MAX_LENGTH ) ||
        ( pOutLength == NULL ) )
    {
        ret = FleetProvisioningBadParameter;

        LogError( ( "Invalid input parameter. pTopicBuffer: %p, bufferLength: %u, format: %d, "
                    "topic: %d, pTemplateName: %p, templateNameLength: %u, pOutLength: %p.",
                    ( void * ) pTopicBuffer,
                    ( unsigned int ) bufferLength,
                    ( int ) format,
                    ( int ) topic,
                    ( void * ) pTemplateName,
                    ( unsigned int ) templateNameLength,
                    ( void * ) pOutLength ) );
    }
    else
    {
        ret = FleetProvisioningSuccess;
    }

    return ret;
}
/*-----------------------------------------------------------*/

FleetProvisioningStatus_t FleetProvisioning_GetRegisterThingTopic( char * pTopicBuffer,
                                                                   uint16_t bufferLength,
                                                                   FleetProvisioningFormat_t format,
                                                                   FleetProvisioningApiTopics_t topic,
                                                                   const char * pTemplateName,
                                                                   uint16_t templateNameLength,
                                                                   uint16_t * pOutLength )
{
    FleetProvisioningStatus_t status = FleetProvisioningError;
    uint16_t topicLength = 0U;
    char * pBufferCursor = pTopicBuffer;

    status = GetRegisterThingTopicCheckParams( pTopicBuffer,
                                               format,
                                               topic,
                                               pTemplateName,
                                               templateNameLength,
                                               pOutLength );

    if( status == FleetProvisioningSuccess )
    {
        topicLength = getRegisterThingTopicLength( templateNameLength, format, topic );

        if( bufferLength < topicLength )
        {
            status = FleetProvisioningBufferTooSmall;

            LogError( ( "The buffer is too small to hold the topic string. "
                        "Provided buffer size: %u, Required buffer size: %u.",
                        ( unsigned int ) bufferLength,
                        ( unsigned int ) topicLength ) );
        }
    }

    if( status == FleetProvisioningSuccess )
    {
        /* At this point, it is certain that we have a large enough buffer to
         * write the topic string into. */

        /* Write prefix first. */
        writeTopicFragmentAndAdvance( &pBufferCursor,
                                      FLEET_PROVISIONING_REGISTER_THING_API_PREFIX,
                                      FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_PREFIX );

        /* Write template name next. */
        writeTopicFragmentAndAdvance( &pBufferCursor,
                                      pTemplateName,
                                      templateNameLength );

        /* Write bridge next. */
        writeTopicFragmentAndAdvance( &pBufferCursor,
                                      FLEET_PROVISIONING_REGISTER_THING_API_BRIDGE,
                                      FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_BRIDGE );

        /* Write report format. */
        if( format == FleetProvisioningJson )
        {
            writeTopicFragmentAndAdvance( &pBufferCursor,
                                          FLEET_PROVISIONING_API_JSON_FORMAT,
                                          FLEET_PROVISIONING_API_LENGTH_JSON_FORMAT );
        }

        if( format == FleetProvisioningCbor )
        {
            writeTopicFragmentAndAdvance( &pBufferCursor,
                                          FLEET_PROVISIONING_API_CBOR_FORMAT,
                                          FLEET_PROVISIONING_API_LENGTH_CBOR_FORMAT );
        }

        /* Write report suffix. */
        if( topic == FleetProvisioningAccepted )
        {
            writeTopicFragmentAndAdvance( &pBufferCursor,
                                          FLEET_PROVISIONING_API_ACCEPTED_SUFFIX,
                                          FLEET_PROVISIONING_API_LENGTH_ACCEPTED_SUFFIX );
        }

        if( topic == FleetProvisioningRejected )
        {
            writeTopicFragmentAndAdvance( &pBufferCursor,
                                          FLEET_PROVISIONING_API_REJECTED_SUFFIX,
                                          FLEET_PROVISIONING_API_LENGTH_REJECTED_SUFFIX );
        }

        *pOutLength = topicLength;
    }

    return status;
}
/*-----------------------------------------------------------*/
