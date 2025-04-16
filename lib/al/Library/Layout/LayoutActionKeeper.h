#pragma once

namespace al {
class IUseAudioKeeper;
class IUseEffectKeeper;
class LayoutKeeper;
class LayoutPaneGroup;

class LayoutActionKeeper {
public:
    LayoutActionKeeper(LayoutKeeper*, IUseAudioKeeper*, IUseEffectKeeper*);
    void startAction(const char* actionName, const char* groupName);
    void findPaneGroupInfo(const char* groupName);
    void update();
    void setMainGroupName(const char* groupName);
    LayoutPaneGroup* getLayoutPaneGroup(const char* groupName);

private:
    bool mHasStartedAction;
    LayoutPaneGroup* mLayoutPaneGroups;
    u32 mPaneGroupNum;
    const char* mMainGroupName;
};
}  // namespace al
