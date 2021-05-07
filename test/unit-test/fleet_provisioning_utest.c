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
 * @file fleet_provisioning_utest.c
 * @brief Unit tests for the Fleet Provisioning library.
 */

/* Standard includes. */
#include <string.h>

/* Test framework include. */
#include "unity.h"

/* Fleet Provisioning API include. */
#include "fleet_provisioning.h"

/* Helper macro to calculate the length of a string literal. */
#define STRING_LITERAL_LENGTH( literal )    ( ( uint16_t ) ( sizeof( literal ) - 1U ) )

/* Template name used in the tests. */
#define TEST_TEMPLATE_NAME                                             "TestTemplateName"
#define TEST_TEMPLATE_NAME_LENGTH                                      STRING_LITERAL_LENGTH( TEST_TEMPLATE_NAME )

/* Topics used in the tests. */
#define TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_PUBLISH_TOPIC            "$aws/certificates/create-from-csr/json"
#define TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_PUBLISH_TOPIC_LENGTH     STRING_LITERAL_LENGTH( TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_PUBLISH_TOPIC )

#define TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_ACCEPTED_TOPIC           "$aws/certificates/create-from-csr/json/accepted"
#define TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_ACCEPTED_TOPIC_LENGTH    STRING_LITERAL_LENGTH( TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_ACCEPTED_TOPIC )

#define TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_REJECTED_TOPIC           "$aws/certificates/create-from-csr/json/rejected"
#define TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_REJECTED_TOPIC_LENGTH    STRING_LITERAL_LENGTH( TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_REJECTED_TOPIC )

#define TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_PUBLISH_TOPIC            "$aws/certificates/create-from-csr/cbor"
#define TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_PUBLISH_TOPIC_LENGTH     STRING_LITERAL_LENGTH( TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_PUBLISH_TOPIC )

#define TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_ACCEPTED_TOPIC           "$aws/certificates/create-from-csr/cbor/accepted"
#define TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_ACCEPTED_TOPIC_LENGTH    STRING_LITERAL_LENGTH( TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_ACCEPTED_TOPIC )

#define TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_REJECTED_TOPIC           "$aws/certificates/create-from-csr/cbor/rejected"
#define TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_REJECTED_TOPIC_LENGTH    STRING_LITERAL_LENGTH( TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_REJECTED_TOPIC )

#define TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_PUBLISH_TOPIC            "$aws/certificates/create/json"
#define TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_PUBLISH_TOPIC_LENGTH     STRING_LITERAL_LENGTH( TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_PUBLISH_TOPIC )

#define TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_ACCEPTED_TOPIC           "$aws/certificates/create/json/accepted"
#define TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_ACCEPTED_TOPIC_LENGTH    STRING_LITERAL_LENGTH( TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_ACCEPTED_TOPIC )

#define TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_REJECTED_TOPIC           "$aws/certificates/create/json/rejected"
#define TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_REJECTED_TOPIC_LENGTH    STRING_LITERAL_LENGTH( TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_REJECTED_TOPIC )

#define TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_PUBLISH_TOPIC            "$aws/certificates/create/cbor"
#define TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_PUBLISH_TOPIC_LENGTH     STRING_LITERAL_LENGTH( TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_PUBLISH_TOPIC )

#define TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_ACCEPTED_TOPIC           "$aws/certificates/create/cbor/accepted"
#define TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_ACCEPTED_TOPIC_LENGTH    STRING_LITERAL_LENGTH( TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_ACCEPTED_TOPIC )

#define TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_REJECTED_TOPIC           "$aws/certificates/create/cbor/rejected"
#define TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_REJECTED_TOPIC_LENGTH    STRING_LITERAL_LENGTH( TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_REJECTED_TOPIC )

#define TEST_REGISTER_THING_JSON_PUBLISH_TOPIC                         "$aws/provisioning-templates/" TEST_TEMPLATE_NAME "/provision/json"
#define TEST_REGISTER_THING_JSON_PUBLISH_TOPIC_LENGTH                  STRING_LITERAL_LENGTH( TEST_REGISTER_THING_JSON_PUBLISH_TOPIC )

#define TEST_REGISTER_THING_JSON_ACCEPTED_TOPIC                        "$aws/provisioning-templates/" TEST_TEMPLATE_NAME "/provision/json/accepted"
#define TEST_REGISTER_THING_JSON_ACCEPTED_TOPIC_LENGTH                 STRING_LITERAL_LENGTH( TEST_REGISTER_THING_JSON_ACCEPTED_TOPIC )

#define TEST_REGISTER_THING_JSON_REJECTED_TOPIC                        "$aws/provisioning-templates/" TEST_TEMPLATE_NAME "/provision/json/rejected"
#define TEST_REGISTER_THING_JSON_REJECTED_TOPIC_LENGTH                 STRING_LITERAL_LENGTH( TEST_REGISTER_THING_JSON_REJECTED_TOPIC )

#define TEST_REGISTER_THING_CBOR_PUBLISH_TOPIC                         "$aws/provisioning-templates/" TEST_TEMPLATE_NAME "/provision/cbor"
#define TEST_REGISTER_THING_CBOR_PUBLISH_TOPIC_LENGTH                  STRING_LITERAL_LENGTH( TEST_REGISTER_THING_CBOR_PUBLISH_TOPIC )

#define TEST_REGISTER_THING_CBOR_ACCEPTED_TOPIC                        "$aws/provisioning-templates/" TEST_TEMPLATE_NAME "/provision/cbor/accepted"
#define TEST_REGISTER_THING_CBOR_ACCEPTED_TOPIC_LENGTH                 STRING_LITERAL_LENGTH( TEST_REGISTER_THING_CBOR_ACCEPTED_TOPIC )

#define TEST_REGISTER_THING_CBOR_REJECTED_TOPIC                        "$aws/provisioning-templates/" TEST_TEMPLATE_NAME "/provision/cbor/rejected"
#define TEST_REGISTER_THING_CBOR_REJECTED_TOPIC_LENGTH                 STRING_LITERAL_LENGTH( TEST_REGISTER_THING_CBOR_REJECTED_TOPIC )

/* Length of the topic buffer used in tests. Guard buffers are placed before and
 * after the topic buffer to verify that Fleet Provisioning APIs do not write
 * out of bounds. The memory layout is:
 *
 *     +--------------+-------------------------------+------------+
 *     |    Guard     |    Writable Topic Buffer      |   Guard    |
 *     +--------------+-------------------------------+------------+
 *
 * Both guard buffers are filled with a known pattern before each test and are
 * verified to remain unchanged after each test.
 */
#define TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH                          32
#define TEST_TOPIC_BUFFER_WRITABLE_LENGTH                              256
#define TEST_TOPIC_BUFFER_SUFFIX_GUARD_LENGTH                          32
#define TEST_TOPIC_BUFFER_TOTAL_LENGTH        \
    ( TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH + \
      TEST_TOPIC_BUFFER_WRITABLE_LENGTH +     \
      TEST_TOPIC_BUFFER_SUFFIX_GUARD_LENGTH )
/*-----------------------------------------------------------*/

/**
 * @brief Topic buffer used in tests.
 */
static char testTopicBuffer[ TEST_TOPIC_BUFFER_TOTAL_LENGTH ];
/*-----------------------------------------------------------*/

/* ============================   UNITY FIXTURES ============================ */

/* Called before each test method. */
void setUp()
{
    /* Initialize the topic buffer with 0xA5. */
    memset( &( testTopicBuffer[ 0 ] ), 0xA5, TEST_TOPIC_BUFFER_TOTAL_LENGTH );
}

/* Called after each test method. */
void tearDown()
{
    /* Prefix and Suffix guard buffers must never change. */
    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ 0 ] ),
                                 TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH +
                                                     TEST_TOPIC_BUFFER_WRITABLE_LENGTH ] ),
                                 TEST_TOPIC_BUFFER_SUFFIX_GUARD_LENGTH );
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
/*-----------------------------------------------------------*/

/**
 * @brief Test that macros generate expected strings.
 */
void test_RegisterThing_MacrosString( void )
{
    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_JSON_PUBLISH_TOPIC,
                                  FLEET_PROVISIONING_JSON_REGISTER_THING_PUBLISH_TOPIC( TEST_TEMPLATE_NAME ),
                                  TEST_REGISTER_THING_JSON_PUBLISH_TOPIC_LENGTH );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_JSON_ACCEPTED_TOPIC,
                                  FLEET_PROVISIONING_JSON_REGISTER_THING_ACCEPTED_TOPIC( TEST_TEMPLATE_NAME ),
                                  TEST_REGISTER_THING_JSON_ACCEPTED_TOPIC_LENGTH );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_JSON_REJECTED_TOPIC,
                                  FLEET_PROVISIONING_JSON_REGISTER_THING_REJECTED_TOPIC( TEST_TEMPLATE_NAME ),
                                  TEST_REGISTER_THING_JSON_REJECTED_TOPIC_LENGTH );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_CBOR_PUBLISH_TOPIC,
                                  FLEET_PROVISIONING_CBOR_REGISTER_THING_PUBLISH_TOPIC( TEST_TEMPLATE_NAME ),
                                  TEST_REGISTER_THING_CBOR_PUBLISH_TOPIC_LENGTH );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_CBOR_ACCEPTED_TOPIC,
                                  FLEET_PROVISIONING_CBOR_REGISTER_THING_ACCEPTED_TOPIC( TEST_TEMPLATE_NAME ),
                                  TEST_REGISTER_THING_CBOR_ACCEPTED_TOPIC_LENGTH );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_CBOR_REJECTED_TOPIC,
                                  FLEET_PROVISIONING_CBOR_REGISTER_THING_REJECTED_TOPIC( TEST_TEMPLATE_NAME ),
                                  TEST_REGISTER_THING_CBOR_REJECTED_TOPIC_LENGTH );
}
/*-----------------------------------------------------------*/

/**
 * @brief Test that macros generates expected string lengths.
 */
void test_RegisterThing_MacrosLength( void )
{
    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_JSON_PUBLISH_TOPIC_LENGTH,
                       FLEET_PROVISIONING_JSON_REGISTER_THING_PUBLISH_TOPIC_LENGTH( TEST_TEMPLATE_NAME_LENGTH ) );

    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_JSON_ACCEPTED_TOPIC_LENGTH,
                       FLEET_PROVISIONING_JSON_REGISTER_THING_ACCEPTED_TOPIC_LENGTH( TEST_TEMPLATE_NAME_LENGTH ) );

    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_JSON_REJECTED_TOPIC_LENGTH,
                       FLEET_PROVISIONING_JSON_REGISTER_THING_REJECTED_TOPIC_LENGTH( TEST_TEMPLATE_NAME_LENGTH ) );

    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_CBOR_PUBLISH_TOPIC_LENGTH,
                       FLEET_PROVISIONING_CBOR_REGISTER_THING_PUBLISH_TOPIC_LENGTH( TEST_TEMPLATE_NAME_LENGTH ) );

    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_CBOR_ACCEPTED_TOPIC_LENGTH,
                       FLEET_PROVISIONING_CBOR_REGISTER_THING_ACCEPTED_TOPIC_LENGTH( TEST_TEMPLATE_NAME_LENGTH ) );

    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_CBOR_REJECTED_TOPIC_LENGTH,
                       FLEET_PROVISIONING_CBOR_REGISTER_THING_REJECTED_TOPIC_LENGTH( TEST_TEMPLATE_NAME_LENGTH ) );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_GetRegisterThingTopic_BadParams( void )
{
    FleetProvisioningStatus_t ret;
    uint16_t topicLength;

    /* Null buffer. */
    ret = FleetProvisioning_GetRegisterThingTopic( NULL,
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningJson,
                                                   FleetProvisioningPublish,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );
    TEST_ASSERT_EQUAL( FleetProvisioningBadParameter, ret );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH );

    /* Invalid Format. */
    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningCbor + 1,
                                                   FleetProvisioningPublish,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );
    TEST_ASSERT_EQUAL( FleetProvisioningBadParameter, ret );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH );

    /* Invalid api. */
    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningCbor,
                                                   FleetProvisioningRejected + 1,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );
    TEST_ASSERT_EQUAL( FleetProvisioningBadParameter, ret );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH );

    /* NULL template name. */
    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningJson,
                                                   FleetProvisioningPublish,
                                                   NULL,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );
    TEST_ASSERT_EQUAL( FleetProvisioningBadParameter, ret );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH );

    /* Zero length thing name. */
    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningJson,
                                                   FleetProvisioningPublish,
                                                   TEST_TEMPLATE_NAME,
                                                   0,
                                                   &( topicLength ) );
    TEST_ASSERT_EQUAL( FleetProvisioningBadParameter, ret );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH );

    /* Thing name length more than the maximum allowed. */
    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningJson,
                                                   FleetProvisioningPublish,
                                                   TEST_TEMPLATE_NAME,
                                                   FLEET_PROVISIONING_TEMPLATENAME_MAX_LENGTH + 1,
                                                   &( topicLength ) );
    TEST_ASSERT_EQUAL( FleetProvisioningBadParameter, ret );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH );

    /* NULL output parameter. */
    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningJson,
                                                   FleetProvisioningPublish,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   NULL );
    TEST_ASSERT_EQUAL( FleetProvisioningBadParameter, ret );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_GetRegisterThingTopic_BufferTooSmall( void )
{
    FleetProvisioningStatus_t ret;
    uint16_t topicLength;

    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   5, /* Length too small to fit the entire topic. */
                                                   FleetProvisioningJson,
                                                   FleetProvisioningPublish,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );
    TEST_ASSERT_EQUAL( FleetProvisioningBufferTooSmall, ret );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_GetRegisterThingTopic_JsonPublishHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    uint16_t topicLength;

    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningJson,
                                                   FleetProvisioningPublish,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_JSON_PUBLISH_TOPIC_LENGTH, topicLength );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_JSON_PUBLISH_TOPIC,
                                  &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                  topicLength );

    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH + topicLength ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH - topicLength );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_GetRegisterThingTopic_JsonAcceptedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    uint16_t topicLength;

    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningJson,
                                                   FleetProvisioningAccepted,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_JSON_ACCEPTED_TOPIC_LENGTH, topicLength );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_JSON_ACCEPTED_TOPIC,
                                  &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                  topicLength );

    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH + topicLength ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH - topicLength );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_GetRegisterThingTopic_JsonRejectedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    uint16_t topicLength;

    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningJson,
                                                   FleetProvisioningRejected,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_JSON_REJECTED_TOPIC_LENGTH, topicLength );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_JSON_REJECTED_TOPIC,
                                  &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                  topicLength );

    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH + topicLength ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH - topicLength );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_GetRegisterThingTopic_CborPublishHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    uint16_t topicLength;

    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningCbor,
                                                   FleetProvisioningPublish,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_CBOR_PUBLISH_TOPIC_LENGTH, topicLength );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_CBOR_PUBLISH_TOPIC,
                                  &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                  topicLength );

    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH + topicLength ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH - topicLength );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_GetRegisterThingTopic_CborAcceptedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    uint16_t topicLength;

    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningCbor,
                                                   FleetProvisioningAccepted,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_CBOR_ACCEPTED_TOPIC_LENGTH, topicLength );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_CBOR_ACCEPTED_TOPIC,
                                  &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                  topicLength );

    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH + topicLength ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH - topicLength );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_GetRegisterThingTopic_CborRejectedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    uint16_t topicLength;

    ret = FleetProvisioning_GetRegisterThingTopic( &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                                   TEST_TOPIC_BUFFER_WRITABLE_LENGTH,
                                                   FleetProvisioningCbor,
                                                   FleetProvisioningRejected,
                                                   TEST_TEMPLATE_NAME,
                                                   TEST_TEMPLATE_NAME_LENGTH,
                                                   &( topicLength ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( TEST_REGISTER_THING_CBOR_REJECTED_TOPIC_LENGTH, topicLength );

    TEST_ASSERT_EQUAL_STRING_LEN( TEST_REGISTER_THING_CBOR_REJECTED_TOPIC,
                                  &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH ] ),
                                  topicLength );

    TEST_ASSERT_EACH_EQUAL_HEX8( 0xA5,
                                 &( testTopicBuffer[ TEST_TOPIC_BUFFER_PREFIX_GUARD_LENGTH + topicLength ] ),
                                 TEST_TOPIC_BUFFER_WRITABLE_LENGTH - topicLength );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_BadParams( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    /* NULL topic. */
    ret = FleetProvisioning_MatchTopic( NULL,
                                        TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_PUBLISH_TOPIC_LENGTH,
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningBadParameter, ret );

    /* NULL output parameter. */
    ret = FleetProvisioning_MatchTopic( TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_PUBLISH_TOPIC,
                                        TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_PUBLISH_TOPIC_LENGTH,
                                        NULL );
    TEST_ASSERT_EQUAL( FleetProvisioningBadParameter, ret );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_InvalidFormat( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( "$aws/cert",
                                        STRING_LITERAL_LENGTH( "$aws/cert" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create-from-csr/bad",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create-from-csr/bad" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create/bad",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create/bad" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/provisioning-templates/TestTemplate/provision/bad",
                                        STRING_LITERAL_LENGTH( "$aws/provisioning-templates/TestTemplate/provision/bad" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_ZeroLengthTemplateName( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( "$aws/provisioning-templates//provision",
                                        STRING_LITERAL_LENGTH( "$aws/provisioning-templates//provision" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_RegisterThingMissingSuffix( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( "$aws/provisioning-templates/TestTemplate",
                                        STRING_LITERAL_LENGTH( "$aws/provisioning-templates/TestTemplate" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_InvalidSuffix( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create-from-csr/json/bad",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create-from-csr/json/bad" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create/json/bad",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create/json/bad" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/provisioning-templates/TestTemplate/provision/json/bad",
                                        STRING_LITERAL_LENGTH( "$aws/provisioning-templates/TestTemplate/provision/json/bad" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create-from-csr/cbor/bad",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create-from-csr/cbor/bad" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create/cbor/bad",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create/cbor/bad" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/provisioning-templates/TestTemplate/provision/cbor/bad",
                                        STRING_LITERAL_LENGTH( "$aws/provisioning-templates/TestTemplate/provision/cbor/bad" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_ExtraData( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create-from-csr/json/gibberish",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create-from-csr/json/gibberish" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create-from-csr/json/accepted/gibberish",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create-from-csr/json/accepted/gibberish" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create-from-csr/json/rejected/gibberish",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create-from-csr/json/accepted/gibberish" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create-from-csr/cbor/gibberish",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create-from-csr/json/accepted/gibberish" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create-from-csr/cbor/accepted/gibberish",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create-from-csr/json/accepted/gibberish" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );

    ret = FleetProvisioning_MatchTopic( "$aws/certificates/create-from-csr/cbor/rejected/gibberish",
                                        STRING_LITERAL_LENGTH( "$aws/certificates/create-from-csr/json/accepted/gibberish" ),
                                        &( api ) );
    TEST_ASSERT_EQUAL( FleetProvisioningNoMatch, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningInvalidTopic, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateCertificateFromCsrJsonPublishHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_PUBLISH_TOPIC,
                                        TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_PUBLISH_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningJsonCreateCertificateFromCsrPublish, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateCertificateFromCsrJsonAcceptedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_ACCEPTED_TOPIC,
                                        TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_ACCEPTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningJsonCreateCertificateFromCsrAccepted, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateCertificateFromCsrJsonRejectedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_REJECTED_TOPIC,
                                        TEST_CREATE_CERTIFICATE_FROM_CSR_JSON_REJECTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningJsonCreateCertificateFromCsrRejected, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateCertificateFromCsrCborPublishHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_PUBLISH_TOPIC,
                                        TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_PUBLISH_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningCborCreateCertificateFromCsrPublish, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateCertificateFromCsrCborAcceptedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_ACCEPTED_TOPIC,
                                        TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_ACCEPTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningCborCreateCertificateFromCsrAccepted, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateCertificateFromCsrCborRejectedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_REJECTED_TOPIC,
                                        TEST_CREATE_CERTIFICATE_FROM_CSR_CBOR_REJECTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningCborCreateCertificateFromCsrRejected, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateKeysAndCertificateJsonPublishHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_PUBLISH_TOPIC,
                                        TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_PUBLISH_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningJsonCreateKeysAndCertificatePublish, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateKeysAndCertificateJsonAcceptedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_ACCEPTED_TOPIC,
                                        TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_ACCEPTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningJsonCreateKeysAndCertificateAccepted, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateKeysAndCertificateJsonRejectedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_REJECTED_TOPIC,
                                        TEST_CREATE_KEYS_AND_CERTIFICATE_JSON_REJECTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningJsonCreateKeysAndCertificateRejected, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateKeysAndCertificateCborPublishHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_PUBLISH_TOPIC,
                                        TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_PUBLISH_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningCborCreateKeysAndCertificatePublish, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateKeysAndCertificateCborAcceptedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_ACCEPTED_TOPIC,
                                        TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_ACCEPTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningCborCreateKeysAndCertificateAccepted, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_CreateKeysAndCertificateCborRejectedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_REJECTED_TOPIC,
                                        TEST_CREATE_KEYS_AND_CERTIFICATE_CBOR_REJECTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningCborCreateKeysAndCertificateRejected, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_RegisterThingJsonPublishHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_REGISTER_THING_JSON_PUBLISH_TOPIC,
                                        TEST_REGISTER_THING_JSON_PUBLISH_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningJsonRegisterThingPublish, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_RegisterThingJsonAcceptedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_REGISTER_THING_JSON_ACCEPTED_TOPIC,
                                        TEST_REGISTER_THING_JSON_ACCEPTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningJsonRegisterThingAccepted, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_RegisterThingJsonRejectedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_REGISTER_THING_JSON_REJECTED_TOPIC,
                                        TEST_REGISTER_THING_JSON_REJECTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningJsonRegisterThingRejected, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_RegisterThingCborPublishHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_REGISTER_THING_CBOR_PUBLISH_TOPIC,
                                        TEST_REGISTER_THING_CBOR_PUBLISH_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningCborRegisterThingPublish, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_RegisterThingCborAcceptedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_REGISTER_THING_CBOR_ACCEPTED_TOPIC,
                                        TEST_REGISTER_THING_CBOR_ACCEPTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningCborRegisterThingAccepted, api );
}
/*-----------------------------------------------------------*/

void test_FleetProvisioning_MatchTopic_RegisterThingCborRejectedHappyPath( void )
{
    FleetProvisioningStatus_t ret;
    FleetProvisioningTopic_t api;

    ret = FleetProvisioning_MatchTopic( TEST_REGISTER_THING_CBOR_REJECTED_TOPIC,
                                        TEST_REGISTER_THING_CBOR_REJECTED_TOPIC_LENGTH,
                                        &( api ) );

    TEST_ASSERT_EQUAL( FleetProvisioningSuccess, ret );
    TEST_ASSERT_EQUAL( FleetProvisioningCborRegisterThingRejected, api );
}
/*-----------------------------------------------------------*/
