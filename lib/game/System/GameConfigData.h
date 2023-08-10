#pragma once

#include "game/System/ByamlSave.h"

namespace al {
class ByamlIter;
class IUseSceneObjHolder;
class LayoutActor;
class Scene;
}  // namespace al

class GameConfigData : public ByamlSave {
public:
    GameConfigData();
    void init();
    bool isCameraReverseInputH() const;
    void onCameraReverseInputH();
    void offCameraReverseInputH();
    bool isCameraReverseInputV() const;
    void onCameraReverseInputV();
    void offCameraReverseInputV();
    int getCameraStickSensitivityLevel() const;
    void setCameraStickSensitivityLevel(int);
    bool isValidCameraGyro() const;
    void validateCameraGyro();
    void invalidateCameraGyro();
    int getCameraGyroSensitivityLevel() const;
    void setCameraGyroSensitivityLevel(int);
    bool isUseOpenListAdditionalButton() const;
    void onUseOpenListAdditionalButton();
    void offUseOpenListAdditionalButton();
    bool isValidPadRumble() const;
    void validatePadRumble();
    void invalidatePadRumble();
    int getPadRumbleLevel() const;
    void setPadRumbleLevel(int);
    void write(al::ByamlWriter*) override;
    void read(const al::ByamlIter&) override;

private:
    int mCameraStickSensitivityLevel = -1;
    bool mIsCameraReverseInputH = false;
    bool mIsCameraReverseInputV = false;
    bool mIsValidCameraGyro = true;
    int mCameraGyroSensitivityLevel = -1;
    bool mIsUseOpenListAdditionalButton = false;
    bool mIsValidPadRumble = true;
    int mPadRumbleLevel = 0;
};

namespace rs {
GameConfigData* getGameConfigData(const al::LayoutActor*);
void saveGameConfigData(const al::LayoutActor*);
void applyGameConfigData(al::Scene*, const GameConfigData*);
bool isUseOpenListAdditionalButton(const al::IUseSceneObjHolder*);
}  // namespace rs
