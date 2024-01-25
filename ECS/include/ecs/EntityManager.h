#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <memory>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <gba_types.h>

#include "ecs/Component.h"

using Entity = u32;

class EntityManager {
public:
    EntityManager(std::unordered_set<ComponentType> indexableComponents);
    ~EntityManager();

    Entity CreateEntity();
    void destroyEntity(Entity entity);
    void addComponent(Entity entity, ComponentType componentType, std::shared_ptr<Component> component);
    void removeComponent(Entity entity, ComponentType componentType);
    bool hasComponent(Entity entity, ComponentType component);
    std::shared_ptr<Component> getComponent(Entity entity, ComponentType componentType);
    std::vector<Entity> getEntitiesWithComponent(ComponentType type); // looks up entities with a component using entitiesByComponent_
    std::vector<Entity> getEntitiesWithComponents(const std::unordered_set<ComponentType>& componentTypes);
private:
    // May need to make these unordered maps idk
    std::unordered_map<Entity, std::unordered_map<ComponentType, std::shared_ptr<Component>>> entityComponents_; // Stores components indexed by entity
    std::unordered_map<ComponentType, std::vector<Entity>> entitiesByComponent_;  // stores entities indexed by an indexable component type
    std::shared_ptr<IndexableComponentsComponent> indexableComponentTypes_; // the types of components that should be indexed in entitiesByComponent_. Other component types do not need to be indexed.
    Entity nextEntity_;
};

#endif // ENTITY_MANAGER_H