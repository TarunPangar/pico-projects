#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "comm_task.h"
#include "common.h"

static void blink_led();
static void print_help();
static void process_cli_command(const char *cmd);
static void handle_set_command(const char *cmd);

const char* sensorNames[] = {
    "NTC_TEMP1",
    "NTC_TEMP2",
    "NTC_TEMP3",
    "CURR_HR",
    "CURR_LR",
    "PDU_TEMP"
};

static void blink_led()
{
	gpio_put(LED_PIN, 1);
	sleep_ms(100);
	gpio_put(LED_PIN, 0);
}

void vTaskCliComm(__unused void *params)
{
	char cli_cmd[CLI_BUFFER_LEN];
	int idx = 0;

	printf("=====Welcome to CLI interface=====\n");
    printf("\n> "); // Start prompt

    while (1) {
        int c = getchar_timeout_us(0);

        // Ignore timeout - no character received
        if (c == PICO_ERROR_TIMEOUT) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue; // Skip the rest of the loop and delay
        }

        // Echo the character immediately
        printf("%c", c);

        // Backspace Handling
        if (c == 0x7F || c == '\b') {
            if (idx > 0) {
                idx--;
                printf(" ");
            }
            // Move to next loop iteration
            goto next_iteration;
        }

        // Command completion - Enter/Newline
        if (c == '\r' || c == '\n') {
            cli_cmd[idx] = '\0'; // Null-terminate the string

            if (idx > 0) {
                // Process the command
                process_cli_command(cli_cmd);
            }

            // Reset buffer and print new prompt
            idx = 0;
            printf("\n> ");
            // Move to next loop iteration
            goto next_iteration;
        }

        // Update command array
        if (idx < CLI_BUFFER_LEN - 1) {
            cli_cmd[idx++] = (char)c;
        }

        // Label and goto for task loop
        next_iteration:
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

static void handle_set_command(const char *cmd)
{
    int id;
    float val;
    sensor_val_t data_to_send;

    // Handle parsing failure
    if (sscanf(cmd + 4, "%d %f", &id, &val) != 2) {
        printf("Usage: set <id> <value>\n");
        return;
    }

    printf("Set sensor %d-%s with value %.2f\n", id, sensorNames[id - 1], val);

    // Prepare data
    data_to_send.id = id;
    data_to_send.value = val;

    // Send data to PWM task through queue
    if (xQueueSend(pwm_update_queue, &data_to_send, (TickType_t)10) != pdPASS) {
        printf("ERROR: Queue update failed\n");
        return;
    }
}

static void process_cli_command(const char *cmd)
{
    if (strcmp(cmd, "help") == 0) {
        print_help();

    } else if (strncmp(cmd, "set ", 4) == 0) {
        handle_set_command(cmd);

    } else if (strcmp(cmd, "show") == 0) {
        // TODO: Implement show logic
        printf("TODO: implement show current sensors value\n");

    } else {
        printf("Unknown command: %s\n", cmd);
        print_help();
    }
}
