#include <basis/seadTypes.h>
#include "Library/Controller/InputFunction.h"
#include "Library/Controller/JoyPadAccelPoseAnalyzer.h"
#include "Library/Controller/SpinInputAnalyzer.h"

namespace al {

bool isPadTypeJoySingle(int) {CRASH}
bool isValidReplayController(u32) {CRASH}
sead::ControllerBase* getReplayController(u32) {CRASH}

SpinInputAnalyzer::SpinInputAnalyzer() {}
void SpinInputAnalyzer::update(const sead::Vector2f &) {WARN_UNIMPL; /*ALSO FIX ABOVE IF REQUIRED*/}

}  // namespace al
