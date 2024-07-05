#include "Device.h"

Device::Device(int id, const String& status) : id(id), status(status) {}

Device Device::fromJSON(const JsonObject& jsonObject) {
    if (jsonObject.isNull()) {
        Serial.println("Error parsing JSON");
        return Device(-1, "unknown");
    }

    int id = jsonObject["id"];
    String status = jsonObject["status"].as<String>();

    return Device(id, status);
}

int Device::getId() const {
    return id;
}

String Device::getStatus() const {
    return status;
}
