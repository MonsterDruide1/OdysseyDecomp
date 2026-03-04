#pragma once

class HackCap;
class PlayerAnimator;
struct HackObjInfo;

class CapFunction {
public:
    static void putOnCapPlayer(HackCap* hackCap, PlayerAnimator* animator);
    static void endHack(HackCap* hackCap, PlayerAnimator* animator);
    static const HackObjInfo& getHackObjInfo(HackCap* hackCap);
};
