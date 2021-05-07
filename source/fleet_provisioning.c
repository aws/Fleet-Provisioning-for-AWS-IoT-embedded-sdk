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
 * @brief Identifier for which of the topic suffixes for a given format and
 * Fleet Provisioning MQTT API.
 */
typedef enum TopicSuffix
{
    TopicPublish,
    TopicAccepted,
    TopicRejected,
    TopicInvalidSuffix
} TopicSuffix_t;

/**
 * @brief Identifier for which of the topics in each Fleet Provisioning MQTT API.
 */
typedef enum TopicFormatSuffix
{
    TopicJsonPublish,
    TopicJsonAccepted,
    TopicJsonRejected,
    TopicCborPublish,
    TopicCborAccepted,
    TopicCborRejected,
    TopicInvalidFormatSuffix
} TopicFormatSuffix_t;

/**
 * @brief Match the suffix from the remaining topic string and return the
 * corresponding suffix.
 *
 * Suffix: empty, /accepted, or /rejected.
 *
 * @param[in] pRemainingTopic The remaining portion of the topic.
 * @param[in] remainingLength The remaining length of the topic.
 *
 * @return The matching #TopicSuffix_t.
 */
static TopicSuffix_t parseTopicSuffix( const char * pRemainingTopic,
                                       uint16_t remainingLength );

/**
 * @brief Match the format and suffix from the remaining topic string and
 * return the corresponding format and suffix.
 *
 * Format: json or cbor.
 * Suffix: empty, /accepted, or /rejected.
 *
 * @param[in] pRemainingTopic The remaining portion of the topic.
 * @param[in] remainingLength The remaining length of the topic.
 *
 * @return The matching #TopicFormatSuffix_t.
 */
static TopicFormatSuffix_t parseTopicFormatSuffix( const char * pRemainingTopic,
                                                   uint16_t remainingLength );

/**
 * @brief Match a topic string with the CreateCertificateFromCsr topics.
 *
 * @param[in] pTopic The topic string to match.
 * @param[in] topicLength The length of the topic string.
 *
 * @return The matching #FleetProvisioningTopic_t if the topic string is a
 *     Fleet Provisioning CreateCertificateFromCsr topic, else
 *     FleetProvisioningInvalidTopic.
 */
static FleetProvisioningTopic_t parseCreateCertificateFromCsrTopic( const char * pTopic,
                                                                    uint16_t topicLength );

/**
 * @brief Match a topic string with the CreateKeysAndCertificate topics.
 *
 * @param[in] pTopic The topic string to match.
 * @param[in] topicLength The length of the topic string.
 *
 * @return The matching FleetProvisioningTopic_t if the topic string is a
 *     Fleet Provisioning CreateKeysAndCertificate topic, else
 *     FleetProvisioningInvalidTopic.
 */
static FleetProvisioningTopic_t parseCreateKeysAndCertificateTopic( const char * pTopic,
                                                                    uint16_t topicLength );

/**
 * @brief Match a topic string with the RegisterThing topics.
 *
 * @param[in] pTopic The topic string to match.
 * @param[in] topicLength The length of the topic string.
 *
 * @return The matching #FleetProvisioningTopic_t if the topic string is a
 *     Fleet Provisioning RegisterThing topic, else
 *     FleetProvisioningInvalidTopic.
 */
static FleetProvisioningTopic_t parseRegisterThingTopic( const char * pTopic,
                                                         uint16_t topicLength );

/**
 * @brief Check if the remaining buffer starts with a specified string. If so,
 * moves the remaining buffer pointer past the matched section and updates the
 * remaining length.
 *
 * @param[in, out] pBufferCursor Pointer to the remaining portion of the buffer.
 * @param[in, out] pRemainingLength The remaining length of the buffer.
 * @param[in] matchString The string to match against.
 * @param[in] matchLength The length of @p matchString.
 *
 * @return FleetProvisioningSuccess if the string matches and is skipped over;
 * FleetProvisioningNoMatch otherwise.
 */
static FleetProvisioningStatus_t consumeIfMatch( const char ** pBufferCursor,
                                                 uint16_t * pRemainingLength,
                                                 const char * matchString,
                                                 uint16_t matchLength );

/**
 * @brief Move the remaining topic pointer past the template name in the
 * unparsed topic so far, and update the remaining topic length.
 *
 * The end of thing name is marked by a forward slash. A zero length thing name
 * is not valid.
 *
 * This function extracts the same template name from the following topic strings:
 *   - $aws/provisioning-templates/TEMPLATE_NAME/provision/json/accepted
 *   - $aws/provisioning-templates/TEMPLATE_NAME
 * The second topic is not a valid Fleet Provisioning topic and the matching
 * will fail when we try to match the bridge part.
 *
 * @param[in, out] pTopicCursor Pointer to the remaining topic string.
 * @param[in, out] remainingLength Pointer to the length of the remaining topic string.
 *
 * @return FleetProvisioningSuccess if a valid template name is skipped over;
 * FleetProvisioningNoMatch otherwise.
 */
static FleetProvisioningStatus_t consumeTemplateName( const char ** pTopicCursor,
                                                      uint16_t * remainingLength );
/*-----------------------------------------------------------*/

static TopicSuffix_t parseTopicSuffix( const char * pRemainingTopic,
                                       uint16_t remainingLength )
{
    TopicSuffix_t ret = TopicInvalidSuffix;
    FleetProvisioningStatus_t status = FleetProvisioningNoMatch;
    const char * pTopicCursor = pRemainingTopic;
    uint16_t cursorLength = remainingLength;

    assert( pRemainingTopic != NULL );

    /* Check if publish topic */
    if( cursorLength == 0 )
    {
        ret = TopicPublish;
        status = FleetProvisioningSuccess;
    }

    /* Check if accepted topic */
    if( status == FleetProvisioningNoMatch )
    {
        status = consumeIfMatch( &pTopicCursor,
                                 &cursorLength,
                                 FLEET_PROVISIONING_API_ACCEPTED_SUFFIX,
                                 FLEET_PROVISIONING_API_LENGTH_ACCEPTED_SUFFIX );

        if( status == FleetProvisioningSuccess )
        {
            if( cursorLength == 0 )
            {
                ret = TopicAccepted;
            }
            else
            {
                status = FleetProvisioningError;
            }
        }
    }

    /* Check if rejected topic */
    if( status == FleetProvisioningNoMatch )
    {
        status = consumeIfMatch( &pTopicCursor,
                                 &cursorLength,
                                 FLEET_PROVISIONING_API_REJECTED_SUFFIX,
                                 FLEET_PROVISIONING_API_LENGTH_REJECTED_SUFFIX );

        if( status == FleetProvisioningSuccess )
        {
            if( cursorLength == 0 )
            {
                ret = TopicRejected;
            }
            else
            {
                status = FleetProvisioningError;
            }
        }
    }

    return ret;
}
/*-----------------------------------------------------------*/

static TopicFormatSuffix_t parseTopicFormatSuffix( const char * pRemainingTopic,
                                                   uint16_t remainingLength )
{
    /* Table of JSON format and suffixes in same order as TopicSuffix_t. */
    static const TopicFormatSuffix_t jsonSuffixes[] =
    {
        TopicJsonPublish,
        TopicJsonAccepted,
        TopicJsonRejected,
        TopicInvalidFormatSuffix
    };
    /* Table of CBOR format and suffixes in same order as TopicSuffix_t. */
    static const TopicFormatSuffix_t cborSuffixes[] =
    {
        TopicCborPublish,
        TopicCborAccepted,
        TopicCborRejected,
        TopicInvalidFormatSuffix
    };
    TopicFormatSuffix_t ret = TopicInvalidFormatSuffix;
    TopicSuffix_t suffix = TopicInvalidSuffix;
    FleetProvisioningStatus_t status = FleetProvisioningNoMatch;
    const char * pTopicCursor = pRemainingTopic;
    uint16_t cursorLength = remainingLength;

    assert( pRemainingTopic != NULL );

    /* Check if JSON format */
    status = consumeIfMatch( &pTopicCursor,
                             &cursorLength,
                             FLEET_PROVISIONING_API_JSON_FORMAT,
                             FLEET_PROVISIONING_API_LENGTH_JSON_FORMAT );

    if( status == FleetProvisioningSuccess )
    {
        /* Match suffix */
        suffix = parseTopicSuffix( pTopicCursor, cursorLength );
        ret = jsonSuffixes[ suffix ];
    }

    if( status == FleetProvisioningNoMatch )
    {
        /* Check if CBOR format */
        status = consumeIfMatch( &pTopicCursor,
                                 &cursorLength,
                                 FLEET_PROVISIONING_API_CBOR_FORMAT,
                                 FLEET_PROVISIONING_API_LENGTH_CBOR_FORMAT );

        if( status == FleetProvisioningSuccess )
        {
            /* Match suffix */
            suffix = parseTopicSuffix( pTopicCursor, cursorLength );
            ret = cborSuffixes[ suffix ];
        }
    }

    return ret;
}
/*-----------------------------------------------------------*/

static FleetProvisioningTopic_t parseCreateCertificateFromCsrTopic( const char * pTopic,
                                                                    uint16_t topicLength )
{
    /* Table of topics in the same order as TopicFormatSuffix_t. */
    static const FleetProvisioningTopic_t createCertificateFromCsrApi[] =
    {
        FleetProvisioningJsonCreateCertificateFromCsrPublish,
        FleetProvisioningJsonCreateCertificateFromCsrAccepted,
        FleetProvisioningJsonCreateCertificateFromCsrRejected,
        FleetProvisioningCborCreateCertificateFromCsrPublish,
        FleetProvisioningCborCreateCertificateFromCsrAccepted,
        FleetProvisioningCborCreateCertificateFromCsrRejected,
        FleetProvisioningInvalidTopic
    };
    FleetProvisioningTopic_t ret = FleetProvisioningInvalidTopic;
    FleetProvisioningStatus_t status = FleetProvisioningError;
    TopicFormatSuffix_t rest = TopicInvalidFormatSuffix;
    const char * pTopicCursor = pTopic;
    uint16_t cursorLength = topicLength;

    assert( pTopic != NULL );

    /* Check if prefix matches */
    status = consumeIfMatch( &pTopicCursor,
                             &cursorLength,
                             FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_PREFIX,
                             FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_LENGTH_PREFIX );

    if( status == FleetProvisioningSuccess )
    {
        /* Match format and suffix */
        rest = parseTopicFormatSuffix( pTopicCursor, cursorLength );
        ret = createCertificateFromCsrApi[ rest ];
    }

    return ret;
}
/*-----------------------------------------------------------*/

static FleetProvisioningTopic_t parseCreateKeysAndCertificateTopic( const char * pTopic,
                                                                    uint16_t topicLength )
{
    /* Table of topics in the same order as TopicFormatSuffix_t. */
    static const FleetProvisioningTopic_t createKeysAndCertificateApi[] =
    {
        FleetProvisioningJsonCreateKeysAndCertificatePublish,
        FleetProvisioningJsonCreateKeysAndCertificateAccepted,
        FleetProvisioningJsonCreateKeysAndCertificateRejected,
        FleetProvisioningCborCreateKeysAndCertificatePublish,
        FleetProvisioningCborCreateKeysAndCertificateAccepted,
        FleetProvisioningCborCreateKeysAndCertificateRejected,
        FleetProvisioningInvalidTopic
    };
    FleetProvisioningTopic_t ret = FleetProvisioningInvalidTopic;
    FleetProvisioningStatus_t status = FleetProvisioningError;
    TopicFormatSuffix_t rest = TopicInvalidFormatSuffix;
    const char * pTopicCursor = pTopic;
    uint16_t cursorLength = topicLength;

    assert( pTopic != NULL );

    /* Check if prefix matches */
    status = consumeIfMatch( &pTopicCursor,
                             &cursorLength,
                             FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_PREFIX,
                             FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_LENGTH_PREFIX );

    if( status == FleetProvisioningSuccess )
    {
        /* Match format and suffix */
        rest = parseTopicFormatSuffix( pTopicCursor, cursorLength );
        ret = createKeysAndCertificateApi[ rest ];
    }

    return ret;
}
/*-----------------------------------------------------------*/

static FleetProvisioningTopic_t parseRegisterThingTopic( const char * pTopic,
                                                         uint16_t topicLength )
{
    /* Table of topics in the same order as TopicFormatSuffix_t. */
    static const FleetProvisioningTopic_t registerThingApi[] =
    {
        FleetProvisioningJsonRegisterThingPublish,
        FleetProvisioningJsonRegisterThingAccepted,
        FleetProvisioningJsonRegisterThingRejected,
        FleetProvisioningCborRegisterThingPublish,
        FleetProvisioningCborRegisterThingAccepted,
        FleetProvisioningCborRegisterThingRejected,
        FleetProvisioningInvalidTopic
    };
    FleetProvisioningTopic_t ret = FleetProvisioningInvalidTopic;
    FleetProvisioningStatus_t status = FleetProvisioningError;
    TopicFormatSuffix_t rest = TopicInvalidFormatSuffix;
    const char * pTopicCursor = pTopic;
    uint16_t cursorLength = topicLength;

    assert( pTopic != NULL );

    /* Check if prefix matches */
    status = consumeIfMatch( &pTopicCursor,
                             &cursorLength,
                             FLEET_PROVISIONING_REGISTER_THING_API_PREFIX,
                             FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_PREFIX );

    if( status == FleetProvisioningSuccess )
    {
        /* Skip template name */
        status = consumeTemplateName( &pTopicCursor,
                                      &cursorLength );
    }

    if( status == FleetProvisioningSuccess )
    {
        /* Check if bridge matches */
        status = consumeIfMatch( &pTopicCursor,
                                 &cursorLength,
                                 FLEET_PROVISIONING_REGISTER_THING_API_BRIDGE,
                                 FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_BRIDGE );
    }

    if( status == FleetProvisioningSuccess )
    {
        /* Match format and suffix */
        rest = parseTopicFormatSuffix( pTopicCursor, cursorLength );
        ret = registerThingApi[ rest ];
    }

    return ret;
}
/*-----------------------------------------------------------*/

static FleetProvisioningStatus_t consumeIfMatch( const char ** pBufferCursor,
                                                 uint16_t * pRemainingLength,
                                                 const char * matchString,
                                                 uint16_t matchLength )
{
    FleetProvisioningStatus_t status = FleetProvisioningError;
    int cmpVal = -1;

    assert( pBufferCursor != NULL );
    assert( *pBufferCursor != NULL );
    assert( pRemainingLength != NULL );
    assert( matchString != NULL );

    if( *pRemainingLength < matchLength )
    {
        status = FleetProvisioningNoMatch;
    }
    else
    {
        cmpVal = strncmp( *pBufferCursor,
                          matchString,
                          ( size_t ) matchLength );

        if( cmpVal != 0 )
        {
            status = FleetProvisioningNoMatch;
        }
        else
        {
            status = FleetProvisioningSuccess;
            *pBufferCursor += matchLength;
            *pRemainingLength -= matchLength;
        }
    }

    return status;
}
/*-----------------------------------------------------------*/

static FleetProvisioningStatus_t consumeTemplateName( const char ** pTopicCursor,
                                                      uint16_t * pRemainingLength )
{
    FleetProvisioningStatus_t ret = FleetProvisioningNoMatch;
    uint16_t i = 0U;

    assert( pTopicCursor != NULL );
    assert( *pTopicCursor != NULL );
    assert( pRemainingLength != NULL );

    /* Find the first forward slash. It marks the end of the template name. */
    for( i = 0U; i < *pRemainingLength; i++ )
    {
        if( ( *pTopicCursor )[ i ] == '/' )
        {
            break;
        }
    }

    /* Zero length template name is not valid. */
    if( i > 0U )
    {
        ret = FleetProvisioningSuccess;
        *pTopicCursor += i;
        *pRemainingLength -= i;
    }

    return ret;
}
/*-----------------------------------------------------------*/

FleetProvisioningStatus_t FleetProvisioning_MatchTopic( const char * pTopic,
                                                        uint16_t topicLength,
                                                        FleetProvisioningTopic_t * pOutApi )
{
    FleetProvisioningStatus_t ret = FleetProvisioningNoMatch;

    if( ( pTopic == NULL ) || ( pOutApi == NULL ) )
    {
        ret = FleetProvisioningBadParameter;
        LogError( ( "Invalid input parameter. pTopic: %p, pOutApi: %p.",
                    ( void * ) pTopic,
                    ( void * ) pOutApi ) );
    }
    else
    {
        *pOutApi = parseCreateCertificateFromCsrTopic( pTopic, topicLength );

        if( *pOutApi == FleetProvisioningInvalidTopic )
        {
            *pOutApi = parseCreateKeysAndCertificateTopic( pTopic, topicLength );
        }

        if( *pOutApi == FleetProvisioningInvalidTopic )
        {
            *pOutApi = parseRegisterThingTopic( pTopic, topicLength );
        }

        if( *pOutApi != FleetProvisioningInvalidTopic )
        {
            ret = FleetProvisioningSuccess;
        }
    }

    return ret;
}
/*-----------------------------------------------------------*/
