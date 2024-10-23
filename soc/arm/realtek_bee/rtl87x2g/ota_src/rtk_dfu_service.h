/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      dfu_service.h
   * @brief     Head file for using OTA service
   * @author    Grace
   * @date      2023-12-06
   * @version   v1.1
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2023 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _DFU_SERVICE_H_
#define _DFU_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include "app_msg.h"

/** @defgroup  DFU_SERVICE DFU Service
  * @brief DFU Service to implement DFU feature
  * @{
  */
/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup DFU_SERVICE_Exported_Macros DFU service Exported Macros
  * @brief
  * @{
  */
#define  BLE_GATT_UUID128_DFU_SERVICE       0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x62, 0x00, 0x00
#define  BLE_GATT_UUID128_DFU_PACKET        0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x63, 0x00, 0x00
#define  BLE_GATT_UUID128_DFU_CONTROL_POINT 0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x64, 0x00, 0x00

#define BT_UUID_CHAR_DFU_PACKET BT_UUID_DECLARE_128(BLE_GATT_UUID128_DFU_PACKET)
#define BT_UUID_CHAR_DFU_CONTROL_POINT BT_UUID_DECLARE_128(BLE_GATT_UUID128_DFU_CONTROL_POINT)


#define INDEX_DFU_CHAR_DFU_PACKET_INDEX           0x2
#define INDEX_DFU_CHAR_DFU_CONTROL_POINT_INDEX    0x4
#define INDEX_DFU_CHAR_DFU_CP_CCCD_INDEX          0x5


#define DFU_CP_NOTIFY_ENABLE    0x1
#define DFU_CP_NOTIFY_DISABLE   0x2

/** End of DFU_SERVICE_Exported_Macros
  * @}
  */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup DFU_SERVICE_Exported_Types DFU Service Exported Types
  * @brief
  * @{
  */

typedef enum
{
    DFU_FAIL_UPDATE_FLASH,
    DFU_FAIL_SYSTEM_RESET_CMD,
    DFU_FAIL_EXCEED_MAX_BUFFER_SIZE,
    DFU_FAIL_EXCEED_IMG_TOTAL_LEN,
} T_DFU_FAIL_REASON;


typedef enum
{
    DFU_WRITE_ATTR_ENTER,
    DFU_WRITE_ATTR_EXIT,
    DFU_WRITE_START,
    DFU_WRITE_DOING,
    DFU_WRITE_END,
    DFU_WRITE_FAIL,
} T_DFU_WRITE_OPCODE;


typedef struct
{
    uint8_t opcode;
    uint8_t write_attrib_index;
    uint16_t length;
    uint8_t *p_value;
} T_DFU_WRITE_MSG;

/** @brief  OTA upstream message data */
typedef union
{
    uint8_t notification_indification_index;
    T_DFU_WRITE_MSG write;
} T_DFU_UPSTREAM_MSG_DATA;

/** @brief Event type to inform app*/
typedef enum
{
    SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION = 1,    /**< CCCD update event. */
    SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE = 2,              /**< client read event. */
    SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE = 3,             /**< client write event. */
} T_SERVICE_CALLBACK_TYPE;

/** Dfu service data to inform application */
typedef struct
{
    T_SERVICE_CALLBACK_TYPE  msg_type;
    uint8_t                  conn_id;
#if F_APP_GATT_SERVER_EXT_API_SUPPORT
    uint16_t                    conn_handle;//gatt extend api? conn_handle?cid
    uint16_t                    cid;
#endif
    T_DFU_UPSTREAM_MSG_DATA  msg_data;
} T_DFU_CALLBACK_DATA;

/** End of DFU_SERVICE_Exported_Types
  * @}
  */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup DFU_SERVICE_Exported_Functions DFU service Exported Functions
  * @brief
  * @{
  */

/**
    * @brief    Add DFU BLE service to application
    * @param    p_func  Pointer of APP callback function called by profile
    * @return   A T_SERVER_ID type value, Service ID auto generated by profile layer
    */
// T_SERVER_ID dfu_add_service(void *p_func);

/**
    * @brief    Send notification to peer side
    * @param    conn_id  PID to identify the connection
    * @param    p_data  value to be send to peer
    * @param    data_len  data length of the value to be send
    * @return   void
    */
void dfu_service_send_notification(struct bt_conn *conn, uint8_t *p_data, uint16_t data_len);

/** End of DFU_SERVICE_Exported_Functions
  * @}
  */

/** End of DFU_SERVICE
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
