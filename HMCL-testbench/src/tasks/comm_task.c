#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "comm_task.h"
#include "common.h"

static void handle_command(char *cmd);
static void blink_led();
static void print_help();

const char* sensorNames[] = {
    "NTC_TEMP1",
    "NTC_TEMP2",
    "NTC_TEMP3",
    "CURR_HR",
    "CURR_LR",
    "PDU_TEMP"
};

void system_init()
{
    stdio_init_all();
    sleep_ms(1000);									// Allow USB serial to initialize

//    gpio_init(LED_PIN);
//    gpio_set_dir(LED_PIN, GPIO_OUT);
}

static void blink_led()
{
	gpio_put(LED_PIN, 1);
	sleep_ms(100);
	gpio_put(LED_PIN, 0);
}

void vTaskCliComm(__unused void *params)
{
    system_init();
	char cli_cmd[CLI_BUFFER_LEN];
	int idx = 0;

	printf("=====Welcome to CLI interface=====\n");
    while (1) {
		int c = getchar_timeout_us(0);				// Non-blocking read call
		if (c != PICO_ERROR_TIMEOUT)
        {
			printf("%c", c);
//			blink_led();							// blink led after reading a char

			if (c == '\r' || c == '\n') {
				cli_cmd[idx] = '\0';
				if (idx > 0) {
					handle_command(cli_cmd);
					idx = 0;
                    printf("\n> ");
				}
			} else if (c == 0x7F || c == '\b') {	// Backspace
				if (idx > 0) idx--;
			} else if (idx < CLI_BUFFER_LEN - 1) {
				cli_cmd[idx++] = (char)c;
			}
		}
		vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void print_help()
{
    printf("Available commands:\n");
    printf("  help   - Show this message\n");
    printf("  show   - Show current lookup values\n");
    printf("  set x y - Set value for sensor id=x\n");
    printf("Available sensors:\n");
    printf("\t1. NTC_TEMP1\n");
    printf("\t2. NTC_TEMP2\n");
    printf("\t3. NTC_TEMP3\n");
    printf("\t4. CURR_HR\n");
    printf("\t5. CURR_LR\n");
    printf("\t6. PDU_TEMP\n");
}

static void handle_command(char *cmd)
{
	if (strcmp(cmd, "help") == 0) {
        print_help();
	} else if (strncmp(cmd, "set ", 4) == 0) {
		int id; 
		float val;
		if (sscanf(cmd + 4, "%d %f", &id, &val) == 2) {
			printf("Set sensor %d-%s with value %.2f\n", id, sensorNames[id - 1], val);
		} else {
			printf("Usage: set <id> <value>\n");
		}
	} else if (strcmp(cmd, "show") == 0) {
		printf("TODO: implement show current sensors value\n");
	} else {
		printf("Unknown command: %s\n", cmd);
        print_help();
	}
}
