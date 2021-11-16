#pragma once

#include <math/seadVector.h>

namespace al {

bool isPadTypeJoySingle(int);

bool isPadTriggerUp(int);
bool isPadTriggerDown(int);
bool isPadTriggerLeft(int);
bool isPadTriggerRight(int);

bool isPadTriggerA(int);
bool isPadTriggerB(int);
bool isPadTriggerX(int);
bool isPadTriggerY(int);

bool isPadTriggerZL(int);
bool isPadTriggerZR(int);

bool isPadTriggerL(int);
bool isPadTriggerR(int);

bool isPadHoldUp(int);
bool isPadHoldDown(int);
bool isPadHoldLeft(int);
bool isPadHoldRight(int);

bool isPadHoldA(int);
bool isPadHoldB(int);
bool isPadHoldX(int);
bool isPadHoldY(int);

bool isPadHoldL(int);
bool isPadHoldR(int);

bool isPadHoldZL(int);
bool isPadHoldZR(int);

sead::Vector2f *getLeftStick(int);
sead::Vector2f *getRightStick(int);

unsigned int getPlayerControllerPort(int);

}
