#include "Scene/SceneObjFactory.h"

#include "Library/Scene/ISceneObj.h"
#include "Library/Scene/SceneObjHolder.h"
#include "Library/Stage/StageRhythm.h"

#include "Amiibo/AmiiboNpcDirector.h"
#include "Amiibo/HelpAmiiboDirector.h"
#include "Item/CoinCollectHolder.h"
#include "Item/CoinCollectWatcher.h"
#include "Layout/KidsModeLayoutAccessor.h"
#include "MapObj/RouteGuideDirector.h"
#include "Scene/HintPhotoLayoutHolder.h"

static al::ISceneObj* sceneObjCreator(s32 id) {
    switch (id) {
    case SceneObjID_AmiiboNpcDirector:
        return new AmiiboNpcDirector();

    case SceneObjID_BgmAnimeSyncDirector:
        return nullptr;

    case SceneObjID_CapManHeroDemoDirector:
        return nullptr;

    case SceneObjID_CapMessageDirector:
        return nullptr;

    case SceneObjID_CapMessageMoonNotifier:
        return nullptr;

    case SceneObjID_CoinCollectHolder:
        return new CoinCollectHolder();

    case SceneObjID_CoinCollectWatcher:
        return new CoinCollectWatcher();

    case SceneObjID_CollectBgmPlayer:
        return nullptr;

    case SceneObjID_EchoEmitterHolder:
        return nullptr;

    case SceneObjID_ElectricWireCameraTicketHolder:
        return nullptr;

    case SceneObjID_FukankunZoomObjHolder:
        return nullptr;

    case SceneObjID_GrowPlantDirector:
        return nullptr;

    case SceneObjID_GuidePosInfoHolder:
        return nullptr;

    case SceneObjID_HintPhotoLayoutHolder:
        return new HintPhotoLayoutHolder();

    case SceneObjID_HtmlViewerRequester:
        return nullptr;

    case SceneObjID_KidsModeLayoutAccessor:
        return new KidsModeLayoutAccessor();

    case SceneObjID_LoginLotteryDirector:
        return nullptr;

    case SceneObjID_MoviePlayer:
        return nullptr;

    case SceneObjID_PaintObjHolder:
        return nullptr;

    case SceneObjID_PlayerStartInfoHolder:
        return nullptr;

    case SceneObjID_QuestInfoHolder:
        return nullptr;

    case SceneObjID_RandomItemSelector:
        return nullptr;

    case SceneObjID_ReactionObjectActionIndexHolder:
        return nullptr;

    case SceneObjID_RhyhtmInfoWatcher:
        return nullptr;

    case SceneObjID_RouteGuideDirector:
        return new RouteGuideDirector();

    case SceneObjID_SceneEventNotifier:
        return nullptr;

    case SceneObjID_alStageSyncCounter:
        return new al::StageSyncCounter();

    case SceneObjID_TalkNpcParamHolder:
        return nullptr;

    case SceneObjID_TalkNpcSceneEventSwitcher:
        return nullptr;

    case SceneObjID_TestStageTimeDirector:
        return nullptr;

    case SceneObjID_TimeBalloonDirector:
        return nullptr;

    case SceneObjID_TsukkunTraceHolder:
        return nullptr;

    case SceneObjID_WipeHolderRequester:
        return nullptr;

    case SceneObjID_YoshiFruitWatcher:
        return nullptr;

    case SceneObjID_HelpAmiiboDirector:
        return new HelpAmiiboDirector();

    default:
        return nullptr;
    }
}

al::SceneObjHolder* SceneObjFactory::createSceneObjHolder() {
    return new al::SceneObjHolder(&sceneObjCreator, SceneObjID_Max);
}
