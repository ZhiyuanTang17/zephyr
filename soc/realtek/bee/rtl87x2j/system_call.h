/**
*****************************************************************************************
*     Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    system_call.h
  * @brief
  * @details
  * @author
  * @date    3. March 2024
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2024 Realtek Semiconductor Corporation</center></h2>
   * ************************************************************************************
   */

/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H


/*============================================================================*
 *                      Headers
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


/*============================================================================*
 *                      Types
 *============================================================================*/



/*============================================================================*
 *                      Variables
 *============================================================================*/
extern void (*patch_syscall)(uint32_t opcode, uint32_t parm);
extern bool (*patch_syscall_platform)(uint32_t opcode, va_list argp);
extern bool (*patch_syscall_bt_host_le)(uint32_t opcode, va_list argp);
extern bool (*patch_syscall_bt_host_common)(uint32_t opcode, va_list argp);
extern bool (*patch_syscall_bt_controller)(uint32_t opcode, va_list argp);

/*============================================================================*
 *                      Functions
 *============================================================================*/
void syscall(uint32_t opcode, uint32_t parm);

void syscall_platform(uint32_t opcode, ...);

void syscall_bt_host_le(uint32_t opcode, ...);

void syscall_bt_host_common(uint32_t opcode, ...);

void syscall_bt_controller(uint32_t opcode, ...);




#ifdef __cplusplus
}
#endif


#endif /* SYSTEM_CALL_H */

