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

#ifndef _U_CELL_PRIVATE_H_
#define _U_CELL_PRIVATE_H_

/* No #includes allowed here */

/** @file
 * @brief This header file defines types, functions and inclusions that
 * are common and private to the cellular API.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------
 * COMPILE-TIME MACROS
 * -------------------------------------------------------------- */

/** The maximum number of RATs that can be supported
 * simultaneously by any module.
 */
#define U_CELL_PRIVATE_MAX_NUM_SIMULTANEOUS_RATS 3

#ifndef U_CELL_PRIVATE_AT_CFUN_OFF_RESPONSE_TIME_SECONDS
/** The amount of time to allow to transition to
 * AT+CFUN=0, AT+CFUN=4, AT+CFUN=15 or AT+CFUN=16
 * (can sometimes take longer than the usual AT
 * default response time).
 */
# define U_CELL_PRIVATE_AT_CFUN_OFF_RESPONSE_TIME_SECONDS 30
#endif

#ifndef U_CELL_PRIVATE_AT_CFUN_FLIP_DELAY_SECONDS
/** Modules can sometimes get upset if they are flipped
 * in and out of AT+CFUN=0/4 to AT+CFUN=1 states in
 * rapid succession.  This delay to mitigate that problem.
 */
# define U_CELL_PRIVATE_AT_CFUN_FLIP_DELAY_SECONDS 1
#endif

#ifndef U_CELL_PRIVATE_CPWROFF_WAIT_TIME_SECONDS
/** The amount of time to wait for the AT+CPWROFF
 * command to return an OK or ERROR response.
 */
# define U_CELL_PRIVATE_CPWROFF_WAIT_TIME_SECONDS 40
#endif

#ifndef U_CELL_PRIVATE_COPS_WAIT_TIME_SECONDS
/** The amount of time to wait for AT+COPS=
 * command to return an OK or ERROR response.
 */
# define U_CELL_PRIVATE_COPS_WAIT_TIME_SECONDS 30
#endif

/** Return true if the given module type is SARA-R4-xx.
 */
#define U_CELL_PRIVATE_MODULE_IS_SARA_R4(moduleType)      \
    (((moduleType) == U_CELL_MODULE_TYPE_SARA_R410M_02B) || \
     ((moduleType) == U_CELL_MODULE_TYPE_SARA_R412M_02B) || \
     ((moduleType) == U_CELL_MODULE_TYPE_SARA_R412M_03B) || \
     ((moduleType) == U_CELL_MODULE_TYPE_SARA_R410M_03B) || \
     ((moduleType) == U_CELL_MODULE_TYPE_SARA_R422))

/** Return true if the supported RATS bitmap includes LTE.
 */
#define U_CELL_PRIVATE_SUPPORTED_RATS_LTE(supportedRatsBitmap)  \
    ((supportedRatsBitmap) &                      \
     ((1UL << (int32_t) U_CELL_NET_RAT_LTE)   |   \
      (1UL << (int32_t) U_CELL_NET_RAT_CATM1) |   \
      (1UL << (int32_t) U_CELL_NET_RAT_NB1)))

/** Return true if the given RAT is an EUTRAN RAT.
 */
//lint -esym(755, U_CELL_PRIVATE_RAT_IS_EUTRAN) Suppress macro not
// referenced as references may be conditionally compiled-out.
#define U_CELL_PRIVATE_RAT_IS_EUTRAN(rat)  \
    (((rat) == U_CELL_NET_RAT_LTE) ||      \
     ((rat) == U_CELL_NET_RAT_CATM1) ||    \
     ((rat) == U_CELL_NET_RAT_NB1))

/** Determine if a given network status value means that
 * we're registered.
 */
#define U_CELL_PRIVATE_STATUS_MEANS_REGISTERED(status)             \
   (((status) == U_CELL_NET_STATUS_REGISTERED_HOME) ||             \
    ((status) == U_CELL_NET_STATUS_REGISTERED_ROAMING) ||          \
    ((status) == U_CELL_NET_STATUS_REGISTERED_SMS_ONLY_HOME) ||    \
    ((status) == U_CELL_NET_STATUS_REGISTERED_SMS_ONLY_ROAMING) || \
    ((status) == U_CELL_NET_STATUS_REGISTERED_NO_CSFB_HOME) ||     \
    ((status) == U_CELL_NET_STATUS_REGISTERED_NO_CSFB_ROAMING))

/** Determine if the given feature is supported or not
 * by the pointed-to module.
 */
//lint --emacro((774), U_CELL_PRIVATE_HAS) Suppress left side always
// evaluates to True
#define U_CELL_PRIVATE_HAS(pModule, feature) \
    ((pModule != NULL) && ((pModule->featuresBitmap) & (1UL << (int32_t) (feature))))

#ifndef U_CELL_PRIVATE_GREETING_STR
/** A greeting string, a useful indication that the module
 * rebooted underneath us unexpectedly.
 */
#define U_CELL_PRIVATE_GREETING_STR "Module has booted."
#endif

#ifndef U_CELL_PRIVATE_UART_WAKE_UP_RETRIES
/** The number of times to retry poking the AT interface
 * to wake the module up from UART power saving.
 */
# define U_CELL_PRIVATE_UART_WAKE_UP_RETRIES 3
#endif

#ifndef U_CELL_PRIVATE_UART_WAKE_UP_FIRST_WAIT_MS
/** How long to wait for the response to the first poke
 * of the AT interface when waking the module up from
 * UART power saving; this should be relatively
 * short as the outgoing poke is quite likely to be
 * lost.
 */
# define U_CELL_PRIVATE_UART_WAKE_UP_FIRST_WAIT_MS 100
#endif

#ifndef U_CELL_PRIVATE_UART_WAKE_UP_RETRY_INTERVAL_MS
/** The interval at which to poke the AT interface
 * to wake the module up from UART power saving
 * after the first one; this should be longer than
 * the first wait in case the module is having trouble
 * heaving itself out of bed.
 */
# define U_CELL_PRIVATE_UART_WAKE_UP_RETRY_INTERVAL_MS 333
#endif

/* ----------------------------------------------------------------
 * TYPES
 * -------------------------------------------------------------- */

/** Features of a module that require different compile-time
 * behaviours in this implementation.
 */
//lint -esym(756, uCellPrivateFeature_t) Suppress not referenced,
// Lint can't seem to find it inside macros.
typedef enum {
    U_CELL_PRIVATE_FEATURE_USE_UPSD_CONTEXT_ACTIVATION,
    U_CELL_PRIVATE_FEATURE_MNO_PROFILE,
    U_CELL_PRIVATE_FEATURE_CSCON,
    U_CELL_PRIVATE_FEATURE_ROOT_OF_TRUST,
    U_CELL_PRIVATE_FEATURE_ASYNC_SOCK_CLOSE,
    U_CELL_PRIVATE_FEATURE_SECURITY_C2C,
    U_CELL_PRIVATE_FEATURE_DATA_COUNTERS,
    U_CELL_PRIVATE_FEATURE_SECURITY_TLS_IANA_NUMBERING,
    U_CELL_PRIVATE_FEATURE_SECURITY_TLS_SERVER_NAME_INDICATION,
    U_CELL_PRIVATE_FEATURE_SECURITY_TLS_PSK_AS_HEX,
    U_CELL_PRIVATE_FEATURE_MQTT,
    U_CELL_PRIVATE_FEATURE_MQTT_SARA_R4_OLD_SYNTAX,
    U_CELL_PRIVATE_FEATURE_MQTT_SET_LOCAL_PORT,
    U_CELL_PRIVATE_FEATURE_MQTT_SESSION_RETAIN,
    U_CELL_PRIVATE_FEATURE_MQTT_BINARY_PUBLISH,
    U_CELL_PRIVATE_FEATURE_MQTT_WILL,
    U_CELL_PRIVATE_FEATURE_MQTT_KEEP_ALIVE,
    U_CELL_PRIVATE_FEATURE_MQTT_SECURITY,
    U_CELL_PRIVATE_FEATURE_UCGED5,
    U_CELL_PRIVATE_FEATURE_CONTEXT_MAPPING_REQUIRED,
    U_CELL_PRIVATE_FEATURE_SECURITY_TLS_CIPHER_LIST,
    U_CELL_PRIVATE_FEATURE_AUTO_BAUDING,
    U_CELL_PRIVATE_FEATURE_AT_PROFILES,
    U_CELL_PRIVATE_FEATURE_SECURITY_ZTP,
    U_CELL_PRIVATE_FEATURE_FILE_SYSTEM_TAG,
    U_CELL_PRIVATE_FEATURE_DTR_POWER_SAVING,
    U_CELL_PRIVATE_FEATURE_3GPP_POWER_SAVING,
    U_CELL_PRIVATE_FEATURE_3GPP_POWER_SAVING_PAGING_WINDOW_SET,
    U_CELL_PRIVATE_FEATURE_DEEP_SLEEP_URC,
    U_CELL_PRIVATE_FEATURE_EDRX
} uCellPrivateFeature_t;

/** The characteristics that may differ between cellular modules.
 * Note: order is important since this is statically initialised.
 */
//lint -esym(768, uCellPrivateModule_t::minAwakeTimeSeconds) Suppress
// may not be referenced as references may be conditionally compiled-out.
typedef struct {
    uCellModuleType_t moduleType; /**< The module type. */
    int32_t powerOnPullMs; /**< The time for which PWR_ON must be
                                pulled down to effect power-on. */
    int32_t powerOffPullMs; /**< The time for which PWR_ON must be
                                 pulled down to effect power-off. */
    int32_t bootWaitSeconds; /**< How long to wait before the module is
                                  ready after boot. */
    int32_t minAwakeTimeSeconds; /**< Some modules don't react well
                                      to being powered up and down
                                      again rapidly. This is purely
                                      advisory, used during testing,
                                      which generally involves lots
                                      of powering up and down. */
    int32_t powerDownWaitSeconds; /**< How long to wait for a organised
                                       power-down in the ansence of VInt. */
    int32_t rebootCommandWaitSeconds; /**< How long to wait before the module is
                                           ready after it has been commanded
                                           to reboot. */
    int32_t atTimeoutSeconds; /**< The time to wait for completion of an
                                   AT command, i.e. from sending ATblah to
                                   receiving OK or ERROR back. */
    int32_t commandDelayMs; /**< How long to wait between the end of
                                 one AT command and the start of the
                                 next. */
    int32_t responseMaxWaitMs; /**< The maximum response time one can
                                    expect from the cellular module.
                                    This is usually quite large since,
                                    if there is a URC about to come
                                    through, it can delay what are
                                    normally immediate responses. */
    int32_t radioOffCfun; /**< The type of AT+CFUN state to use to switch
                               the radio off: either 0 for truly off or
                               4 for "airplane" mode. */
    int32_t resetHoldMilliseconds; /**< How long the reset line has to
                                        be held for to reset the cellular
                                        module. */
    size_t maxNumSimultaneousRats; /**< The maximum number of
                                        simultaneous RATs that are
                                        supported by the cellular
                                        module. */
    uint32_t supportedRatsBitmap; /**< A bit-map of the uCellNetRat_t
                                       values supported by the cellular
                                       module. */
    uint32_t featuresBitmap; /**< a bit-map of the uCellPrivateFeature_t
                                  characteristics of this module. */
} uCellPrivateModule_t;

/** The radio parameters.
 */
typedef struct {
    int32_t rssiDbm;  /**< The RSSI of the serving cell. */
    int32_t rsrpDbm;  /**< The RSRP of the serving cell. */
    int32_t rsrqDb;   /**< The RSRQ of the serving cell. */
    int32_t rxQual;   /**< The RxQual of the serving cell. */
    int32_t cellId;   /**< The cell ID of the serving cell. */
    int32_t earfcn;   /**< The EARFCN of the serving cell. */
} uCellPrivateRadioParameters_t;

/** Structure to hold a network name, MCC/MNC and RAT
 * as part of a linked list.
 */
typedef struct uCellPrivateNet_t {
    char name[U_CELL_NET_MAX_NAME_LENGTH_BYTES];
    int32_t mcc;
    int32_t mnc;
    uCellNetRat_t rat;
    struct uCellPrivateNet_t *pNext;
} uCellPrivateNet_t;

/** Context for the cell loc API.
 */
typedef struct {
    int32_t desiredAccuracyMillimetres;  /**< the accuracy we'd like. */
    int32_t desiredFixTimeoutSeconds;    /**< the timeout on a fix we'd like. */
    bool gnssEnable;                     /**< whether a GNSS chip attached
                                              to the cellular module should
                                              be used in the fix or not. */
    uPortMutexHandle_t
    fixDataStorageMutex;  /**< mutex to protect manipulation of the fix data storage. */
    void *pFixDataStorage;                   /**< pointer to data storage used when establishing a fix. */
    int32_t fixStatus;                       /**< status of a location fix. */
} uCellPrivateLocContext_t;

/** Type to keep track of the deep sleep state.
 */
//lint -esym(769, uCellPrivateDeepSleepState_t::U_CELL_PRIVATE_MAX_NUM_SLEEP_STATES) Suppress not referenced
typedef enum {
    U_CELL_PRIVATE_DEEP_SLEEP_STATE_UNKNOWN,
    U_CELL_PRIVATE_DEEP_SLEEP_STATE_UNAVAILABLE, /**< Deep sleep is not available, deep sleep is not possible. */
    U_CELL_PRIVATE_DEEP_SLEEP_STATE_AVAILABLE,   /**< Deep sleep is available, could sleep at any time. */
    U_CELL_PRIVATE_DEEP_SLEEP_STATE_PROTOCOL_STACK_ASLEEP,   /**< +UUPSMR: 1 has been received. */
    U_CELL_PRIVATE_DEEP_SLEEP_STATE_ASLEEP,      /**< VInt is "off", the module is in deep sleep. */
    U_CELL_PRIVATE_MAX_NUM_SLEEP_STATES
} uCellPrivateDeepSleepState_t;

/** Structure to keep track of all things deep sleep related.
 */
typedef struct {
    // *INDENT-OFF* (otherwise AStyle makes a mess of this)
    bool powerSaving3gppAgreed; /**< 3GPP power saving has been agreed with the network. */
    bool powerSaving3gppOnNotOffCereg; /**< Whether 3GPP power saving is on or off according to the +CEREG URC. */
    int32_t activeTimeSecondsCereg; /**< The assigned active time according to the +CEREG URC. */
    int32_t periodicWakeupSecondsCereg; /**< The assigned periodic wake-up time according to the +CEREG URC. */
    void (*p3gppPowerSavingCallback) (int32_t, bool, int32_t, int32_t, void *); /**< User callback called when +CEREG is seen. */
    void *p3gppPowerSavingCallbackParam; /**< User parameter to p3gppPowerSavingCallback. */
    void (*pEDrxCallback) (int32_t, uCellNetRat_t, bool, int32_t, int32_t, int32_t, void *); /**< User callback called when E-DRX parameters changes. */
    void *pEDrxCallbackParam; /**< User parameter to pEDrxCallback. */
    void (*pWakeUpCallback) (int32_t, void *); /**< A callback that can be called when a module is awoken from deep sleep. */
    void *pWakeUpCallbackParam; /**< Parameter provided by the user and passed to pWakeUpCallback when called. */
    // *INDENT-ON*
} uCellPrivateSleep_t;

/** Definition of a cellular instance.
 */
typedef struct uCellPrivateInstance_t {
    int32_t handle; /**< The handle for this instance. */
    const uCellPrivateModule_t *pModule; /**< Pointer to the module type. */
    uAtClientHandle_t atHandle; /**< The AT client handle to use. */
    int32_t pinEnablePower; /**< The pin that switches on the
                                 power supply to the cellular module. */
    int32_t pinPwrOn;       /**< The pin that is conneted to the
                                 PWR_ON pin of the cellular module. */
    int32_t pinVInt;        /**< The pin that is connected to the
                                 VINT pin of the cellular module. */
    int32_t pinDtrPowerSaving; /**< The pin that is connected to the
                                    cellular module's DTR pin, ONLY used
                                    for UPSV mode 3, -1 otherwise. */
    char mccMnc[U_CELL_NET_MCC_MNC_LENGTH_BYTES]; /**< The MCC MNC if manual
                                                       network selection has
                                                       been requested (set
                                                       to zeroes for automatic
                                                       mode). */
    int64_t lastCfunFlipTimeMs; /**< The last time a flip of state from
                                     "off" (AT+CFUN=0/4) to "on" (AT+CFUN=1)
                                     or back was performed. */
    uCellNetStatus_t
    networkStatus[U_CELL_NET_REG_DOMAIN_MAX_NUM]; /**< Registation status in each domain. */
    uCellNetRat_t rat[U_CELL_NET_REG_DOMAIN_MAX_NUM];  /**< The active RAT for each domain. */
    uCellPrivateRadioParameters_t radioParameters; /**< The radio parameters. */
    int64_t startTimeMs;     /**< Used while connecting and scanning. */
    int64_t connectedAtMs;   /**< When a connection was last established,
                                  can be used for offsetting from that time;
                                  does NOT mean that we are currently connected. */
    bool rebootIsRequired;   /**< Set to true if a reboot of the module is
                                  required, e.g. as a result of a configuration
                                  change. */
    bool (*pKeepGoingCallback) (int32_t);  /**< Used while connecting. */
    void (*pRegistrationStatusCallback) (uCellNetRegDomain_t, uCellNetStatus_t, void *);
    void *pRegistrationStatusCallbackParameter;
    void (*pConnectionStatusCallback) (bool, void *);
    void *pConnectionStatusCallbackParameter;
    uCellPrivateNet_t *pScanResults;    /**< Anchor for list of network scan results. */
    void *pSecurityC2cContext;  /**< Hook for a chip to chip security context. */
    volatile void *pMqttContext; /**< Hook for MQTT context, volatile as it
                                      can be populared by a URC in a different thread. */
    uCellPrivateLocContext_t *pLocContext; /**< Hook for a location context. **/
    bool socketsHexMode; /**< Set to true for sockets to use hex mode. */
    const char *pFileSystemTag; /**< The tagged area of the file system currently being addressed. */
    uCellPrivateDeepSleepState_t deepSleepState; /**< The current deep sleep state. */
    bool inWakeUpCallback; /**< So that we can avoid recursion. */
    uCellPrivateSleep_t *pSleepContext; /**< Context for sleep stuff. */
    struct uCellPrivateInstance_t *pNext;
} uCellPrivateInstance_t;

/* ----------------------------------------------------------------
 * VARIABLES
 * -------------------------------------------------------------- */

/** The characteristics of the supported module types, compiled
 * into the driver.
 */
extern const uCellPrivateModule_t gUCellPrivateModuleList[];

/** Number of items in the gUCellPrivateModuleList array.
 */
extern const size_t gUCellPrivateModuleListSize;

/** Root for the linked list of instances.
 */
extern uCellPrivateInstance_t *gpUCellPrivateInstanceList;

/** Mutex to protect the linked list.
 */
extern uPortMutexHandle_t gUCellPrivateMutex;

/* ----------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------- */

/** Return true if the given buffer contains only numeric
 * characters (i.e. 0 to 9).
 *
 * @param pBuffer     pointer to the buffer.
 * @param bufferSize  number of characters at pBuffer.
 * @return            true if all the characters in pBuffer are
 *                    numeric characters, else false.
 */
bool uCellPrivateIsNumeric(const char *pBuffer, size_t bufferSize);

/** Find a cellular instance in the list by instance handle.
 * Note: gUCellPrivateMutex should be locked before this is called.
 *
 * @param handle  the instance handle.
 * @return        a pointer to the instance.
 */
uCellPrivateInstance_t *pUCellPrivateGetInstance(int32_t handle);

/** Set the radio parameters back to defaults.
 *
 * @param pParameters pointer to a radio parameters structure.
 */
void uCellPrivateClearRadioParameters(uCellPrivateRadioParameters_t *pParameters);

/** Clear the dynamic parameters of an instance, so the network
 * status, the active RAT and the radio parameters.  This should
 * be called when the module is being rebooted or powered off.
 *
 * @param pInstance a pointer to the instance.
 */
void uCellPrivateClearDynamicParameters(uCellPrivateInstance_t *pInstance);

/** Get the current AT+CFUN mode of the module.
 *
 * @param pInstance  pointer to the cellular instance.
 * @return           the AT+CFUN mode or negative error code.
 */
int32_t uCellPrivateCFunGet(const uCellPrivateInstance_t *pInstance);

/** Ensure that a module is powerered up if it isn't already
 * and return the AT+CFUN mode it was originally in so that
 * uCellPrivateCFunMode() can be called subseqently to put it
 * back again.
 *
 * @param pInstance  pointer to the cellular instance.
 * @return           the previous mode or negative error code.
 */
int32_t uCellPrivateCFunOne(uCellPrivateInstance_t *pInstance);

/** Do the opposite of uCellPrivateCFunOne(), put the mode back.
 *
 * @param pInstance  pointer to the cellular instance.
 * @param mode       the AT+CFUN mode to set.
 */
void uCellPrivateCFunMode(uCellPrivateInstance_t *pInstance,
                          int32_t mode);

/** Get the IMSI of the SIM.
 * Note: gUCellPrivateMutex should be locked before this is called.
 *
 * @param pInstance  a pointer to the cellular instance.
 * @param pImsi      a pointer to 15 bytes in which the IMSI
 *                   will be stored.
 * @return           zero on success else negative error code.
 */
int32_t uCellPrivateGetImsi(const uCellPrivateInstance_t *pInstance,
                            char *pImsi);

/** Get the IMEI of the module.
 * Note: gUCellPrivateMutex should be locked before this is called.
 *
 * @param pInstance  a pointer to the cellular instance.
 * @param pImei      a pointer to 15 bytes in which the IMEI
 *                   will be stored.
 * @return           zero on success else negative error code.
 */
int32_t uCellPrivateGetImei(const uCellPrivateInstance_t *pInstance,
                            char *pImei);

/** Get whether the given instance is registered with the network.
 * Note: gUCellPrivateMutex should be locked before this is called.
 *
 * @param pInstance  a pointer to the cellular instance.
 * @return           true if it is registered, else false.
 */
bool uCellPrivateIsRegistered(const uCellPrivateInstance_t *pInstance);

/** Convert the module's RA numbering to our RAT numbering.
 *
 * @param moduleType  the module type (since the numbering is different
 *                    in some cases).
 * @param moduleRat   the RAT number used by the module.
 * @return            the RAT number in ubxlib numbering.
 */
uCellNetRat_t uCellPrivateModuleRatToCellRat(uCellModuleType_t moduleType,
                                             int32_t moduleRat);

/** Get the active RAT.
 * Note: gUCellPrivateMutex should be locked before this is called.
 *
 * @param pInstance  a pointer to the cellular instance.
 * @return           the active RAT.
 */
uCellNetRat_t uCellPrivateGetActiveRat(const uCellPrivateInstance_t *pInstance);

/** Get the operator name.
 *
 * @param pInstance   a pointer to the cellular instance.
 * @param pStr        a pointer to size bytes of storage into which
 *                    the operator name will be copied.  Room
 *                    should be allowed for a null terminator, which
 *                    will be added to terminate the string.  This
 *                    pointer cannot be NULL.
 * @param size        the number of bytes available at pStr, including
 *                    room for a null terminator. Must be greater
 *                    than zero.
 * @return            on success, the number of characters copied into
 *                    pStr NOT including the terminator (i.e. as
 *                    strlen() would return), on failure negative
 *                    error code.
 */
int32_t uCellPrivateGetOperatorStr(const uCellPrivateInstance_t *pInstance,
                                   char *pStr, size_t size);

/** Free network scan results.
 *
 * @param ppScanResults a pointer to the pointer to the scan results
 */
void uCellPrivateScanFree(uCellPrivateNet_t **ppScanResults);

/** Get the module characteristics for a given instance.
 *
 * @param handle  the instance handle.
 * @return        a pointer to the module characteristics.
 */
//lint -esym(714, pUCellPrivateGetModule) Suppress lack of a reference
//lint -esym(759, pUCellPrivateGetModule) etc. since use of this function
//lint -esym(765, pUCellPrivateGetModule) may be compiled-out in various ways
const uCellPrivateModule_t *pUCellPrivateGetModule(int32_t handle);

/** Remove the chip to chip security context for the given instance.
 * Note: gUCellPrivateMutex should be locked before this is called.
 *
 * @param pInstance   a pointer to the cellular instance.
 */
void uCellPrivateC2cRemoveContext(uCellPrivateInstance_t *pInstance);

/** Remove the location context for the given instance.
 * Note: gUCellPrivateMutex should be locked before this is called.
 *
 * @param pInstance   a pointer to the cellular instance.
 */
void uCellPrivateLocRemoveContext(uCellPrivateInstance_t *pInstance);

/** Remove the sleep context for the given instance.
 * Note: gUCellPrivateMutex should be locked before this is called.
 *
 * @param pInstance   a pointer to the cellular instance.
 */
void uCellPrivateSleepRemoveContext(uCellPrivateInstance_t *pInstance);

/** [Re]attach a PDP context to an internal module profile.  This
 * is required by some module types (e.g. SARA-R4 and SARA-R5 modules)
 * when a PDP context is either first established or has been lost, e.g.
 * due to network coverage issues or sleep, and then has been regained
 * once more.  The profile used internally to the module for sockets
 * connections, MQTT, etc. is NOT automatically reattached to the regained
 * context.
 *
 * @param pInstance   a pointer to the cellular instance.
 * @param contextId   the ID for the PDP context.
 * @param profileId   the ID of the profile to associate with the PDP context.
 * @param tries       the number of times to try doing this, should be at
 *                    least 1.
 * @param pKeepGoing  a callback which should return true if the profile
 *                    activation process is to continue, or can be NULL.
 * @return            zero on success else negative error code.
 */
int32_t uCellPrivateActivateProfile(const uCellPrivateInstance_t *pInstance,
                                    int32_t contextId, int32_t profileId, size_t tries,
                                    bool (*pKeepGoing) (const uCellPrivateInstance_t *));

/** Determine whether deep sleep is active, i.e. VInt has gone
 * low; the +UUPSMR URC doesn't count here, it's only actual deep sleep
 * that we care about.
 *
 * @param pInstance  a pointer to the cellular instance.
 * @return           true if the deep sleep is active, else false.
 */
bool uCellPrivateIsDeepSleepActive(uCellPrivateInstance_t *pInstance);

/** Callback to wake up the cellular module from power saving.
 *
 * @param atHandle   the handle of the AT client that is talking
 *                   to the module.
 * @param pInstance  the parameter for the callback, should be a
 *                   pointer to the instance data.
 * @return           zero on successful wake-up, else negative error.
 */
int32_t uCellPrivateWakeUpCallback(uAtClientHandle_t atHandle,
                                   void *pInstance);

/** Determine the deep sleep state.  This is not at all straightforward.
 * If deep sleep is supported then a check is made as to whether the
 * 3GPP sleep or E-DRX parameters have been set.  If they are then it may
 * be possible to go to sleep if an EUTRAN RAT is in the list of supported
 * RATs.  Something like that anyway.  This should be called after
 * power-on and after a RAT change; it doesn't talk to the module,
 * simply works on the current state of the module as known to this code.
 * Note: gUCellPrivateMutex should be locked before this is called.
 *
 * @param pInstance a pointer to the cellular instance.
 */
void uCellPrivateSetDeepSleepState(uCellPrivateInstance_t *pInstance);

#ifdef __cplusplus
}
#endif

#endif // _U_CELL_PRIVATE_H_

// End of file
