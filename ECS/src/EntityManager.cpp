// EntityManager.cpp
#include "ecs/EntityManager.h"

EntityManager::EntityManager(std::unordered_set<ComponentType> indexableComponents): nextEntity_(1) {
    indexableComponentTypes_ = std::make_shared<IndexableComponentsComponent>();
    indexableComponentTypes_->indexableComponents.insert(indexableComponents.begin(), indexableComponents.end());
    Entity indexableComponentsEntity = CreateEntity();
    addComponent(indexableComponentsEntity, EngineReservedComponents::INDEXABLE_COMPONENTS_COMPONENT, indexableComponentTypes_);
}

EntityManager::~EntityManager() {}

Entity EntityManager::CreateEntity() {
    Entity newEntity = nextEntity_;
    nextEntity_++;
    return newEntity;
}

void EntityManager::destroyEntity(Entity entity) {
    for (auto const& [componentType, component] : entityComponents_[entity]) {
        if (indexableComponentTypes_->indexableComponents.count(componentType) > 0) {
            auto& entities = entitiesByComponent_[componentType];
            auto entityToRemove = std::find(entities.begin(), entities.end(), entity);
            entities.erase(entityToRemove);
        }
    }
    entityComponents_.erase(entity);
}

void EntityManager::addComponent(Entity entity, ComponentType componentType, std::shared_ptr<Component> component) {
    entityComponents_[entity][componentType] = component;
    if (indexableComponentTypes_->indexableComponents.count(componentType) > 0) {
        entitiesByComponent_[componentType].push_back(entity);
    }
}

void EntityManager::removeComponent(Entity entity, ComponentType componentType) {
    entityComponents_[entity].erase(componentType);
    if (indexableComponentTypes_->indexableComponents.count(componentType) > 0) {
        auto& entities = entitiesByComponent_[componentType];
        auto entityToRemove = std::find(entities.begin(), entities.end(), entity);
        entities.erase(entityToRemove);
    }
}

bool EntityManager::hasComponent(Entity entity, ComponentType component){
    return entityComponents_[entity].find(component) != entityComponents_[entity].end();
}

std::shared_ptr<Component> EntityManager::getComponent(Entity entity, ComponentType componentType) {
    return entityComponents_[entity][componentType]; // Assume the caller knows what they are talking about
}

std::vector<Entity> EntityManager::getEntitiesWithComponent(ComponentType type) {
    if (indexableComponentTypes_->indexableComponents.count(type) > 0) {
        return entitiesByComponent_[type];
    }
    return std::vector<Entity>();
}

std::vector<Entity> EntityManager::getEntitiesWithComponents(const std::unordered_set<ComponentType>& componentTypes) {
    std::vector<Entity> result;

    // Get the smallest entity list to minimize iterations
    auto it = std::min_element(componentTypes.begin(), componentTypes.end(),
        [&](const ComponentType& a, const ComponentType& b) {
            return entitiesByComponent_[a].size() < entitiesByComponent_[b].size();
        });

    const auto& smallestEntityList = entitiesByComponent_[*it];

    // Iterate over the smallest entity list and check if each entity has all the required components
    for (const auto& entity : smallestEntityList) {
        bool hasAllComponents = true;

        for (const auto& componentType : componentTypes) {
            if (entityComponents_[entity].count(componentType) == 0) {
                hasAllComponents = false;
                break;
            }
        }

        if (hasAllComponents) {
            result.push_back(entity);
        }
    }

    return result;
}
