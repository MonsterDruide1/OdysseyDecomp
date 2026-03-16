#pragma once

class HackCap;
class PlayerAnimator;
struct HackObjInfo;

namespace CapFunction {

void putOnCapPlayer(HackCap* hackCap, PlayerAnimator* animator);
void endHack(HackCap* hackCap, PlayerAnimator* animator);
const HackObjInfo* getHackObjInfo(HackCap* hackCap);

}  // namespace CapFunction
