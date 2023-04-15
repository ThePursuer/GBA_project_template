#include <gba.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <cassert>

#include "chipmunk/chipmunk_private.h"

#include "wizard_spritesheet_calciumtrice_indexed.h"
#include "shapes.h"

#include "soundbank.h"
#include "soundbank_bin.h"

#include "engine/Graphics/Sprite.h"
#include "engine/Signal.h"
#include "engine/Graphics/SpriteManager.h"

#include "engine/ECS/SystemManager.h"
#include "engine/ECS/Components/EngineComponents.h"
#include "engine/ECS/EntityManager.h"
#include "engine/ECS/Systems/AnimationSystem.h"
#include "engine/ECS/Systems/InputSystem.h"
#include "engine/ECS/Systems/SpriteSystem.h"
#include "engine/ECS/Systems/PositionSystem.h"
#include "engine/ECS/Systems/AudioSystem.h"
#include "engine/ECS/Systems/PhysicsSystem.h"

#include "PlayerMovementSystem.h"

u32 shapes;
cpSpace* IWRAM_DATA space;

Entity generatePlayer(EntityManager & manager){
    std::shared_ptr<PositionComponent> player_position = std::make_shared<PositionComponent>();
    player_position->x = 80;
    player_position->y = 80;
    player_position->superPositionX = 80;
    player_position->superPositionY = 80;

    std::shared_ptr<AnimationClipComponent> player_animations = std::make_shared<AnimationClipComponent>();
    {
        AnimationClip idle;
        idle.clipDuration = gba_milliseconds(200);
        idle.frameCount = 10;
        idle.graphicsData = (unsigned short*)&wizard_spritesheet_calciumtrice_indexedTiles;
        idle.graphicsDataLen = wizard_spritesheet_calciumtrice_indexedTilesLen / 5;
        player_animations->clips.emplace(IDLE, idle);

        AnimationClip walk;
        walk.clipDuration = gba_milliseconds(200);
        walk.frameCount = 10;
        walk.graphicsData = ((unsigned short*)&wizard_spritesheet_calciumtrice_indexedTiles) + wizard_spritesheet_calciumtrice_indexedTilesLen / 5 ;
        walk.graphicsDataLen = wizard_spritesheet_calciumtrice_indexedTilesLen / 5;
        player_animations->clips.emplace(WALK, walk);
    }

    std::shared_ptr<SpriteComponent> player_sprite = std::make_shared<SpriteComponent>();
    player_sprite->sprite = std::make_unique<Sprite>(ATTR0_COLOR_16 | ATTR0_NORMAL | ATTR0_SQUARE,
        ATTR1_SIZE_32,
        ATTR2_PRIORITY(0) | ATTR2_PALETTE(0),
        1,
        *player_position.get()
    );
    player_sprite->needs_update = true;

    Entity player = manager.CreateEntity();

    std::shared_ptr<PhysicsComponent> physicsComponent = std::make_shared<PhysicsComponent>();

    PhysicsStructure idleForm;
    cpFloat width = 32, height = 32;
    idleForm.body = std::shared_ptr<cpBody>(cpBodyNew(10, cpMomentForBox(10, width, height)));
    idleForm.shape = std::shared_ptr<cpShape>(cpBoxShapeNew(idleForm.body.get(), height, width, 0));

    cpShapeSetFriction(idleForm.shape.get(), 0.5);
    cpShapeSetElasticity(idleForm.shape.get(), 0.3);
    cpBodySetPosition(idleForm.body.get(), cpv(player_position->superPositionX, player_position->superPositionY)); // Set the body's position

    physicsComponent->currentForm = 0;
    physicsComponent->forms[0] = idleForm;

    manager.addComponent(player, EngineReservedComponents::PHYSICS, physicsComponent);
    manager.addComponent(player, EngineReservedComponents::ANIMATION_CLIP, player_animations);
    manager.addComponent(player, EngineReservedComponents::SPRITE, player_sprite);
    manager.addComponent(player, EngineReservedComponents::POSITION, player_position);
    manager.addComponent(player, GAME_COMPONENTS::PLAYER_COMPONENT, std::make_shared<PlayerComponent>());

    return player;
}

Entity generateBox(EntityManager & manager, u32 x, u32 y, u32 oam_id){
    Entity boxEntity = manager.CreateEntity();

    std::shared_ptr<PositionComponent> positionComponent = std::make_shared<PositionComponent>();
    positionComponent->x = x;
    positionComponent->y = y;
    positionComponent->superPositionX = x;
    positionComponent->superPositionY = y;

    std::shared_ptr<SpriteComponent> boxSprite = std::make_shared<SpriteComponent>();
    boxSprite->sprite = std::make_unique<Sprite>(ATTR0_COLOR_16 | ATTR0_NORMAL | ATTR0_SQUARE,
        ATTR1_SIZE_32,
        ATTR2_PRIORITY(0) | ATTR2_PALETTE(1),
        oam_id,
        *positionComponent
    );
    boxSprite->sprite->setGFXIndex(1);
    boxSprite->needs_update = true;

    std::shared_ptr<PhysicsComponent> physicsComponent = std::make_shared<PhysicsComponent>();

    PhysicsStructure idleForm;
    cpFloat width = 32, height = 32;
    idleForm.body = std::shared_ptr<cpBody>(cpBodyNew(10, cpMomentForBox(10, width, height)));
    idleForm.shape = std::shared_ptr<cpShape>(cpBoxShapeNew(idleForm.body.get(), height, width, 0));

    cpShapeSetFriction(idleForm.shape.get(), 0.5);
    cpShapeSetElasticity(idleForm.shape.get(), 0.3);
    cpBodySetPosition(idleForm.body.get(), cpv(positionComponent->superPositionX, positionComponent->superPositionY)); // Set the body's position

    physicsComponent->currentForm = 0;
    physicsComponent->forms[0] = idleForm;

    cpVect imp;
    imp.x = 1;
    imp.y = 0;
    cpBodyApplyImpulseAtWorldPoint(idleForm.body.get(), imp, idleForm.body->p);

    manager.addComponent(boxEntity, EngineReservedComponents::PHYSICS, physicsComponent);
    manager.addComponent(boxEntity, EngineReservedComponents::SPRITE, boxSprite);
    manager.addComponent(boxEntity, EngineReservedComponents::POSITION, positionComponent);

    return boxEntity;
}

Entity generateSoundbank(EntityManager& manager){
    Entity sbEntity = manager.CreateEntity();
    std::shared_ptr<SoundbankComponent> sbComp = std::make_shared<SoundbankComponent>();
    sbComp->addr = (mm_addr)&soundbank_bin;
    sbComp->channels = 8;
    manager.addComponent(sbEntity, EngineReservedComponents::SOUNDBANK, COMPONENT_CAST(sbComp));
    return sbEntity;
}

Entity generateFX(EntityManager& manager){
    Entity fxEntity = manager.CreateEntity();
    std::shared_ptr<SoundFXComponent> fxComp = std::make_shared<SoundFXComponent>();
    manager.addComponent(fxEntity, EngineReservedComponents::SOUND_EFFECT, fxComp);

    // fxComp->soundfx[SFX_AMBULANCE] = mm_sound_effect{
	// 	{ SFX_AMBULANCE } ,			// id
	// 	(int)(1.0f * (1<<10)),	// rate
	// 	0,		// handle
	// 	255,	// volume
	// 	0,		// panning
	// };

    return fxEntity;
}

void initializeGBA(){
    // Initialize GBA and devkitPro libraries
    irqInit();
	irqSet( IRQ_VBLANK, mmVBlank );
    irqEnable(IRQ_VBLANK);

	// initialise the console
	// setting NULL & 0 for the font address & size uses the default font
	// The font should be a complete 1bit 8x8 ASCII font
	consoleInit(	0,		// charbase
					4,		// mapbase
					0,		// background number
					NULL,	// font
					0, 		// font size
					15		// 16 color palette
	);

	// set the screen colors, color 0 is the background color
	// the foreground color is index 1 of the selected 16 color palette
	BG_COLORS[0]=RGB8(58,110,165);
	BG_COLORS[241]=RGB5(31,31,31);

    // Set up the video mode and enable sprites
    SetMode(MODE_0 | BG0_ON | OBJ_ENABLE | OBJ_1D_MAP);
}

int main() {
    initializeGBA();

    SpriteManager& spriteManager = SpriteManager::getInstance();
    CpuFastSet(wizard_spritesheet_calciumtrice_indexedPal, SPRITE_PALETTE, wizard_spritesheet_calciumtrice_indexedPalLen / 4);
    // CpuFastSet(wizard_spritesheet_calciumtrice_indexedTiles, (char*)(SPRITE_GFX) + 32, wizard_spritesheet_calciumtrice_indexedTilesLen / 4);

    CpuFastSet(shapesPal, SPRITE_PALETTE + 16, shapesPalLen / 4);
    // CpuFastSet(shapesTiles, SPRITE_GFX + 2048 * 10, shapesTilesLen / 4);
    shapes = spriteManager.getInstance().loadSprite(&shapesTiles, shapesTilesLen);

    // Create stuff
    std::unordered_set<ComponentType> indexableComponents = {
        EngineReservedComponents::ANIMATION_CLIP,
        EngineReservedComponents::POSITION,
        EngineReservedComponents::SPRITE,
        GAME_COMPONENTS::PLAYER_COMPONENT,
        GAME_COMPONENTS::PLAYER_INPUT_STATE_COMPONENT,
        EngineReservedComponents::SOUNDBANK,
        EngineReservedComponents::SOUND_EFFECT,
        EngineReservedComponents::PHYSICS
    };
    EntityManager entityManager(indexableComponents);
    
    Entity player = generatePlayer(entityManager);
    Entity box = generateBox(entityManager, 10, 10, 2);
    Entity sbEntity = generateSoundbank(entityManager);
    Entity fxEntity = generateFX(entityManager);

    SystemManager systemManager;
    AnimationSystem animationSystem;
    SpriteSystem spriteSystem;
    InputSystem inputSystem;
    AudioSystem audioSystem;
    PlayerMovementSystem movementSystem(inputSystem, audioSystem);
    PhysicsSystem physicsSystem;
    PositionSystem positionSystem;

    systemManager.addSystem(&inputSystem);
    systemManager.addSystem(&animationSystem);
    systemManager.addSystem(&audioSystem);
    systemManager.addSystem(&movementSystem);
    systemManager.addSystem(&physicsSystem);
    systemManager.addSystem(&positionSystem);
    systemManager.addSystem(&spriteSystem);

    // Initialize stuff
    systemManager.initializeAllSystems(entityManager);

    audioSystem.queueMusic(MOD_WARLORD, mm_pmode::MM_PLAY_LOOP);
    animationSystem.playAnimation(entityManager, player, PLAYER_ANIMATIONS::IDLE, true);

    auto clock = GbaClock::instance();
    auto last_frame_time = clock.now();
    
	space = cpSpaceNew();
	space->iterations = 1;
    space->collisionSlop = 0;
    space->collisionBias = 0;
    // space->idleSpeedThreshold = 1;
    // space->sleepTimeThreshold = 0.1;
    space->damping = 0.2;

    PhysicsSystem::updatePhysicsForm(entityManager, player, 0, space);
    PhysicsSystem::updatePhysicsForm(entityManager, box, 0, space);

    // Main game loop
    u32 count = 0;
    while (1) {
        count++;
        auto now = clock.now();
        auto delta = std::chrono::duration_cast<gba_microseconds>(now - last_frame_time);
        last_frame_time = now;
        systemManager.updateAllSystems(entityManager, delta);
        cpSpaceStep(space, (std::chrono::duration_cast<gba_milliseconds>(delta).count() / cpFloat(1000.0)));

        auto deltad = clock.now() - now;
        if(count % 10 == 0){
            printf("\033[17;0HFrame: %i     ", std::chrono::duration_cast<gba_milliseconds>(delta).count());
            printf("\033[18;0HUpdate %i     ", std::chrono::duration_cast<gba_milliseconds>(deltad).count());
        }
        // Wait for VBlank to avoid screen tearing
        VBlankIntrWait();
    }

    return 0;
}
