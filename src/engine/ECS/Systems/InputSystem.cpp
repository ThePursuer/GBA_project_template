#include "engine/ECS/Systems/InputSystem.h"

void InputSystem::initialize(EntityManager& entityManager) {}

Signal<>& InputSystem::getEventSignal(ButtonEventType type, u32 key) {
    if (events.count(type) == 0 || events[type].count(key) == 0)
        events[type][key] = Signal<>();
    return events[type][key];
}

void InputSystem::update(EntityManager& entityManager, gba_microseconds deltaTime) {
    Entity player = entityManager.getEntitiesWithComponent(0)[0];
    // auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(player, EngineReservedComponents::SPRITE));
    // spriteComponent->sprite->setGFXIndex(0);
    // spriteComponent->sprite->update();

    scanKeys();
    u16 keys_down = keysDown();
    u16 keys_up = keysUp();
    u16 keys_held = keysHeld();

    // Handle ButtonPressed, ButtonReleased, and ButtonHeld events
    for (auto& pair : events[ButtonEventType::ButtonPressed]) {
        if (keys_down & pair.first) pair.second();
    }
    for (auto& pair : events[ButtonEventType::ButtonReleased]) {
        if (keys_up & pair.first) pair.second();
    }
    for (auto& pair : events[ButtonEventType::ButtonHeld]) {
        if (keys_held & pair.first) pair.second();
    }

    // Handle ButtonHeld300Ms and ButtonDoubleTap events
    using Clock = GbaClock;
    static std::map<u32, Clock::time_point> keyTimestamps;
    static std::map<u32, bool> keyTapped;

    for (auto& pair : events[ButtonEventType::ButtonHeld300Ms]) {
        if (keys_held & pair.first) {
            auto& timestamp = keyTimestamps[pair.first];
            auto now = Clock::instance().now();
            if (now - timestamp > gba_milliseconds(300)) {
                pair.second();
                timestamp = now;
            }
        }
    }

    for (auto& pair : events[ButtonEventType::ButtonDoubleTap]) {
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

void InputSystem::shutdown(EntityManager& entityManager) {}

std::set<ComponentType> InputSystem::requiredComponents() const {return {};}
