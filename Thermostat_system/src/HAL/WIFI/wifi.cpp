
#include <Arduino.h>
#include <WiFi.h>
#include "../../App_cfg.h"
#include "../../HAL/GPIO/gpio.h"
#include "wifi.h"

#if WIFI_DEBUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)
#else
#define DEBUG_PRINTLN(var)
#endif

void onWifiConnected(void)
{
    Serial.println("WiFi Connected! ");
}

void onWifiDisconnected(void)
{
    Serial.println("WiFi Disconnected ");
}

static WIFI_Config_t g_wifiCfg = {
    .ssid = SSID,
    .password = PASSWORD,
    .reconnect_interval_ms = 5000,
    .on_connect = onWifiConnected,
    .on_disconnect = onWifiDisconnected};

static WIFI_Status_t g_wifiStatus = WIFI_STATUS_DISCONNECTED;
static unsigned long g_lastReconnectAttempt = 0;

static void WIFI_StartConnection(void)
{
    if (g_wifiCfg.ssid == NULL || g_wifiCfg.password == NULL)
    {
        g_wifiStatus = WIFI_STATUS_ERROR;
        return;
    }

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true, true);
    delay(100);

    WiFi.begin(g_wifiCfg.ssid, g_wifiCfg.password);
    g_wifiStatus = WIFI_STATUS_CONNECTING;
}

void WIFI_Init_(const WIFI_Config_t *config)
{
    g_wifiCfg = *config;
    g_lastReconnectAttempt = millis();
    WIFI_StartConnection();
}

void WIFI_Process(void)
{
    wl_status_t st = WiFi.status();

    switch (g_wifiStatus)
    {
    case WIFI_STATUS_CONNECTING:
        if (st == WL_CONNECTED)
        {
            g_wifiStatus = WIFI_STATUS_CONNECTED;
            if (g_wifiCfg.on_connect)
                g_wifiCfg.on_connect();
        }
        else if (st == WL_CONNECT_FAILED ||
                 st == WL_NO_SSID_AVAIL ||
                 st == WL_DISCONNECTED)
        {
            g_wifiStatus = WIFI_STATUS_DISCONNECTED;
            g_lastReconnectAttempt = millis();
        }
        break;

    case WIFI_STATUS_CONNECTED:
        if (st != WL_CONNECTED)
        {
            g_wifiStatus = WIFI_STATUS_DISCONNECTED;
            if (g_wifiCfg.on_disconnect)
                g_wifiCfg.on_disconnect();
            g_lastReconnectAttempt = millis();
        }
        break;

    case WIFI_STATUS_DISCONNECTED:
        if (millis() - g_lastReconnectAttempt >= g_wifiCfg.reconnect_interval_ms)
        {
            WIFI_StartConnection();
            g_lastReconnectAttempt = millis();
        }
        break;

    case WIFI_STATUS_ERROR:
    default:
        break;
    }
}

WIFI_Status_t WIFI_GetStatus(void)
{
    return g_wifiStatus;
}

bool WIFI_IsConnected(void)
{
    return (g_wifiStatus == WIFI_STATUS_CONNECTED);
}

int WIFI_GetRSSI(void)
{
    if (WiFi.status() == WL_CONNECTED)
        return WiFi.RSSI();
    return 0;
}
