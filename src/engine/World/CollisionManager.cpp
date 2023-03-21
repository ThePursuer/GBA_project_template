// #include <engine/World/CollisionManager.h>


// void CollisionManager::update(){
//     std::vector<std::shared_ptr<Collider>> colliders;
//     for (Actor actor : actors_){
//         for (const auto& actorCollider : actor.getHitboxes())
//             colliders.push_back(actorCollider);
//     }
//     for(auto collider_it = colliders.begin(); collider_it < colliders.end() - 1; collider_it++){
//         for(auto otherCollider_it = collider_it + 1; otherCollider_it < colliders.end(); otherCollider_it++){
//             // Check the collision
//             // todo: build a tree if we have many actors in the future
//             if((*collider_it)->collidesWith(**otherCollider_it))
//                 collisionSignal.emit((*collider_it)->getActor(), *collider_it, (*otherCollider_it)->getActor(), *otherCollider_it);
//         }
//     }
// }

// void runCollisionCallbacks(const Actor&, std::shared_ptr<Collider>, const Actor&, std::shared_ptr<Collider>){

// }
