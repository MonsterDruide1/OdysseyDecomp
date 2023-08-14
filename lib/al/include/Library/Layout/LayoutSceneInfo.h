#pragma once

namespace eui {
class FontMgr;
}

namespace al {
class CameraDirector;
class PadRumbleDirector;
class SceneObjHolder;
class MessageSystem;
class GamePadSystem;

class LayoutSceneInfo {
public:
    LayoutSceneInfo();
    eui::FontMgr* getFontMgr() const { return mFontMgr; }
    CameraDirector* getCameraDirector() const { return mCameraDirector; }
    PadRumbleDirector* getPadRumbleDirector() const { return mPadRumbleDirector; }
    SceneObjHolder* getSceneObjHolder() const { return mSceneObjHolder; }
    const MessageSystem* getMessageSystem() const { return mMessageSystem; }
    const GamePadSystem* getGamePadSystem() const { return mGamePadSystem; }

private:
    eui::FontMgr* mFontMgr = nullptr;
    CameraDirector* mCameraDirector = nullptr;
    PadRumbleDirector* mPadRumbleDirector = nullptr;
    SceneObjHolder* mSceneObjHolder = nullptr;
    const MessageSystem* mMessageSystem = nullptr;
    const GamePadSystem* mGamePadSystem = nullptr;
};
}  // namespace al
