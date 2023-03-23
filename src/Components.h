#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "engine/ECS/Component.h"

enum GAME_COMPONENTS{
    PLAYER_COMPONENT, // the main player
    PLAYER_INPUT_STATE_COMPONENT
};

class PlayerComponent: public Component{
    public:
};

#endif // COMPONENTS_H