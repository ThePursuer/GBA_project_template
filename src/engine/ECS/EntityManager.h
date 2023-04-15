// EntityManager.h
#pragma once

#include <memory>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <gba_types.h>

#include "engine/ECS/Component.h"

using Entity = u32;

class EntityManager {
public:
    EntityManager(std::unordered_set<ComponentType> indexableComponents);
    ~EntityManager();

    Entity __attribute__((section(".iwram"), long_call)) CreateEntity();
    void __attribute__((section(".iwram"), long_call)) destroyEntity(Entity entity);
    void __attribute__((section(".iwram"), long_call)) addComponent(Entity entity, ComponentType componentType, std::shared_ptr<Component> component);
    void __attribute__((section(".iwram"), long_call)) removeComponent(Entity entity, ComponentType componentType);
    std::shared_ptr<Component> __attribute__((section(".iwram"), long_call)) getComponent(Entity entity, ComponentType componentType);
    bool __attribute__((section(".iwram"), long_call)) hasComponent(Entity entity, ComponentType componentType);
    std::vector<Entity> __attribute__((section(".iwram"), long_call)) getEntitiesWithComponent(ComponentType type); // looks up entities with a component using entitiesByComponent_
    std::vector<Entity> __attribute__((section(".iwram"), long_call)) getEntitiesWithComponents(const std::unordered_set<ComponentType>& componentTypes);
private:
    // May need to make these unordered maps idk
    std::unordered_map<Entity, std::unordered_map<ComponentType, std::shared_ptr<Component>>> entityComponents_; // Stores components indexed by entity
    std::unordered_map<ComponentType, std::vector<Entity>> entitiesByComponent_;  // stores entities indexed by an indexable component type
    std::shared_ptr<IndexableComponentsComponent> indexableComponentTypes_; // the types of components that should be indexed in entitiesByComponent_. Other component types do not need to be indexed.
    Entity nextEntity_;
};