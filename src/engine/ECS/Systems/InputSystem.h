#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <gba_input.h>
#include <gba_timers.h>

#include "engine/Input.h"
#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"

#include "engine/ECS/Systems/AnimationSystem.h"

class InputSystem : public ISystem{
public:
    void initialize(EntityManager& entityManager);
    Signal<>& getEventSignal(ButtonEventType type, u32 key); // get signal associated with button press
    void update(EntityManager& entityManager, gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::set<ComponentType> requiredComponents() const;
private:
    std::map<ButtonEventType, std::map<u32, Signal<>>> events;
};


#endif // INPUT_SYSTEM_H