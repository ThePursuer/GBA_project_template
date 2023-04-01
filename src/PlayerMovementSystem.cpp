#include "PlayerMovementSystem.h"

PlayerMovementSystem::PlayerMovementSystem(InputSystem& inputSystem){
    registerInputs(inputSystem, *this);
}

void PlayerMovementSystem::initialize(EntityManager& entityManager) {
    playerInputStateComponent = std::make_shared<PlayerInputStateComponent>();
    playerInputStateManager = entityManager.CreateEntity();
    entityManager.addComponent(playerInputStateManager, GAME_COMPONENTS::PLAYER_INPUT_STATE_COMPONENT, std::static_pointer_cast<Component>(playerInputStateComponent));
}

void PlayerMovementSystem::update(EntityManager& entityManager, gba_microseconds deltaTime) {
    Entity player = entityManager.getEntitiesWithComponent(GAME_COMPONENTS::PLAYER_COMPONENT)[0];
    auto rigidBodyComp = std::static_pointer_cast<RigidBodyComponent>(entityManager.getComponent(player, EngineReservedComponents::RIGID_BODY));
    auto positionComponent = std::static_pointer_cast<PositionComponent>(entityManager.getComponent(player, EngineReservedComponents::POSITION));

    if(playerInputStateComponent->changes.any()){
        auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(player, EngineReservedComponents::SPRITE));

        if(playerInputStateComponent->changes[PlayerInputStateComponent::DELTAX] && playerInputStateComponent->changes[PlayerInputStateComponent::DELTAY]){
            rigidBodyComp->body->applyImpulse(float(playerInputStateComponent->deltaX) * 0.7, float(playerInputStateComponent->deltaY) * 0.7);
            // Make the movement feel less like you slow down while walking diagonally without affecting physics.
            positionComponent->superPositionX += float(playerInputStateComponent->deltaX) * 0.3;
            positionComponent->superPositionY += float(playerInputStateComponent->deltaY) * 0.3;
        }
        else if(playerInputStateComponent->changes[PlayerInputStateComponent::DELTAX])
            rigidBodyComp->body->applyImpulse(playerInputStateComponent->deltaX, 0);
        else if(playerInputStateComponent->changes[PlayerInputStateComponent::DELTAY])
            rigidBodyComp->body->applyImpulse(0, playerInputStateComponent->deltaY);

        if(playerInputStateComponent->changes[PlayerInputStateComponent::FLIPX])
            spriteComponent->sprite->flipX(playerInputStateComponent->flipX);
        if(playerInputStateComponent->changes[PlayerInputStateComponent::NEXT_ANIMATION])
            AnimationSystem::playAnimation(entityManager, player, playerInputStateComponent->next_animation, playerInputStateComponent->loop_animation);
        spriteComponent->needs_update = true;
        playerInputStateComponent->changes.reset();
    }

    positionComponent->superPositionX += rigidBodyComp->body->getVelocityX();
    positionComponent->superPositionY += rigidBodyComp->body->getVelocityY();
}

void PlayerMovementSystem::shutdown(EntityManager& entityManager) {
    entityManager.destroyEntity(playerInputStateManager);
}

std::unordered_set<ComponentType> PlayerMovementSystem::requiredComponents() const { return {GAME_COMPONENTS::PLAYER_INPUT_STATE_COMPONENT, EngineReservedComponents::POSITION};}

void PlayerMovementSystem::moveUp() {
    playerInputStateComponent->deltaY = -1;
    playerInputStateComponent->next_animation = WALK;
    playerInputStateComponent->loop_animation = true;
    playerInputStateComponent->changes.set(PlayerInputStateComponent::DELTAY);
    playerInputStateComponent->changes.set(PlayerInputStateComponent::NEXT_ANIMATION);
}

void PlayerMovementSystem::moveDown() {
    playerInputStateComponent->deltaY = 1;
    playerInputStateComponent->next_animation = WALK;
    playerInputStateComponent->loop_animation = true;
    playerInputStateComponent->changes.set(PlayerInputStateComponent::DELTAY);
    playerInputStateComponent->changes.set(PlayerInputStateComponent::NEXT_ANIMATION);
}

void PlayerMovementSystem::moveLeft() {
    playerInputStateComponent->deltaX = -1;
    playerInputStateComponent->next_animation = WALK;
    playerInputStateComponent->loop_animation = true;
    playerInputStateComponent->changes.set(PlayerInputStateComponent::DELTAX);
    playerInputStateComponent->changes.set(PlayerInputStateComponent::NEXT_ANIMATION);
}

void PlayerMovementSystem::moveRight() {
    playerInputStateComponent->deltaX = 1;
    playerInputStateComponent->next_animation = WALK;
    playerInputStateComponent->loop_animation = true;
    playerInputStateComponent->changes.set(PlayerInputStateComponent::DELTAX);
    playerInputStateComponent->changes.set(PlayerInputStateComponent::NEXT_ANIMATION);
}

void PlayerMovementSystem::turnLeft(){
    PlayerMovementSystem::playerInputStateComponent->flipX = 1;
    playerInputStateComponent->changes.set(PlayerInputStateComponent::FLIPX);
}

void PlayerMovementSystem::turnRight(){
    playerInputStateComponent->flipX = 0;
    playerInputStateComponent->changes.set(PlayerInputStateComponent::FLIPX);
}

void PlayerMovementSystem::setIdle(){
    playerInputStateComponent->next_animation = IDLE;
    playerInputStateComponent->loop_animation = true;
    playerInputStateComponent->changes.set(PlayerInputStateComponent::NEXT_ANIMATION);
}

void PlayerMovementSystem::fireWand(){
    playerInputStateComponent->next_animation = FIRE;
    playerInputStateComponent->loop_animation = false;
    playerInputStateComponent->changes.set(PlayerInputStateComponent::NEXT_ANIMATION);
}

void PlayerMovementSystem::die(){
    playerInputStateComponent->next_animation = DIE;
    playerInputStateComponent->loop_animation = false;
    playerInputStateComponent->changes.set(PlayerInputStateComponent::NEXT_ANIMATION);
}

void registerInputs(InputSystem& inputHandler, PlayerMovementSystem& pms){
    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonHeld, KEY_UP), &pms, &PlayerMovementSystem::moveUp);
    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonHeld, KEY_DOWN), &pms, &PlayerMovementSystem::moveDown);
    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonHeld, KEY_LEFT), &pms, &PlayerMovementSystem::moveLeft);
    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonHeld, KEY_RIGHT), &pms, &PlayerMovementSystem::moveRight);

    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonReleased, KEY_UP), &pms, &PlayerMovementSystem::setIdle);
    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonReleased, KEY_DOWN), &pms, &PlayerMovementSystem::setIdle);
    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonReleased, KEY_LEFT), &pms, &PlayerMovementSystem::setIdle);
    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonReleased, KEY_RIGHT), &pms, &PlayerMovementSystem::setIdle);

    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonPressed, KEY_LEFT), &pms, &PlayerMovementSystem::turnLeft);
    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonPressed, KEY_RIGHT), &pms, &PlayerMovementSystem::turnRight);

    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonPressed, KEY_A), &pms, &PlayerMovementSystem::fireWand);
    CONNECT_SIGNAL(inputHandler.getEventSignal(ButtonEventType::ButtonPressed, KEY_B), &pms, &PlayerMovementSystem::die);
}