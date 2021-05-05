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
 * @file fleet_provisioning.h
 * @brief Interface for the AWS IoT Fleet Provisioning Client Library.
 */

#ifndef FLEET_PROVISIONING_H_
#define FLEET_PROVISIONING_H_

/* Standard includes. */
#include <stdint.h>

/* FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG allows building the Fleet
 * Provisioning library without a config file. If a config file is provided,
 * FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG macro must not be defined.
 */
#ifndef FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG
    #include "fleet_provisioning_config.h"
#endif

/* Default config values. */
#include "fleet_provisioning_config_defaults.h"

/**
 * @ingroup fleet_provisioning_enum_types
 * @brief Return codes for Fleet Provisioning APIs.
 */
typedef enum
{
    FleetProvisioningError = 0,
    FleetProvisioningSuccess,
    FleetProvisioningNoMatch,
    FleetProvisioningBadParameter,
    FleetProvisioningBufferTooSmall
} FleetProvisioningStatus_t;

/**
 * @ingroup fleet_provisioning_enum_types
 * @brief Fleet Provisioning topic values.
 */
typedef enum
{
    FleetProvisioningInvalidTopic = 0,
    FleetProvisioningJsonCreateCertificateFromCsrPublish,
    FleetProvisioningJsonCreateCertificateFromCsrAccepted,
    FleetProvisioningJsonCreateCertificateFromCsrRejected,
    FleetProvisioningJsonCreateKeysAndCertificatePublish,
    FleetProvisioningJsonCreateKeysAndCertificateAccepted,
    FleetProvisioningJsonCreateKeysAndCertificateRejected,
    FleetProvisioningJsonRegisterThingPublish,
    FleetProvisioningJsonRegisterThingAccepted,
    FleetProvisioningJsonRegisterThingRejected,
    FleetProvisioningCborCreateCertificateFromCsrPublish,
    FleetProvisioningCborCreateCertificateFromCsrAccepted,
    FleetProvisioningCborCreateCertificateFromCsrRejected,
    FleetProvisioningCborCreateKeysAndCertificatePublish,
    FleetProvisioningCborCreateKeysAndCertificateAccepted,
    FleetProvisioningCborCreateKeysAndCertificateRejected,
    FleetProvisioningCborRegisterThingPublish,
    FleetProvisioningCborRegisterThingAccepted,
    FleetProvisioningCborRegisterThingRejected
} FleetProvisioningTopic_t;

/**
 * @ingroup fleet_provisioning_enum_types
 * @brief Topics for each Fleet Provisioning APIs.
 */
typedef enum
{
    FleetProvisioningPublish,
    FleetProvisioningAccepted,
    FleetProvisioningRejected
} FleetProvisioningApiTopics_t;

/**
 * @ingroup fleet_provisioning_enum_types
 * @brief Message format for Fleet Provisioning APIs.
 */
typedef enum
{
    FleetProvisioningJson,
    FleetProvisioningCbor
} FleetProvisioningFormat_t;

/*-----------------------------------------------------------*/

/**
 * @ingroup fleet_provisioning_constants
 * @brief Maximum length of a thing's name as permitted by AWS IoT Core.
 */
#define FLEET_PROVISIONING_TEMPLATENAME_MAX_LENGTH    36U

/*-----------------------------------------------------------*/

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore these macros as they are private.
 */

#define FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_PREFIX           "$aws/certificates/create-from-csr/"
#define FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_LENGTH_PREFIX    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_PREFIX ) - 1U ) )

#define FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_PREFIX           "$aws/certificates/create/"
#define FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_LENGTH_PREFIX    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_PREFIX ) - 1U ) )

#define FLEET_PROVISIONING_REGISTER_THING_API_PREFIX                        "$aws/provisioning-templates/"
#define FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_PREFIX                 ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_REGISTER_THING_API_PREFIX ) - 1U ) )

#define FLEET_PROVISIONING_REGISTER_THING_API_BRIDGE                        "/provision/"
#define FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_BRIDGE                 ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_REGISTER_THING_API_BRIDGE ) - 1U ) )

#define FLEET_PROVISIONING_API_JSON_FORMAT                                  "json"
#define FLEET_PROVISIONING_API_LENGTH_JSON_FORMAT                           ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_API_JSON_FORMAT ) - 1U ) )

#define FLEET_PROVISIONING_API_CBOR_FORMAT                                  "cbor"
#define FLEET_PROVISIONING_API_LENGTH_CBOR_FORMAT                           ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_API_CBOR_FORMAT ) - 1U ) )

#define FLEET_PROVISIONING_API_ACCEPTED_SUFFIX                              "/accepted"
#define FLEET_PROVISIONING_API_LENGTH_ACCEPTED_SUFFIX                       ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_API_ACCEPTED_SUFFIX ) - 1U ) )

#define FLEET_PROVISIONING_API_REJECTED_SUFFIX                              "/rejected"
#define FLEET_PROVISIONING_API_LENGTH_REJECTED_SUFFIX                       ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_API_REJECTED_SUFFIX ) - 1U ) )

/** @endcond */

/*-----------------------------------------------------------*/

/* Fleet Provisioning CreateCertificateFromCSR macros */

/**
 * @brief Topic string for publishing a JSON CreateCertificateFromCSR request.
 */
#define FLEET_PROVISIONING_JSON_CREATE_CERTIFICATE_FROM_CSR_PUBLISH_TOPIC \
    ( FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_PREFIX           \
      FLEET_PROVISIONING_API_JSON_FORMAT )

/**
 * @brief Topic string for getting a JSON CreateCertificateFromCSR response.
 */
#define FLEET_PROVISIONING_JSON_CREATE_CERTIFICATE_FROM_CSR_ACCEPTED_TOPIC \
    ( FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_PREFIX            \
      FLEET_PROVISIONING_API_JSON_FORMAT                                   \
      FLEET_PROVISIONING_API_ACCEPTED_SUFFIX )

/**
 * @brief Topic string for getting a JSON CreateCertificateFromCSR error response.
 */
#define FLEET_PROVISIONING_JSON_CREATE_CERTIFICATE_FROM_CSR_REJECTED_TOPIC \
    ( FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_PREFIX            \
      FLEET_PROVISIONING_API_JSON_FORMAT                                   \
      FLEET_PROVISIONING_API_REJECTED_SUFFIX )

/**
 * @brief Topic string for publishing a CBOR CreateCertificateFromCSR request.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_CERTIFICATE_FROM_CSR_PUBLISH_TOPIC \
    ( FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_PREFIX           \
      FLEET_PROVISIONING_API_CBOR_FORMAT )

/**
 * @brief Topic string for getting a CBOR CreateCertificateFromCSR response.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_CERTIFICATE_FROM_CSR_ACCEPTED_TOPIC \
    ( FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_PREFIX            \
      FLEET_PROVISIONING_API_CBOR_FORMAT                                   \
      FLEET_PROVISIONING_API_ACCEPTED_SUFFIX )

/**
 * @brief Topic string for getting a CBOR CreateCertificateFromCSR error response.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_CERTIFICATE_FROM_CSR_REJECTED_TOPIC \
    ( FLEET_PROVISIONING_CREATE_CERTIFICATE_FROM_CSR_API_PREFIX            \
      FLEET_PROVISIONING_API_CBOR_FORMAT                                   \
      FLEET_PROVISIONING_API_REJECTED_SUFFIX )

/**
 * @brief Length of topic string for publishing a JSON CreateCertificateFromCSR request.
 */
#define FLEET_PROVISIONING_JSON_CREATE_CERTIFICATE_FROM_CSR_PUBLISH_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_JSON_CREATE_CERTIFICATE_FROM_CSR_PUBLISH_TOPIC ) - 1U ) )

/**
 * @brief Length of topic string for getting a JSON CreateCertificateFromCSR response.
 */
#define FLEET_PROVISIONING_JSON_CREATE_CERTIFICATE_FROM_CSR_ACCEPTED_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_JSON_CREATE_CERTIFICATE_FROM_CSR_ACCEPTED_TOPIC ) - 1U ) )

/**
 * @brief Length of topic string for getting a JSON CreateCertificateFromCSR error response.
 */
#define FLEET_PROVISIONING_JSON_CREATE_CERTIFICATE_FROM_CSR_REJECTED_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_JSON_CREATE_CERTIFICATE_FROM_CSR_REJECTED_TOPIC ) - 1U ) )

/**
 * @brief Length of topic string for publishing a CBOR CreateCertificateFromCSR request.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_CERTIFICATE_FROM_CSR_PUBLISH_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_CBOR_CREATE_CERTIFICATE_FROM_CSR_PUBLISH_TOPIC ) - 1U ) )

/**
 * @brief Length of topic string for getting a CBOR CreateCertificateFromCSR response.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_CERTIFICATE_FROM_CSR_ACCEPTED_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_CBOR_CREATE_CERTIFICATE_FROM_CSR_ACCEPTED_TOPIC ) - 1U ) )

/**
 * @brief Length of topic string for getting a CBOR CreateCertificateFromCSR error response.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_CERTIFICATE_FROM_CSR_REJECTED_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_CBOR_CREATE_CERTIFICATE_FROM_CSR_REJECTED_TOPIC ) - 1U ) )

/*-----------------------------------------------------------*/

/* Fleet Provisioning CreateKeysAndCertificate macros */

/**
 * @brief Topic string for publishing a JSON CreateKeysAndCertificate request.
 */
#define FLEET_PROVISIONING_JSON_CREATE_KEYS_AND_CERTIFICATE_PUBLISH_TOPIC \
    ( FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_PREFIX           \
      FLEET_PROVISIONING_API_JSON_FORMAT )

/**
 * @brief Topic string for getting a JSON CreateKeysAndCertificate response.
 */
#define FLEET_PROVISIONING_JSON_CREATE_KEYS_AND_CERTIFICATE_ACCEPTED_TOPIC \
    ( FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_PREFIX            \
      FLEET_PROVISIONING_API_JSON_FORMAT                                   \
      FLEET_PROVISIONING_API_ACCEPTED_SUFFIX )

/**
 * @brief Topic string for getting a JSON CreateKeysAndCertificate error
 * response.
 */
#define FLEET_PROVISIONING_JSON_CREATE_KEYS_AND_CERTIFICATE_REJECTED_TOPIC \
    ( FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_PREFIX            \
      FLEET_PROVISIONING_API_JSON_FORMAT                                   \
      FLEET_PROVISIONING_API_REJECTED_SUFFIX )

/**
 * @brief Topic string for publishing a CBOR CreateKeysAndCertificate request.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_KEYS_AND_CERTIFICATE_PUBLISH_TOPIC \
    ( FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_PREFIX           \
      FLEET_PROVISIONING_API_CBOR_FORMAT )

/**
 * @brief Topic string for getting a CBOR CreateKeysAndCertificate response.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_KEYS_AND_CERTIFICATE_ACCEPTED_TOPIC \
    ( FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_PREFIX            \
      FLEET_PROVISIONING_API_CBOR_FORMAT                                   \
      FLEET_PROVISIONING_API_ACCEPTED_SUFFIX )

/**
 * @brief Topic string for getting a CBOR CreateKeysAndCertificate error
 * response.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_KEYS_AND_CERTIFICATE_REJECTED_TOPIC \
    ( FLEET_PROVISIONING_CREATE_KEYS_AND_CERTIFICATE_API_PREFIX            \
      FLEET_PROVISIONING_API_CBOR_FORMAT                                   \
      FLEET_PROVISIONING_API_REJECTED_SUFFIX )

/**
 * @brief Length of topic string for publishing a JSON CreateKeysAndCertificate request.
 */
#define FLEET_PROVISIONING_JSON_CREATE_KEYS_AND_CERTIFICATE_PUBLISH_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_JSON_CREATE_KEYS_AND_CERTIFICATE_PUBLISH_TOPIC ) - 1U ) )

/**
 * @brief Length of topic string for getting a JSON CreateKeysAndCertificate response.
 */
#define FLEET_PROVISIONING_JSON_CREATE_KEYS_AND_CERTIFICATE_ACCEPTED_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_JSON_CREATE_KEYS_AND_CERTIFICATE_ACCEPTED_TOPIC ) - 1U ) )

/**
 * @brief Length of topic string for getting a JSON CreateKeysAndCertificate error response.
 */
#define FLEET_PROVISIONING_JSON_CREATE_KEYS_AND_CERTIFICATE_REJECTED_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_JSON_CREATE_KEYS_AND_CERTIFICATE_REJECTED_TOPIC ) - 1U ) )

/**
 * @brief Length of topic string for publishing a CBOR CreateKeysAndCertificate request.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_KEYS_AND_CERTIFICATE_PUBLISH_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_CBOR_CREATE_KEYS_AND_CERTIFICATE_PUBLISH_TOPIC ) - 1U ) )

/**
 * @brief Length of topic string for getting a CBOR CreateKeysAndCertificate response.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_KEYS_AND_CERTIFICATE_ACCEPTED_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_CBOR_CREATE_KEYS_AND_CERTIFICATE_ACCEPTED_TOPIC ) - 1U ) )

/**
 * @brief Length of topic string for getting a CBOR CreateKeysAndCertificate error response.
 */
#define FLEET_PROVISIONING_CBOR_CREATE_KEYS_AND_CERTIFICATE_REJECTED_TOPIC_LENGTH \
    ( ( uint16_t ) ( sizeof( FLEET_PROVISIONING_CBOR_CREATE_KEYS_AND_CERTIFICATE_REJECTED_TOPIC ) - 1U ) )

/*-----------------------------------------------------------*/

/* Fleet Provisioning RegisterThing macros */


/**
 * @brief Topic string for publishing a JSON RegisterThing request.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateName The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_JSON_REGISTER_THING_PUBLISH_TOPIC( templateName ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_PREFIX                           \
      templateName                                                           \
      FLEET_PROVISIONING_REGISTER_THING_API_BRIDGE                           \
      FLEET_PROVISIONING_API_JSON_FORMAT )

/**
 * @brief Topic string for getting a JSON RegisterThing response.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateName The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_JSON_REGISTER_THING_ACCEPTED_TOPIC( templateName ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_PREFIX                            \
      templateName                                                            \
      FLEET_PROVISIONING_REGISTER_THING_API_BRIDGE                            \
      FLEET_PROVISIONING_API_JSON_FORMAT                                      \
      FLEET_PROVISIONING_API_ACCEPTED_SUFFIX )

/**
 * @brief Topic string for getting a JSON RegisterThing error response.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateName The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_JSON_REGISTER_THING_REJECTED_TOPIC( templateName ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_PREFIX                            \
      templateName                                                            \
      FLEET_PROVISIONING_REGISTER_THING_API_BRIDGE                            \
      FLEET_PROVISIONING_API_JSON_FORMAT                                      \
      FLEET_PROVISIONING_API_REJECTED_SUFFIX )

/**
 * @brief Topic string for publishing a CBOR RegisterThing request.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateName The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_CBOR_REGISTER_THING_PUBLISH_TOPIC( templateName ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_PREFIX                           \
      templateName                                                           \
      FLEET_PROVISIONING_REGISTER_THING_API_BRIDGE                           \
      FLEET_PROVISIONING_API_CBOR_FORMAT )

/**
 * @brief Topic string for getting a CBOR RegisterThing response.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateName The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_CBOR_REGISTER_THING_ACCEPTED_TOPIC( templateName ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_PREFIX                            \
      templateName                                                            \
      FLEET_PROVISIONING_REGISTER_THING_API_BRIDGE                            \
      FLEET_PROVISIONING_API_CBOR_FORMAT                                      \
      FLEET_PROVISIONING_API_ACCEPTED_SUFFIX )

/**
 * @brief Topic string for getting a CBOR RegisterThing error response.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateName The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_CBOR_REGISTER_THING_REJECTED_TOPIC( templateName ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_PREFIX                            \
      templateName                                                            \
      FLEET_PROVISIONING_REGISTER_THING_API_BRIDGE                            \
      FLEET_PROVISIONING_API_CBOR_FORMAT                                      \
      FLEET_PROVISIONING_API_REJECTED_SUFFIX )

/**
 * @brief Length of topic string for publishing a JSON RegisterThing request.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateNameLength The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_JSON_REGISTER_THING_PUBLISH_TOPIC_LENGTH( templateNameLength ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_PREFIX +                               \
      ( templateNameLength ) +                                                            \
      FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_BRIDGE +                               \
      FLEET_PROVISIONING_API_LENGTH_JSON_FORMAT )

/**
 * @brief Length of topic string for getting a JSON RegisterThing response.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateNameLength The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_JSON_REGISTER_THING_ACCEPTED_TOPIC_LENGTH( templateNameLength ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_PREFIX +                                \
      ( templateNameLength ) +                                                             \
      FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_BRIDGE +                                \
      FLEET_PROVISIONING_API_LENGTH_JSON_FORMAT +                                          \
      FLEET_PROVISIONING_API_LENGTH_ACCEPTED_SUFFIX )

/**
 * @brief Length of topic string for getting a JSON RegisterThing error response.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateNameLength The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_JSON_REGISTER_THING_REJECTED_TOPIC_LENGTH( templateNameLength ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_PREFIX +                                \
      ( templateNameLength ) +                                                             \
      FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_BRIDGE +                                \
      FLEET_PROVISIONING_API_LENGTH_JSON_FORMAT +                                          \
      FLEET_PROVISIONING_API_LENGTH_REJECTED_SUFFIX )

/**
 * @brief Length of topic string for publishing a CBOR RegisterThing request.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateNameLength The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_CBOR_REGISTER_THING_PUBLISH_TOPIC_LENGTH( templateNameLength ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_PREFIX +                               \
      ( templateNameLength ) +                                                            \
      FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_BRIDGE +                               \
      FLEET_PROVISIONING_API_LENGTH_CBOR_FORMAT )

/**
 * @brief Length of topic string for getting a CBOR RegisterThing response.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateNameLength The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_CBOR_REGISTER_THING_ACCEPTED_TOPIC_LENGTH( templateNameLength ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_PREFIX +                                \
      ( templateNameLength ) +                                                             \
      FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_BRIDGE +                                \
      FLEET_PROVISIONING_API_LENGTH_CBOR_FORMAT +                                          \
      FLEET_PROVISIONING_API_LENGTH_ACCEPTED_SUFFIX )

/**
 * @brief Length of topic string for getting a CBOR RegisterThing error response.
 *
 * This macro should be used when the provisioning template name is known at
 * compile time. If the provisioning template name is not known at compile time,
 * the #FleetProvisioning_GetRegisterThingTopic API should be used instead.
 *
 * @param templateNameLength The name of the provisioning template to use.
 */
#define FLEET_PROVISIONING_CBOR_REGISTER_THING_REJECTED_TOPIC_LENGTH( templateNameLength ) \
    ( FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_PREFIX +                                \
      ( templateNameLength ) +                                                             \
      FLEET_PROVISIONING_REGISTER_THING_API_LENGTH_BRIDGE +                                \
      FLEET_PROVISIONING_API_LENGTH_CBOR_FORMAT +                                          \
      FLEET_PROVISIONING_API_LENGTH_REJECTED_SUFFIX )

/*-----------------------------------------------------------*/

#endif /* FLEET_PROVISIONING_H_ */