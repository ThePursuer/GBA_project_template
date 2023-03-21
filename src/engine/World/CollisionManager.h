// #ifndef COLLISIONMANAGER_H
// #define COLLISIONMANAGER_H

// #include <map>
// #include <set>
// #include <vector>
// #include <algorithm>

// #include <gba_types.h>

// #include "engine/World/World.h"
// #include "engine/Math/ColliderI.h"
// #include "engine/World/Signal.h"
// #include "engine/ECS/System.h"


// class CollisionManager{
//     CollisionManager(const std::vector<Actor>& actors): actors_(actors){}
//     void update();

//     static Signal<const Actor&, std::shared_ptr<Collider>, const Actor&, std::shared_ptr<Collider>> collisionSignal; 
// private:
//     const std::vector<Actor>& actors_;
// };

// class CollisionSystem : public ISystem {
// public:
//     void initialize(EntityManager& entityManager) override {
//         // Initialization code
//     }

//     void update(EntityManager& entityManager, float deltaTime) override {
//         auto entities = entityManager.getEntitiesWithComponent(COMPONENT_TYPE_CUSTOM);

//         for (const auto& entity : entities) {
//             auto customComponent = std::static_pointer_cast<CustomComponent>(entityManager.getComponent(entity, COMPONENT_TYPE_CUSTOM));
//             // Update the customComponent as needed
//         }
//     }

//     void shutdown(EntityManager& entityManager) override {
//         // Cleanup code
//     }

//     std::set<ComponentType> requiredComponents() const override {
//         return { COMPONENT_TYPE_CUSTOM };
//     }
// };


// #endif // COLLISIONMANAGER_H