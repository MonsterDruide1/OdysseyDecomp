#pragma once

namespace al {
class LiveActor;

bool isAlive(const LiveActor* actor);
bool isDead(const LiveActor* actor);
}  // namespace al
