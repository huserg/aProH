#pragma once

#include <ArduinoJson.h>

class Device {
public:
    Device(int id, const String& status);

    static Device fromJSON(const JsonObject& jsonObject);

    int getId() const;
    String getStatus() const;

private:
    int id;
    String status;
};
