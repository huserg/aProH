#include "Device.h"

Device::Device(int id, const String& status, int order) : id(id), status(status), order(order) {}

Device Device::fromJSON(const String& jsonString) {
    JSONVar json = JSON.parse(jsonString);

    if (JSON.typeof(json) == "undefined") {
        Serial.println("Error parsing JSON");
        return Device(-1, "unknown", '-1');
    }

    int id = JSON.stringify(json["device_id"]).toInt();
    String status = JSON.stringify(json["status"]);
    int order = JSON.stringify(json["order"]).toInt();
    status.replace("\"", "");  // Remove quotes from the parsed string

    return Device(id, status, order);
}

int Device::getId() const {
    return id;
}

String Device::getStatus() const {
    return status;
}

int Device::getOrder() const {
    return order;
}