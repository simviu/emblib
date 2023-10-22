#include "emb/esp32/embLib_esp.h"

using namespace ut;

namespace{
    extern "C" bool esp_init_mc(int pin, 
                                MCPWM_dt& mc)
    {

        //----------
        log_i("Create timer and operator");
        mcpwm_timer_handle_t timer = NULL;
        mcpwm_timer_config_t timer_config = {
            .group_id = 0,
            .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
            .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
            .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
            .period_ticks = SERVO_TIMEBASE_PERIOD
        };
        ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

        mcpwm_oper_handle_t oper = NULL;
        mcpwm_operator_config_t operator_config = {
            .group_id = 0, // operator must be in the same group to the timer
        };
        ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

        log_i("Connect timer and operator");
        ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

        log_i("Create comparator and generator from the operator");
        mcpwm_cmpr_handle_t comparator = NULL;

        /*
        mcpwm_comparator_config_t comparator_config = {
            .flags.update_cmp_on_tez = true
        };
        */
        mcpwm_comparator_config_t comparator_config;
        comparator_config.flags.update_cmp_on_tez = true;
        comparator_config.intr_priority = 0; // let sys allocate


        ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));

        mcpwm_gen_handle_t generator = NULL;
        mcpwm_generator_config_t generator_config = {
            .gen_gpio_num = pin //SERVO_PULSE_GPIO
        };
        ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

        // set the initial compare value, so that the servo will spin to the center position
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, esp32_ang2us(0)));
        
        
        log_i("Set generator action on timer and compare event");
        // go high on counter empty
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_timer_event(generator,
                        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH),
                        MCPWM_GEN_TIMER_EVENT_ACTION_END()));
        // go low on compare threshold
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_compare_event(generator,
                        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW),
                        MCPWM_GEN_COMPARE_EVENT_ACTION_END()));

        log_i("Enable and start timer");
        ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
        ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));


        //----- save
        mc.timer = timer;
        mc.oper = oper;
        mc.generator = generator;
        mc.comparator = comparator;
        
        return true;
    }
}

//-----
bool ServoEsp::init()
{
    log_i("Init servo esp on pin "+ to_string(PWM::cfg_.pin) +"...");
    esp_init_mc(PWM::cfg_.pin, mc_);
    return true;
}
//----
        
bool ServoEsp::set_degree(float ang) 
{
    auto& comparator = mc_.comparator;
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, esp32_ang2us(ang)));
    return true;
}
    