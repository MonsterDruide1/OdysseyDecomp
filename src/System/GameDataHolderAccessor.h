#pragma once

namespace al {
class IUseSceneObjHolder;
class SceneObjHolder;
}  // namespace al
class SaveObjInfo;
class GameDataHolder;

class GameDataHolderAccessor {
public:
    GameDataHolderAccessor(const al::IUseSceneObjHolder*);
    GameDataHolderAccessor(const al::SceneObjHolder*);

    GameDataHolderAccessor(GameDataHolder* holder) { mData = holder; }

    operator GameDataHolder*() const { return mData; }

    GameDataHolder* operator->() const { return mData; }

private:
    GameDataHolder* mData;
};
