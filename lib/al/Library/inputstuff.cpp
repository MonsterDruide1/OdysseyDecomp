#include <basis/seadTypes.h>
#include "Library/Controller/InputFunction.h"
#include "Library/Controller/JoyPadAccelPoseAnalyzer.h"
#include "Library/Controller/SpinInputAnalyzer.h"

namespace al {

bool isPadTypeJoySingle(int) {CRASH}
bool isValidReplayController(u32) {CRASH}
sead::ControllerBase* getReplayController(u32) {CRASH}

bool JoyPadAccelPoseAnalyzer::isSwingAnyHand() const {WARN_UNIMPL;return false;}
bool JoyPadAccelPoseAnalyzer::isSwingDoubleHandSameDir() const {CRASH}
void JoyPadAccelPoseAnalyzer::setSwingBorder(f32 val1, f32 val2) {
    mSwingBorder = {val1, val2};
}

SpinInputAnalyzer::SpinInputAnalyzer() {}
void SpinInputAnalyzer::update(const sead::Vector2f &) {CRASH /*ALSO FIX ABOVE IF REQUIRED*/}

}  // namespace al
