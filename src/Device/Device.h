#pragma once

#include <Arduino_JSON.h>

class Device {
public:
    Device(int id, const String& status, int order);

    static Device fromJSON(const String& jsonString);

    int getId() const;
    String getStatus() const;
    int getOrder() const;

private:
    int id;
    String status;
    int order;
};
