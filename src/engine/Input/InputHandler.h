#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <gba_input.h>
#include <gba_timers.h>
#include <map>
#include <functional>
#include <chrono>
#include <stdio.h>

#include "engine/Clock/GbaClock.h"

class InputHandler {
public:
    enum class EventType {
        ButtonPressed,
        ButtonReleased,
        ButtonHeld,
        ButtonDoubleTap
    };

    using EventCallback = std::function<void()>;

    void registerEvent(EventType type, u32 key, EventCallback callback);
    void update();
private:
    void handleEvents(u16 keys_down, u16 keys_up, u16 keys_held);

    std::map<EventType, std::map<u32, EventCallback>> events;
};

#endif // INPUTHANDLER_H