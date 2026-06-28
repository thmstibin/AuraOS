#pragma once
#include <stdint.h>
#include <vector>

struct WiFiNetwork {
    char ssid[33];
    int8_t rssi;
    uint8_t encryptionType;
};

class WiFiManager {
public:
    static WiFiManager &instance();

    bool begin();
    void scan();
    bool connect(const char *ssid, const char *password);
    void disconnect();

    bool isConnected() const { return _connected; }
    bool isConnecting() const { return _connecting; }
    const char *connectedSSID() const { return _connectedSSID; }
    int8_t signalStrength() const { return _rssi; }

    int networkCount() const { return _networks.size(); }
    const WiFiNetwork *getNetwork(int idx) const;

    void update();  // call periodically to check connection state

private:
    WiFiManager() = default;

    bool _connected = false;
    bool _connecting = false;
    char _connectedSSID[33] = {};
    int8_t _rssi = 0;

    std::vector<WiFiNetwork> _networks;
};
