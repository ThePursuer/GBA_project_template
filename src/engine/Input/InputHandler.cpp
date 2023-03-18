#include "InputHandler.h"

void InputHandler::registerEvent(EventType type, u32 key, EventCallback callback) {
    events[type][key] = callback;
}

void InputHandler::update() {
    scanKeys();
    u16 keys_down = keysDown();
    u16 keys_up = keysUp();
    u16 keys_held = keysHeld();
    handleEvents(keys_down, keys_up, keys_held);
}

void InputHandler::handleEvents(u16 keys_down, u16 keys_up, u16 keys_held) {
    // Handle ButtonPressed, ButtonReleased, and ButtonHeld events
    for (const auto& pair : events[EventType::ButtonPressed]) {
        if (keys_down & pair.first) pair.second();
    }
    for (const auto& pair : events[EventType::ButtonReleased]) {
        if (keys_up & pair.first) pair.second();
    }
    for (const auto& pair : events[EventType::ButtonHeld]) {
        if (keys_held & pair.first) pair.second();
    }

    // Handle ButtonHeld300Ms and ButtonDoubleTap events
    using Clock = GbaClock;
    static std::map<u32, Clock::time_point> keyTimestamps;
    static std::map<u32, bool> keyTapped;

    for (const auto& pair : events[EventType::ButtonHeld300Ms]) {
        if (keys_held & pair.first) {
            auto& timestamp = keyTimestamps[pair.first];
            auto now = Clock::instance().now();
            if (now - timestamp > gba_milliseconds(300)) {
                pair.second();
                timestamp = now;
            }
        }
    }

    for (const auto& pair : events[EventType::ButtonDoubleTap]) {
        if (keys_down & pair.first) {
            auto& timestamp = keyTimestamps[pair.first];
            auto& tapped = keyTapped[pair.first];
            auto now = Clock::instance().now();

            if (tapped && now - timestamp < gba_milliseconds(300)) {
                pair.second();
                tapped = false;
            } else {
                tapped = true;
                timestamp = now;
            }
        }
    }
}