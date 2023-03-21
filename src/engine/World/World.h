#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "engine/Actor/Actor.h"

class World{
public:
    const std::vector<Actor>& actors() const {return actors_;}
private:
    std::vector<Actor>& actors_;

};



#endif // WORLD_H