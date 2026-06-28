#include "WiFiManager.h"
#include <WiFi.h>
#include <Arduino.h>

WiFiManager &WiFiManager::instance() {
    static WiFiManager inst;
    return inst;
}

bool WiFiManager::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    Serial.println("[WiFi] Initialized in STA mode");
    return true;
}

void WiFiManager::scan() {
    _networks.clear();
    Serial.println("[WiFi] Scanning networks...");

    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("[WiFi] No networks found");
        return;
    }

    for (int i = 0; i < n; i++) {
        WiFiNetwork net;
        snprintf(net.ssid, sizeof(net.ssid), "%s", WiFi.SSID(i).c_str());
        net.rssi = WiFi.RSSI(i);
        net.encryptionType = WiFi.encryptionType(i);
        _networks.push_back(net);
        Serial.printf("[WiFi] Found: %s (RSSI: %d)\n", net.ssid, net.rssi);
    }
}

bool WiFiManager::connect(const char *ssid, const char *password) {
    if (_connecting || _connected) {
        Serial.println("[WiFi] Already connecting or connected");
        return false;
    }

    Serial.printf("[WiFi] Connecting to '%s'...\n", ssid);
    WiFi.begin(ssid, password);

    _connecting = true;
    snprintf(_connectedSSID, sizeof(_connectedSSID), "%s", ssid);

    // Wait up to 10 seconds for connection
    int retries = 0;
    while (retries < 100 && WiFi.status() != WL_CONNECTED) {
        delay(100);
        retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        _connected = true;
        _connecting = false;
        _rssi = WiFi.RSSI();
        Serial.printf("[WiFi] Connected! IP: %s (RSSI: %d)\n",
                      WiFi.localIP().toString().c_str(), _rssi);
        return true;
    } else {
        _connected = false;
        _connecting = false;
        Serial.println("[WiFi] Connection failed");
        return false;
    }
}

void WiFiManager::disconnect() {
    WiFi.disconnect(true);  // true = turn off WiFi radio
    _connected = false;
    _connecting = false;
    _connectedSSID[0] = '\0';
    Serial.println("[WiFi] Disconnected");
}

const WiFiNetwork *WiFiManager::getNetwork(int idx) const {
    if (idx < 0 || idx >= (int)_networks.size()) {
        return nullptr;
    }
    return &_networks[idx];
}

void WiFiManager::update() {
    if (_connected) {
        _rssi = WiFi.RSSI();  // Update signal strength
    } else if (_connecting && WiFi.status() == WL_CONNECTED) {
        _connected = true;
        _connecting = false;
        _rssi = WiFi.RSSI();
        Serial.printf("[WiFi] Auto-connected! IP: %s\n",
                      WiFi.localIP().toString().c_str());
    }
}
