/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "Arduino_interface.h"
#include "HardwareSerial.h"
#include "SimpleFOC.h"
#include "common/base_classes/Sensor.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/timer.h"
#include "hardware/uart.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#include "FreeRTOS.h"
#include "task.h"

// Which core to run on if configNUMBER_OF_CORES==1
#ifndef RUN_FREE_RTOS_ON_CORE
#define RUN_FREE_RTOS_ON_CORE 0
#endif

// Whether to flash the led
#ifndef USE_LED
#define USE_LED 1
#endif

// Whether to busy wait in the led thread
#ifndef LED_BUSY_WAIT
#define LED_BUSY_WAIT 0
#endif

// Delay between led blinking
#define LED_DELAY_MS 200

// Priorities of our threads - higher numbers are higher priority
#define MAIN_TASK_PRIORITY (tskIDLE_PRIORITY + 2UL)
#define BLINK_TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)
#define WORKER_TASK_PRIORITY (tskIDLE_PRIORITY + 4UL)

// Stack sizes of our threads in words (4 bytes)
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define WORKER_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#include "pico/async_context_freertos.h"
static async_context_freertos_t async_context_instance;

// Create an async context
static async_context_t* create_async_context(void) {
    async_context_freertos_config_t config = async_context_freertos_default_config();
    config.task_priority = WORKER_TASK_PRIORITY;      // defaults to ASYNC_CONTEXT_DEFAULT_FREERTOS_TASK_PRIORITY
    config.task_stack_size = WORKER_TASK_STACK_SIZE;  // defaults to ASYNC_CONTEXT_DEFAULT_FREERTOS_TASK_STACK_SIZE
#if configSUPPORT_STATIC_ALLOCATION
    static StackType_t async_context_freertos_task_stack[WORKER_TASK_STACK_SIZE];
    config.task_stack = async_context_freertos_task_stack;
#endif
    if (!async_context_freertos_init(&async_context_instance, &config)) return NULL;
    return &async_context_instance.core;
}

#if USE_LED
// Turn led on or off
static void set_led(bool led_on) {
#if defined PICO_DEFAULT_LED_PIN
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

// Initialise led
static void init_led(void) {
#if defined PICO_DEFAULT_LED_PIN
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    hard_assert(cyw43_arch_init() == PICO_OK);
    set_led(false);  // make sure cyw43 is started
#endif
}

void blink_task(__unused void* params) {
    bool on = false;
    printf("blink_task starts\n");
    init_led();
    while (true) {
#if configNUMBER_OF_CORES > 1
        static int last_core_id = -1;
        if (portGET_CORE_ID() != last_core_id) {
            last_core_id = portGET_CORE_ID();
            printf("blink task is on core %d\n", last_core_id);
        }
#endif
        set_led(on);
        on = !on;

#if LED_BUSY_WAIT
        // You shouldn't usually do this. We're just keeping the thread busy,
        // experiment with BLINK_TASK_PRIORITY and LED_BUSY_WAIT to see what happens
        // if BLINK_TASK_PRIORITY is higher than TEST_TASK_PRIORITY main_task won't get any free time to run
        // unless configNUMBER_OF_CORES > 1
        busy_wait_ms(LED_DELAY_MS);
#else
        sleep_ms(LED_DELAY_MS);
#endif
    }
}
#endif  // USE_LED

// async workers run in their own thread when using async_context_freertos_t with priority WORKER_TASK_PRIORITY
static void do_work(async_context_t* context, async_at_time_worker_t* worker) {
    async_context_add_at_time_worker_in_ms(context, worker, 1000);
    static uint32_t count = 0;
    // printf("Hello from worker count=%u\n", count++);
#if configNUMBER_OF_CORES > 1
    static int last_core_id = -1;
    if (portGET_CORE_ID() != last_core_id) {
        last_core_id = portGET_CORE_ID();
        printf("worker is on core %d\n", last_core_id);
    }
#endif
}
async_at_time_worker_t worker_timeout = {.do_work = do_work};

void main_task(__unused void* params) {
    async_context_t* context = create_async_context();
    // start the worker running
    async_context_add_at_time_worker_in_ms(context, &worker_timeout, 0);
#if USE_LED
    // start the led blinking
#if configSUPPORT_STATIC_ALLOCATION
    static StackType_t blink_stack[BLINK_TASK_STACK_SIZE];
    static StaticTask_t blink_buf;
    xTaskCreateStatic(blink_task, "BlinkThread", BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, blink_stack, &blink_buf);
#else
    static_assert(configSUPPORT_DYNAMIC_ALLOCATION, "");
    xTaskCreate(blink_task, "BlinkThread", BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);
#endif  // configSUPPORT_STATIC_ALLOCATION
#endif  // USE_LED
    int count = 0;
    arduino_spi_init();
    arduino_serial_init();
    arduino_hi2c_init();
    setup();
    while (true) {
#if configNUMBER_OF_CORES > 1
        static int last_core_id = -1;
        if (portGET_CORE_ID() != last_core_id) {
            last_core_id = portGET_CORE_ID();
            printf("main task is on core %d\n", last_core_id);
        }
#endif
        // printf("Hello from main task count=%u\n", count++);
        vTaskDelay(1);
        loop();
    }
    async_context_deinit(context);
}

void vLaunch(void) {
    TaskHandle_t task;
#if configSUPPORT_STATIC_ALLOCATION
    static StackType_t main_stack[MAIN_TASK_STACK_SIZE];
    static StaticTask_t main_buf;
    task = xTaskCreateStatic(main_task, "MainThread", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, main_stack, &main_buf);
#else
    static_assert(configSUPPORT_DYNAMIC_ALLOCATION, "");
    xTaskCreate(main_task, "MainThread", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);
#endif  // configSUPPORT_STATIC_ALLOCATION
#if configUSE_CORE_AFFINITY && configNUMBER_OF_CORES > 1
    // we must bind the main task to one core (well at least while the init is called)
    vTaskCoreAffinitySet(task, 1);
#else
    (void)task;
#endif

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

// magnetic sensor instance - SPI
// MagneticSensorSPI sensor = MagneticSensorSPI(AS5147_SPI, PIN_CS);
// magnetic sensor instance - MagneticSensorI2C
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

// BLDC motor & driver instance
BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(MOTOR_A, MOTOR_B, MOTOR_C, MOTOR_EN);

// voltage set point variable
float target_voltage = 2;
// instantiate the commander
Commander command = Commander(Serial);
void doTarget(char* cmd) { command.scalar(&target_voltage, cmd); }

void setup(void) {
    // initialise magnetic sensor hardware
    sensor.init();
    // link the motor to the sensor
    motor.linkSensor(&sensor);

    // power supply voltage
    driver.voltage_power_supply = 12;
    driver.init();
    motor.linkDriver(&driver);

    // aligning voltage
    motor.voltage_sensor_align = 5;
    // choose FOC modulation (optional)
    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    // set motion control loop to be used
    motor.controller = MotionControlType::torque;

    // use monitoring with serial
    Serial.begin(115200);
    // comment out if not needed
    motor.useMonitoring(Serial);

    // initialize motor
    motor.init();
    // align sensor and start FOC
    motor.initFOC();

    // add target command T
    command.add('T', doTarget, (char*)"target voltage");

    Serial.println(F("Motor ready."));
    Serial.println(F("Set the target voltage using serial terminal:"));
    _delay(1000);
}

void loop(void) {
    // main FOC algorithm function
    // the faster you run this function the better
    // Arduino UNO loop  ~1kHz
    // Bluepill loop ~10kHz
    motor.loopFOC();

    // Motion control function
    // velocity, position or voltage (defined in motor.controller)
    // this function can be run at much lower frequency than loopFOC() function
    // You can also use motor.move() and set the motor.target in the code
    motor.move(target_voltage);

    // user communication
    command.run();
    // float angle = sensor.getAngle();
    // Serial.println(angle);
}

int main(void) {
    stdio_init_all();

    /* Configure the hardware ready to run the demo. */
    const char* rtos_name;
#if (configNUMBER_OF_CORES > 1)
    rtos_name = "FreeRTOS SMP";
#else
    rtos_name = "FreeRTOS";
#endif

#if (configNUMBER_OF_CORES > 1)
    printf("Starting %s on both cores:\n", rtos_name);
    vLaunch();
#elif (RUN_FREE_RTOS_ON_CORE == 1 && configNUMBER_OF_CORES == 1)
    printf("Starting %s on core 1:\n", rtos_name);
    multicore_launch_core1(vLaunch);
    while (true);
#else
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();
#endif
    return 0;
}
