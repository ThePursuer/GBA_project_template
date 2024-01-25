#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <gba_input.h>
#include <gba_timers.h>

#include "ecs/Input.h"
#include "ecs/System.h"
#include "ecs/Components/BuiltinComponents.h"

#include <gba_os/Signal.h>

class InputSystem : public ISystem{
public:
    void initialize(EntityManager& entityManager);
    Signal<>& getEventSignal(ButtonEventType type, u32 key); // get signal associated with button press
    void update(EntityManager& entityManager, gba_os::chrono::gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const;
private:
    std::unordered_map<ButtonEventType, std::unordered_map<u32, Signal<>>> events;
};

#endif // INPUT_SYSTEM_H