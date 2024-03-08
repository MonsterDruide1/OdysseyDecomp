#pragma once

namespace al {
class IUseStageSwitch;
class FunctorBase;

bool listenStageSwitchOn(al::IUseStageSwitch*, const char*, const al::FunctorBase&);
bool listenStageSwitchOnAppear(al::IUseStageSwitch*, const al::FunctorBase&);
bool listenStageSwitchOnKill(al::IUseStageSwitch*, const al::FunctorBase&);
bool listenStageSwitchOnStart(al::IUseStageSwitch*, const al::FunctorBase&);
bool listenStageSwitchOnStop(al::IUseStageSwitch*, const al::FunctorBase&);
bool listenStageSwitchOff(al::IUseStageSwitch*, const char*, const al::FunctorBase&);
bool listenStageSwitchOnOff(al::IUseStageSwitch*, const char*, const al::FunctorBase&,
                            const al::FunctorBase&);
bool listenStageSwitchOnOffAppear(al::IUseStageSwitch*, const al::FunctorBase&,
                                  const al::FunctorBase&);
bool listenStageSwitchOnOffKill(al::IUseStageSwitch*, const al::FunctorBase&,
                                const al::FunctorBase&);
bool listenStageSwitchOnOffStart(al::IUseStageSwitch*, const al::FunctorBase&,
                                 const al::FunctorBase&);

}  // namespace al
