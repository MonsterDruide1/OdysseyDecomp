#pragma once

class HackCap;
class PlayerAnimator;
class HackObjInfo;

namespace CapFunction {

void putOnCapPlayer(HackCap*, PlayerAnimator*);
void endHack(HackCap*, PlayerAnimator*);
const HackObjInfo& getHackObjInfo(HackCap*);

}  // namespace CapFunction
