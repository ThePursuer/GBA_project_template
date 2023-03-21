// EntityManager.h
#pragma once

#include <memory>
#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include <gba_types.h>

#include "engine/ECS/Component.h"

using Entity = u32;

class EntityManager {
public:
    EntityManager(std::set<ComponentType> indexableComponents);
    ~EntityManager();

    Entity CreateEntity();
    void destroyEntity(Entity entity);
    void addComponent(Entity entity, ComponentType componentType, std::shared_ptr<Component> component);
    void removeComponent(Entity entity, ComponentType componentType);
    std::shared_ptr<Component> getComponent(Entity entity, ComponentType componentType);
    std::vector<Entity> getEntitiesWithComponent(ComponentType type); // looks up entities with a component using entitiesByComponent_
    std::vector<Entity> getEntitiesWithComponents(const std::set<ComponentType>& componentTypes);
private:
    // May need to make these unordered maps idk
    std::map<Entity, std::map<ComponentType, std::shared_ptr<Component>>> entityComponents_; // Stores components indexed by entity
    std::map<ComponentType, std::vector<Entity>> entitiesByComponent_;  // stores entities indexed by an indexable component type
    std::set<ComponentType> indexableComponentTypes_; // the types of components that should be indexed in entitiesByComponent_. Other component types do not need to be indexed.
    Entity nextEntity_;
};