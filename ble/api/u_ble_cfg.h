/*
 * Copyright 2020 u-blox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _U_BLE_CFG_H_
#define _U_BLE_CFG_H_

/* No #includes allowed here */

/** @file
 * @brief This header file defines the APIs that configure ble.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------
 * COMPILE-TIME MACROS
 * -------------------------------------------------------------- */

/* ----------------------------------------------------------------
 * TYPES
 * -------------------------------------------------------------- */

typedef enum {
    U_BLE_CFG_ROLE_DISABLED = 0, /**< BLE disabled. */
    U_BLE_CFG_ROLE_CENTRAL, /**< Central only mode. */
    U_BLE_CFG_ROLE_PERIPHERAL, /**< Peripheral only mode. */
    U_BLE_CFG_ROLE_CENTRAL_AND_PERIPHERAL, /**< Simultaneous central and peripheral mode. */
} uBleCfgRole_t;

typedef struct {
    uBleCfgRole_t role;
    bool spsServer;
} uBleCfg_t;

/* ----------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------- */

/** Configure ble for a short range module, may require module restarts
 *  so can take up to 500 ms before it returns.
 *
 * @param bleHandle   the handle of the ble instance.
 * @param pCfg        pointer to the configuration data, must not be NULL.
 * @return            zero on success or negative error code
 *                    on failure.
 */
int32_t uBleCfgConfigure(int32_t bleHandle,
                         const uBleCfg_t *pCfg);
#ifdef __cplusplus
}
#endif

#endif // _U_BLE_CFG_H_

// End of file
