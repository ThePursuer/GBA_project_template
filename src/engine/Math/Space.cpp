#include "engine/Math/Space.h"

namespace gb_physics {

Vector2 getCollisionNormal(Shape& a, Shape& b) {
    Vector2 normal(a.x() - b.x(), a.y() - b.y());
    normal.normalize();
    return normal;
}

void Space::update(gba_microseconds delta) {
    prev_delta = current_delta;
    current_delta = delta;

    for (auto it = spaceEntities.begin(); it != spaceEntities.end(); it++)
        it->second->update(delta);

    checkCollisions();
    resolveCollisions();
}

void Space::registerSpacialEntity(std::shared_ptr<Shape> shape, std::shared_ptr<Rigidbody> body) {
    spaceEntities.push_back({shape, body});
    if (spaceEntities.size() > dynamicCollisions.capacity()){
        dynamicCollisions.reserve(spaceEntities.size());
        kinematicCollisions.reserve(spaceEntities.size());
    }
}

void Space::checkCollisions() {
    for (auto it = spaceEntities.begin(); it != spaceEntities.end() - 1; it++) {
        for (auto itb = it; itb != spaceEntities.end(); itb++) {
            if (it->first->collidesWith(*itb->first)){
                if (it->second->isKinematic() && itb->second->isKinematic()){
                    kinematicCollisions.push_back({*it, *itb});
                    continue;
                }

                Vector2* relativeVelocity = new Vector2(it->second->getVelocity() - itb->second->getVelocity());
                if (relativeVelocity->length() > F16(1.0)) {
                    Vector2* collisionNormal = new Vector2(getCollisionNormal(*it->first, *itb->first));
                    fix16_t* normalVelocity = new fix16_t(relativeVelocity->dot(*collisionNormal));
                    collision c = {*it, *itb, relativeVelocity, collisionNormal, normalVelocity};
                    if (*normalVelocity > F16(0.0)) {
                        kinematicCollisions.push_back(c);
                        continue;
                    }
                    dynamicCollisions.push_back(c);
                }
            }
        }
    }
}

void Space::resolveCollisions() {
    for (auto it = kinematicCollisions.begin(); it != kinematicCollisions.end(); it++) {
        Vector2 penetration = it->a.first->getPenetration(*it->b.first);
        fix16_t ratio;
        if (it->a.second->isKinematic())
            ratio = F16(1.0);
        else if(it->b.second->isKinematic())
            ratio = F16(0.0);
        else
            ratio = fix16_div(it->a.second->getMass(), fix16_add(it->a.second->getMass(), it->b.second->getMass()));


        Vector2 aCorrected = penetration * fix16_sub(1, ratio);
        it->a.second->setPosition(aCorrected);

        Vector2 bCorrected = penetration * ratio;
        it->b.second->setPosition(bCorrected);
    }
    kinematicCollisions.clear();

    for (auto it = dynamicCollisions.begin(); it != dynamicCollisions.end(); it++) {
        // Calculate and apply the normal impulse to both bodies
        fix16_t impulseScalar = fix16_mul(F16(-0.05), fix16_div(*it->normalVelocity, fix16_add(it->a.second->getInvMass(), it->b.second->getInvMass())));
        Vector2 normalImpulse = *it->collisionNormal * impulseScalar;
        it->a.second->applyImpulse(normalImpulse.x, normalImpulse.y);
        it->b.second->applyImpulse(-normalImpulse.x, -normalImpulse.y);
    }
    dynamicCollisions.clear();
}

} // namespace gb_physics