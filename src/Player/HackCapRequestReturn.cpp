#include "Player/HackCapRequestReturn.h"

#include "Player/HackCap.h"

bool HackCap::requestReturn(bool* returnResult) {
    return tryReturn(false, returnResult);
}
