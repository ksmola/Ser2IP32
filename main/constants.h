#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// Storage
#define STORAGE_NAMESPACE "storage"

// Console
#define CONSOLE_START_TIMEOUT 3000

// UART
#define UART_PARITY_ODD 3
#define UART_PARITY_EVEN 2
#define UART_PARITY_NONE 0

#define UART_DEFAULT_BAUDS 115200
#define UART_DEFAULT_BUFFER 2048
#define UART_DEFAULT_DATA_BITS 8
#define UART_DEFAULT_STOP_BITS 1
#define UART_DEFAULT_PARITY 0

// WIFI
#define WIFI_MODE_AP 0
#define WIFI_MODE_STATION 1
#define WIFI_SSID_MAX_LENGTH 31
#define WIFI_PASSWD_MAX_LENGTH 61

#define WIFI_AP_DEFAULT_SSID "Ser2IP32"
#define WIFI_AP_DEFAULT_PASSWD "12345678"
#define WIFI_AP_DEFAULT_CHANNEL 6

// LEDS
#define LED_DEFAULT_AP 21
#define LED_DEFAULT_STATION 23

static int UART_DEFAULT_ENABLE[3] = {1, 1, 1};
static int UART_DEFAULT_TCP_PORT[3] = {2220, 8080, 2222};

static int UART_DEFAULT_TX_PIN[3] = {25, 4, 19};
static int UART_DEFAULT_RX_PIN[3] = {21, 5, 22};

static int LED_DEFAULT_RX[3] = {10, 12, 14};
static int LED_DEFAULT_TX[3] = {15, 17, 19};
static int LED_DEFAULT_CONNECTED[3] = {0, 2, 4};

#endif