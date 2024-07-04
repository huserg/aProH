#include "Device.h"

Device::Device(int id, const String& status) : id(id), status(status) {}

Device Device::fromJSON(const String& jsonString) {
    JSONVar json = JSON.parse(jsonString);

    if (JSON.typeof(json) == "undefined") {
        Serial.println("Error parsing JSON");
        return Device(-1, "unknown");
    }

    int id = JSON.stringify(json["device_id"]).toInt();
    String status = JSON.stringify(json["status"]);
    status.replace("\"", "");  // Remove quotes from the parsed string

    return Device(id, status);
}

int Device::getId() const {
    return id;
}

String Device::getStatus() const {
    return status;
}