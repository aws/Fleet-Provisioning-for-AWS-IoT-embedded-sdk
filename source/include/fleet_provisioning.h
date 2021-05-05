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


#endif /* FLEET_PROVISIONING_H_ */
