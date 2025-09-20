#include "System/GameConfigData.h"

#include "Library/Yaml/ByamlUtil.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

void GameConfigData::init() {
    mCameraStickSensitivityLevel = -1;
    mIsCameraReverseInputH = false;
    mIsCameraReverseInputV = false;
    mIsValidCameraGyro = true;
    mCameraGyroSensitivityLevel = -1;
    mIsUseOpenListAdditionalButton = false;
    mIsValidPadRumble = true;
    mPadRumbleLevel = 0;
}

bool GameConfigData::isCameraReverseInputH() const {
    return mIsCameraReverseInputH;
}

void GameConfigData::onCameraReverseInputH() {
    mIsCameraReverseInputH = true;
}

void GameConfigData::offCameraReverseInputH() {
    mIsCameraReverseInputH = false;
}

bool GameConfigData::isCameraReverseInputV() const {
    return mIsCameraReverseInputV;
}

void GameConfigData::onCameraReverseInputV() {
    mIsCameraReverseInputV = true;
}

void GameConfigData::offCameraReverseInputV() {
    mIsCameraReverseInputV = false;
}

s32 GameConfigData::getCameraStickSensitivityLevel() const {
    return mCameraStickSensitivityLevel;
}

void GameConfigData::setCameraStickSensitivityLevel(s32 value) {
    mCameraStickSensitivityLevel = value;
}

bool GameConfigData::isValidCameraGyro() const {
    return mIsValidCameraGyro;
}

void GameConfigData::validateCameraGyro() {
    mIsValidCameraGyro = true;
}

void GameConfigData::invalidateCameraGyro() {
    mIsValidCameraGyro = false;
}

s32 GameConfigData::getCameraGyroSensitivityLevel() const {
    return mCameraGyroSensitivityLevel;
}

void GameConfigData::setCameraGyroSensitivityLevel(s32 value) {
    mCameraGyroSensitivityLevel = value;
}

bool GameConfigData::isUseOpenListAdditionalButton() const {
    return mIsUseOpenListAdditionalButton;
}

void GameConfigData::onUseOpenListAdditionalButton() {
    mIsUseOpenListAdditionalButton = true;
}

void GameConfigData::offUseOpenListAdditionalButton() {
    mIsUseOpenListAdditionalButton = false;
}

bool GameConfigData::isValidPadRumble() const {
    return mIsValidPadRumble;
}

void GameConfigData::validatePadRumble() {
    mIsValidPadRumble = true;
}

void GameConfigData::invalidatePadRumble() {
    mIsValidPadRumble = false;
}

s32 GameConfigData::getPadRumbleLevel() const {
    return mPadRumbleLevel;
}

void GameConfigData::setPadRumbleLevel(s32 value) {
    mPadRumbleLevel = value;
}

void GameConfigData::write(al::ByamlWriter* writer) {
    writer->pushHash("GameConfigData");
    writer->addInt("CameraStickSensitivityLevel", mCameraStickSensitivityLevel);
    writer->addBool("IsCameraReverseInputH", mIsCameraReverseInputH);
    writer->addBool("IsCameraReverseInputV", mIsCameraReverseInputV);
    writer->addBool("IsValidCameraGyro", mIsValidCameraGyro);
    writer->addInt("CameraGyroSensitivityLevel", mCameraGyroSensitivityLevel);
    writer->addBool("IsUseOpenListAdditionalButton", mIsUseOpenListAdditionalButton);
    writer->addBool("IsPadRumble", mIsValidPadRumble);
    writer->addInt("PadRumbleLevel", mPadRumbleLevel);
    writer->pop();
}

void GameConfigData::read(const al::ByamlIter& save) {
    mCameraStickSensitivityLevel = -1;
    mIsCameraReverseInputH = false;
    mIsCameraReverseInputV = false;
    mIsValidCameraGyro = true;
    mCameraGyroSensitivityLevel = -1;
    mIsUseOpenListAdditionalButton = false;
    mIsValidPadRumble = true;
    mPadRumbleLevel = 0;

    al::ByamlIter iter;
    al::tryGetByamlIterByKey(&iter, save, "GameConfigData");
    al::tryGetByamlS32(&mCameraStickSensitivityLevel, iter, "CameraStickSensitivityLevel");
    al::tryGetByamlBool(&mIsCameraReverseInputH, iter, "IsCameraReverseInputH");
    al::tryGetByamlBool(&mIsCameraReverseInputV, iter, "IsCameraReverseInputV");
    al::tryGetByamlBool(&mIsValidCameraGyro, iter, "IsValidCameraGyro");
    al::tryGetByamlS32(&mCameraGyroSensitivityLevel, iter, "CameraGyroSensitivityLevel");
    al::tryGetByamlBool(&mIsUseOpenListAdditionalButton, iter, "IsUseOpenListAdditionalButton");
    al::tryGetByamlBool(&mIsValidPadRumble, iter, "IsPadRumble");
    al::tryGetByamlS32(&mPadRumbleLevel, iter, "PadRumbleLevel");
}
