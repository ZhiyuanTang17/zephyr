/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <clock_manager.h>
#include <system_rtl876x_int.h>
static int rtl8752h_platform_init(void)
{
	/* enable cache */
	extern void share_cache_ram(void);
	share_cache_ram();

	/*Set the active mode clk src.*/
    set_active_mode_clk_src();

	set_up_32k_clk_src();

	/* Check if it is a boot from Power Down Mode or a HW reset.*/
	// AON_FAST_REG_REG0X_FW_GENERAL_TYPE aon_fast_boot = {.d16 = btaon_fast_read(AON_FAST_REG_REG0X_FW_GENERAL)};
	// bool aon_boot_done = aon_fast_boot.aon_boot_done;


// cannot run in this case, might need some dependency.
//	if (!aon_boot_done) {
//		pmu_power_on_sequence_restart();
//	} else {
		/* Exit flow if it is a Wakup flow from Power Down mode.*/
//	}

	/* Set the flag that use to distingush wheather this is a Boot wake up from Power down mode to 1.*/
	// AON_FAST_REG_REG0X_FW_GENERAL_TYPE aon_fast_reg_0x0 = {.d16 = btaon_fast_read(AON_FAST_REG_REG0X_FW_GENERAL)};
	// aon_fast_reg_0x0.aon_boot_done = 1;
	// btaon_fast_write(AON_FAST_REG_REG0X_FW_GENERAL, aon_fast_reg_0x0.d16);
	
	/* Enable Systck 32K clock, vendor register, enable bus clock, init trng, init ram power control.*/
	hal_setup_hardware();

	/* mpu setup */
	hal_setup_cpu();

	/* Set the flag that use to distingush wheather this is a Boot wake up from DLPS to 1.*/
	// AON_FAST_REG_REG0X_FW_GENERAL_TYPE aon_fast_reg_0x0 = {.d16 = btaon_fast_read(AON_FAST_REG_REG0X_FW_GENERAL)};
	// aon_fast_reg_0x0.d16 = btaon_fast_read(AON_FAST_REG_REG0X_FW_GENERAL);
	// aon_fast_reg_0x0.pon_boot_done = 1;
	// btaon_fast_write(AON_FAST_REG_REG0X_FW_GENERAL, aon_fast_reg_0x0.d16);

	return 0;
}

static int rtl8752h_register_update(void)
{
	/* Selects the SysTick timer clock source: external 32768 */
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;

	return 0;
}

SYS_INIT(rtl8752h_platform_init, EARLY, 0);
SYS_INIT(rtl8752h_register_update, PRE_KERNEL_2, 1);
