#pragma once

#include "Library/LiveActor/LiveActor.h"

class ItemAmiiboKoopa : public al::LiveActor {
public:
    ItemAmiiboKoopa(const char* actorName);
    void init(const al::ActorInitInfo& info);
    void appear();

    void exeExpand();
    void exeWait();

    void attackSensor(al::HitSensor* self, al::HitSensor* other);
};
