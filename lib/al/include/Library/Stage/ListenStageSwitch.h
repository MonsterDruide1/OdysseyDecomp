#pragma once

namespace al {
class IUseStageSwitch;
class FunctorBase;

bool listenStageSwitchOn(IUseStageSwitch*, const char*, const FunctorBase&);
bool listenStageSwitchOnAppear(IUseStageSwitch*, const FunctorBase&);
bool listenStageSwitchOnKill(IUseStageSwitch*, const FunctorBase&);
bool listenStageSwitchOnStart(IUseStageSwitch*, const FunctorBase&);
bool listenStageSwitchOnStop(IUseStageSwitch*, const FunctorBase&);
bool listenStageSwitchOff(IUseStageSwitch*, const char*, const FunctorBase&);
bool listenStageSwitchOnOff(IUseStageSwitch*, const char*, const FunctorBase&, const FunctorBase&);
bool listenStageSwitchOnOffAppear(IUseStageSwitch*, const FunctorBase&, const FunctorBase&);
bool listenStageSwitchOnOffKill(IUseStageSwitch*, const FunctorBase&, const FunctorBase&);
bool listenStageSwitchOnOffStart(IUseStageSwitch*, const FunctorBase&, const FunctorBase&);

}  // namespace al
