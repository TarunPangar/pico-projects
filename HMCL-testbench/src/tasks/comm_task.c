#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "comm_task.h"
#include "common.h"

static void handle_command(char *cmd);
static void blink_led();

void system_init()
{
    stdio_init_all();
    sleep_ms(1000);									// Allow USB serial to initialize

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
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
			blink_led();							// blink led after reading a char

			if (c == '\r' || c == '\n') {
				cli_cmd[idx] = '\0';
				if (idx > 0) {
					handle_command(cli_cmd);
					idx = 0;
				}
			} else if (c == 0x7F || c == '\b') {	// Backspace
				if (idx > 0) {
					idx--;
				}
			} else if (idx < CLI_BUFFER_LEN - 1) {
				cli_cmd[idx++] = (char)c;
			}
		}
		vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void handle_command(char *cmd)
{
	if (strcmp(cmd, "help") == 0) {
		printf("Available commands:\n");
        printf("  help   - Show this message\n");
        printf("  show   - Show current lookup values\n");
        printf("  set x y - Set value for sensor x\n");
	} else if (strncmp(cmd, "set ", 4) == 0) {
		int id; 
		float val;
		if (sscanf(cmd + 4, "%d %f", &id, &val) == 2) {
			printf("Set sensor %d with value %.2f\n", id, val);
		} else {
			printf("Usage: set <id> <value>\n");
		}
	} else if (strcmp(cmd, "show") == 0) {
		printf("TODO: implement show current sensors value\n");
	} else {
		printf("Unknown command: %s\n", cmd);
	}
}
