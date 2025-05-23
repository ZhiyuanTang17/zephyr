/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <os_task.h>
#include <os_sched.h>

#include <stdint.h>

#include <zephyr/drivers/flash.h>
#include <zephyr/devicetree.h>
#include <zephyr/storage/flash_map.h>


#define STACKSZ 2048

uint32_t foo = 2;
/* func3 中触发 HardFault */
void func3(void)
{
    __ASSERT(foo == 0xF0CACC1A, "Invalid value of foo, got 0x%x", foo);
   
    /* 故意写入非法地址：NULL指针 */
    volatile uint32_t *pInvalid = (uint32_t *)0x00000000;
    *pInvalid = 0xDEADBEEF; // 此处将产生访问异常，导致 HardFault
}

/* func2 调用 func3 */
void func2(void)
{
    func3();
}

/* func1 调用 func2 */
void func1(void)
{
    func2();
}


// void thread1(void *argument)
// {
//     func1();
// }
#include <zephyr/drivers/coredump.h>
#include <zephyr/devicetree.h>
#define HEAP_START       DT_REG_ADDR(DT_NODELABEL(heap))
#define HEAP_SIZE        DT_REG_SIZE(DT_NODELABEL(heap))

static struct coredump_mem_region_node heap_region = {
	.start = (uintptr_t)HEAP_START,
	.size = HEAP_SIZE,
};

#define TEST_AREA_DEVICE	FIXED_PARTITION_DEVICE(storage_partition)
static const struct device *const flash_dev = TEST_AREA_DEVICE;

int main(void)
{
	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

    /* 通过一系列调用最终触发 HardFault */
    func1();

    // int rc;
	// rc = flash_erase(flash_dev, 0xf5000, 28672);
    // printk("flash_erace pass: rc=%d\n",rc);


    const struct device *const coredump_dev = DEVICE_DT_GET(DT_NODELABEL(coredump_device));
    bool t = coredump_device_register_memory(coredump_dev, &heap_region);
    printk("res:%d\n",t);

    /* 通过一系列调用最终触发 HardFault */
    func1();

    // void *id1 = NULL;

	// uint32_t task_param;

	// bool status;
	// status = os_task_create(&id1, "task1",  thread1,
    //                     &task_param, STACKSZ, 6);

    // /* 如果没有重启，程序将停在此处 */
    // while (1) {
    //     /* 可在此处做指示灯闪烁或者等待复位 */
    // }

	return 0;
}
