#include <zephyr/kernel.h>
#include <zephyr/pm/policy.h>
#include <zephyr/sys/time_units.h>
#include <zephyr/sys_clock.h>
#include <zephyr/ztest.h>
#include <zephyr/logging/log.h>
#include <aon_reg.h>

LOG_MODULE_REGISTER(pm_wfi_wakeup_latency);

/*
 * 专门用于测量硬件 WFI 到 Timer ISR 唤醒耗时的测试。
 * 依赖于 Zephyr drivers/timer/realtek_grtc_timer.c 中的 g_hw_latency_cyc 和 g_sw_latency_cyc 变量。
 */
#ifdef WFI_WAKEUP_LATENCY_DEBUG
extern uint64_t g_hw_latency_cyc;
extern uint64_t g_sw_latency_cyc;
#endif

/*
 * 测试周期列表: 20000us 和 10000us 用于满足深睡门槛，触发实际深度休眠并测量耗时。
 * 2000us 作为对照组观察未进入深睡时的延迟。
 */
static const uint32_t period_list_us[] = {20000, 10000, 2000};
#define PERIOD_CNT (ARRAY_SIZE(period_list_us))

#define EXPIRE_PER_PERIOD 5
#define TOTAL_EXPIRE_CNT  (PERIOD_CNT * EXPIRE_PER_PERIOD)

struct timer_log_t {
	uint32_t period_us;
	uint64_t hw_latency_cyc;
	uint64_t sw_latency_cyc;
	int low_power_mode_count;
};

static struct timer_log_t log_buf[TOTAL_EXPIRE_CNT];
static volatile uint16_t log_write_pos;

static struct k_timer periodic_timer;
static struct k_sem periodic_sem;

static uint8_t curr_period_idx;
static uint8_t expire_cnt_in_period;

static void timer_period_fn(struct k_timer *t)
{
	expire_cnt_in_period++;

	if (log_write_pos < TOTAL_EXPIRE_CNT) {
		log_buf[log_write_pos].period_us = period_list_us[curr_period_idx];
#ifdef WFI_WAKEUP_LATENCY_DEBUG
		log_buf[log_write_pos].hw_latency_cyc = g_hw_latency_cyc;
		log_buf[log_write_pos].sw_latency_cyc = g_sw_latency_cyc;
#endif
		log_buf[log_write_pos].low_power_mode_count = AON_REG_READ_BITFIELD(AON_REG_PCK600_AON_REG5X, VPON_LEAVE_FUNC_RET_MODE_CNT_VALUE);
		log_write_pos++;
	}

	if (expire_cnt_in_period >= EXPIRE_PER_PERIOD) {
		k_timer_stop(&periodic_timer);
		expire_cnt_in_period = 0;
		curr_period_idx++;
		k_sem_give(&periodic_sem);
	}
}

ZTEST(wfi_wakeup_latency, test_latency)
{
	TC_PRINT("Start Realtek HW Latency test\n");
	TC_PRINT("Note: Please ensure exit-latency-us = <0> in DTS to get raw hardware latency.\n");

	curr_period_idx = 0;
	expire_cnt_in_period = 0;
	log_write_pos = 0;

	k_timer_init(&periodic_timer, timer_period_fn, NULL);

	for(; curr_period_idx < PERIOD_CNT;) {
		uint32_t next_us = period_list_us[curr_period_idx];
		TC_PRINT("Testing period: %d us\n", next_us);
		k_timer_start(&periodic_timer, K_USEC(next_us), K_USEC(next_us));
		k_sem_take(&periodic_sem, K_FOREVER);
	}

	TC_PRINT("\n--- Hardware Latency Measurement Report ---\n");
	for (uint16_t i = 0; i < log_write_pos; i++) {
		const struct timer_log_t *l = &log_buf[i];
		uint32_t hw_latency_us = (uint32_t)(l->hw_latency_cyc * 1000000ULL / CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC);
		uint32_t sw_latency_us = (uint32_t)(l->sw_latency_cyc * 1000000ULL / CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC);
		uint32_t total_latency_us = hw_latency_us + sw_latency_us;

		printk("Period: %5u us | Total: %4llu cyc -> %4u us | HW: %4llu cyc -> %4u us | SW: %4llu cyc -> %4u us | Sleep Count: %d\n",
		       l->period_us, l->hw_latency_cyc + l->sw_latency_cyc, total_latency_us,
			   l->hw_latency_cyc, hw_latency_us, l->sw_latency_cyc, sw_latency_us, l->low_power_mode_count);
	}
	TC_PRINT("-------------------------------------------\n");
}

static void before_fn(void *data)
{
	k_sem_init(&periodic_sem, 0, 1);
}

static void teardown_fn(void *data)
{
	pm_policy_state_lock_get(PM_STATE_SUSPEND_TO_IDLE, 0);
}

ZTEST_SUITE(wfi_wakeup_latency, NULL, NULL, before_fn, NULL, teardown_fn);
