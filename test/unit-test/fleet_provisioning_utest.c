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