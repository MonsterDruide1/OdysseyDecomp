#include "game/System/GameConfigData.h"

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

int GameConfigData::getCameraStickSensitivityLevel() const {
    return mCameraStickSensitivityLevel;
}

void GameConfigData::setCameraStickSensitivityLevel(int value) {
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

int GameConfigData::getCameraGyroSensitivityLevel() const {
    return mCameraGyroSensitivityLevel;
}

void GameConfigData::setCameraGyroSensitivityLevel(int value) {
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

int GameConfigData::getPadRumbleLevel() const {
    return mPadRumbleLevel;
}

void GameConfigData::setPadRumbleLevel(int value) {
    mPadRumbleLevel = value;
}

void GameConfigData::write(al::ByamlWriter *writer) {
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

void GameConfigData::read(al::ByamlIter const &conf) {
    mCameraStickSensitivityLevel = -1;
    mIsCameraReverseInputH = false;
    mIsCameraReverseInputV = false;
    mIsValidCameraGyro = true;
    mCameraGyroSensitivityLevel = -1;
    mIsUseOpenListAdditionalButton = false;
    mIsValidPadRumble = true;
    mPadRumbleLevel = 0;

    al::ByamlIter iter;
    al::tryGetByamlIterByKey(&iter, conf, "GameConfigData");
    al::tryGetByamlS32(&mCameraStickSensitivityLevel, iter, "CameraStickSensitivityLevel");
    al::tryGetByamlBool(&mIsCameraReverseInputH, iter, "IsCameraReverseInputH");
    al::tryGetByamlBool(&mIsCameraReverseInputV, iter, "IsCameraReverseInputV");
    al::tryGetByamlBool(&mIsValidCameraGyro, iter, "IsValidCameraGyro");
    al::tryGetByamlS32(&mCameraGyroSensitivityLevel, iter, "CameraGyroSensitivityLevel");
    al::tryGetByamlBool(&mIsUseOpenListAdditionalButton, iter, "IsUseOpenListAdditionalButton");
    al::tryGetByamlBool(&mIsValidPadRumble, iter, "IsPadRumble");
    al::tryGetByamlS32(&mPadRumbleLevel, iter, "PadRumbleLevel");
}
