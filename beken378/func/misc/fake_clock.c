#include "include.h"
#include "fake_clock_pub.h"
#include "pwm_pub.h"
#include "icu_pub.h"
#include "drv_model_pub.h"
#include "uart_pub.h"

#if CFG_SUPPORT_ALIOS
#include "rtos_pub.h"
#include "ll.h"
#include "k_api.h"
#else
#include "sys_rtos.h"
#include "arm_arch.h"
#endif
#include "power_save_pub.h"

#if CFG_BK7221_MDM_WATCHDOG_PATCH
void rc_reset_patch(void);
#endif

static volatile uint32_t current_clock = 0;
static volatile uint32_t current_seconds = 0;
static uint32_t second_countdown = FCLK_SECOND;
static uint32_t fclk_vernier_cnt = 0;          /*max value: FCLK_CHUNK_COUNT*/
fclk_user_cb fclk_callback = NULL;

uint32_t fclk_register_cb(fclk_user_cb cb)
{
	fclk_callback = cb;

	return 0;
}

void fclk_clear_vernier_cnt(void)
{
	fclk_vernier_cnt = 0;
}

uint32_t fclk_adjust_vernier_cnt(void)
{
	fclk_vernier_cnt += 1;

	return fclk_vernier_cnt;
}

void fclk_tick_handler(UINT8 param)
{
	if(FCLK_CHUNK_COUNT == fclk_adjust_vernier_cnt())
	{
		fclk_clear_vernier_cnt();
		
		fclk_hdl(param);
	}
}

void fclk_pwm_hdl(UINT8 param)
{
	if(fclk_callback)
	{
		(*fclk_callback)();
	}

	fclk_tick_handler(param);
}

void fclk_hdl(UINT8 param)
{
    GLOBAL_INT_DECLARATION();
#if CFG_USE_STA_PS
    if(power_save_use_pwm_isr())
    {
        power_save_pwm_isr(param);
        return;
    }
#endif

    current_clock ++;

#if CFG_BK7221_MDM_WATCHDOG_PATCH
    rc_reset_patch();
#endif

#if (!CFG_SUPPORT_ALIOS)
    /* Increment the tick counter. */
    GLOBAL_INT_DISABLE();
    if( xTaskIncrementTick() != pdFALSE )
    {
        /* Select a new task to run. */
        vTaskSwitchContext();
    }
    GLOBAL_INT_RESTORE();
#endif
    if (--second_countdown == 0)
    {
        current_seconds ++;
        second_countdown = FCLK_SECOND;
    }
#ifdef BEKEN_START_WDT
	/* fresh watch dog if needed */
	if(current_seconds % 5)
	{
		bk_wdg_reload();
	}
#endif

#if CFG_SUPPORT_ALIOS
    krhino_tick_proc();
#endif
}

UINT32 fclk_update_tick(UINT32 tick)
{
    current_clock += tick;

    if(second_countdown <= tick)
    {
        current_seconds ++;
        second_countdown = FCLK_SECOND - (tick - second_countdown);
    }
    else
    {
        second_countdown -= tick;
    }

    return 0;
}

UINT32 fclk_get_tick(void)
{
    return current_clock;
}

UINT32 fclk_get_second(void)
{
    return current_seconds;
}

UINT32 fclk_from_sec_to_tick(UINT32 sec)
{
    return sec * FCLK_SECOND;
}

void fclk_reset_count(void)
{
    current_clock = 0;
    current_seconds = 0;
}

UINT32 fclk_cal_endvalue(UINT32 mode)
{
    UINT32 value = 1;

    if(PWM_CLK_32K == mode)
    {
        /*32k clock*/
        value = FCLK_PWM_DURATION * 32;
    }
    else if(PWM_CLK_26M == mode)
    {
        /*26m clock*/
#if CFG_SUPPORT_ALIOS
	value = 26000000 / RHINO_CONFIG_TICKS_PER_SECOND;
#else
        value = FCLK_PWM_DURATION * 26000;
#endif
    }

    return value;
}

void fclk_init(void)
{
    UINT32 ret;
    pwm_param_t param;

    /*init pwm*/
    param.channel         = FCLK_PWM_ID;
    param.cfg.bits.en     = PWM_ENABLE;
    param.cfg.bits.int_en = PWM_INT_EN;
    param.cfg.bits.mode   = PMODE_TIMER;

#if(CFG_RUNNING_PLATFORM == FPGA_PLATFORM)  // FPGA:PWM0-2-32kCLK, pwm3-5-24CLK
    param.cfg.bits.clk    = PWM_CLK_32K;
#else
    param.cfg.bits.clk    = PWM_CLK_26M;
#endif

    param.p_Int_Handler   = fclk_pwm_hdl;
    param.duty_cycle      = 0;
    param.end_value       = fclk_cal_endvalue((UINT32)param.cfg.bits.clk);

    ret = sddev_control(PWM_DEV_NAME, CMD_PWM_INIT_PARAM, &param);
    ASSERT(PWM_SUCCESS == ret);
}

// eof

