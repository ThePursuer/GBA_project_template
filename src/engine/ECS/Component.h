#ifndef COMPONENT_H
#define COMPONENT_H

#include <unordered_set>

typedef u16 ComponentType;

class Component {}; // Inherit from me!!

#define COMPONENT_CAST(component) std::static_pointer_cast<Component>(component)

enum EngineReservedComponents : ComponentType{
        INDEXABLE_COMPONENTS_COMPONENT = 10000,
        ANIMATION_CLIP,
        POSITION,
        SPRITE,
        SOUNDBANK,
        SOUND_EFFECT,
        PHYSICS
};

class IndexableComponentsComponent : public Component {
public:
    std::unordered_set<ComponentType> indexableComponents;
};

#endif // COMPONENT_H