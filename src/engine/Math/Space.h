#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "engine/Clock/GbaClock.h"
#include "engine/Math/Vector.h"
#include "engine/Math/Shape.h"
#include "engine/Math/RigidBody.h"

namespace gb_physics {

Vector2 getCollisionNormal(Shape& a, Shape& b);

struct collision {
    std::pair<std::shared_ptr<Shape>, std::shared_ptr<Rigidbody>> a;
    std::pair<std::shared_ptr<Shape>, std::shared_ptr<Rigidbody>> b;
    Vector2* relativeVelocity = nullptr;
    Vector2* collisionNormal = nullptr;
    fix16_t* normalVelocity;
};

class Space {
public:
    void IWRAM_CODE update(gba_microseconds delta);
    void registerSpacialEntity(std::shared_ptr<Shape> shape, std::shared_ptr<Rigidbody> body);
private:
    void IWRAM_CODE checkCollisions();
    void IWRAM_CODE resolveCollisions();

    gba_microseconds prev_delta;
    gba_microseconds current_delta;
    Vector2 gravity;
    fix16_t damping;
    std::vector<collision> kinematicCollisions;
    std::vector<collision> dynamicCollisions;
    std::vector<std::pair<std::shared_ptr<Shape>, std::shared_ptr<Rigidbody>>> spaceEntities;
};

// struct cpSpace {
// 	int iterations;
	
// 	cpVect gravity;
// 	cpFloat damping;
	
// 	cpFloat idleSpeedThreshold;
// 	cpFloat sleepTimeThreshold;
	
// 	cpFloat collisionSlop;
// 	cpFloat collisionBias;
// 	cpTimestamp collisionPersistence;
	
// 	cpDataPointer userData;
	
// 	cpTimestamp stamp;
// 	cpFloat curr_dt;

// 	cpArray *dynamicBodies;
// 	cpArray *staticBodies;
// 	cpArray *rousedBodies;
// 	cpArray *sleepingComponents;
	
// 	cpHashValue shapeIDCounter;
// 	cpSpatialIndex *staticShapes;
// 	cpSpatialIndex *dynamicShapes;
	
// 	cpArray *constraints;
	
// 	cpArray *arbiters;
// 	cpContactBufferHeader *contactBuffersHead;
// 	cpHashSet *cachedArbiters;
// 	cpArray *pooledArbiters;
	
// 	cpArray *allocatedBuffers;
// 	int locked;
	
// 	cpBool usesWildcards;
// 	cpHashSet *collisionHandlers;
// 	cpCollisionHandler defaultHandler;
	
// 	cpBool skipPostStep;
// 	cpArray *postStepCallbacks;
	
// 	cpBody *staticBody;
// 	cpBody _staticBody;
// };

} // namespace gb_physics