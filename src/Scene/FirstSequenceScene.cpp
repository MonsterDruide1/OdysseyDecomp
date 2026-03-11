#include "Scene/FirstSequenceScene.h"

#include <common/aglDrawContext.h>
#include <gfx/seadViewport.h>

#include "Library/Audio/AudioDirectorInitInfo.h"
#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Draw/GraphicsFunction.h"
#include "Library/Framework/GameFrameworkNx.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Layout/LayoutSystem.h"
#include "Library/Layout/LayoutUtil.h"
#include "Library/Message/LanguageUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Scene/SceneUtil.h"
#include "Library/System/GameSystemInfo.h"

#include "Layout/BootLayout.h"
#include "Scene/SceneObjFactory.h"
#include "Scene/SceneStateSelectMode.h"
#include "Scene/StageScene.h"
#include "System/Application.h"
#include "System/GameDataFile.h"
#include "System/GameDataFunction.h"
#include "Util/ScenePrepoFunction.h"

namespace {
NERVE_IMPL(FirstSequenceScene, Wipe)
NERVE_IMPL(FirstSequenceScene, SelectMode)

NERVES_MAKE_NOSTRUCT(FirstSequenceScene, Wipe, SelectMode)
}  // namespace

FirstSequenceScene::FirstSequenceScene() : al::Scene("FirstSequenceScene") {}

FirstSequenceScene::~FirstSequenceScene() {
    Application::instance()->getGameFramework()->enableClearRenderBuffer();
}

void FirstSequenceScene::init(const al::SceneInitInfo& info) {
    initDrawSystemInfo(info);

    initSceneObjHolder(SceneObjFactory::createSceneObjHolder());
    al::setSceneObj(this, GameDataFunction::getGameDataHolder(info.gameDataHolder));

    initSceneStopCtrl();

    al::AudioDirectorInitInfo audioDirectorInitInfo;
    al::initAudioDirector2D(this, info, audioDirectorInitInfo);
    alAudioSystemFunction::disableAudioMaximizer(info.gameSystemInfo);
    al::initSceneAudioKeeper(this, info, nullptr);

    initLayoutKit(info);
    getLayoutKit()->createExecuteDirector(0x80);
    al::LayoutInitInfo layoutInitInfo;
    al::initLayoutInitInfo(&layoutInitInfo, this, info);

    mBootLayout = new BootLayout(layoutInitInfo);
    mBootLayout->appear();
    mBootLayout->end();

    initNerve(&Wipe, 1);

    mWaveVibrationHolder = info.gameSystemInfo->waveVibrationHolder;

    mStateSelectMode = new SceneStateSelectMode("モード選択", this, layoutInitInfo, nullptr,
                                                nullptr, mWaveVibrationHolder);
    al::initNerveState(this, mStateSelectMode, &SelectMode, "モード選択");

    getLayoutKit()->endInit();
}

void FirstSequenceScene::appear() {
    al::Scene::appear();

    Application::instance()->getGameFramework()->disableClearRenderBuffer();

    al::startBgm(this, "Stage", -1, 0);
}

void FirstSequenceScene::control() {
    al::isStopScene(this);
}

void FirstSequenceScene::drawMain() const {
    agl::DrawContext* drawContext = al::getSceneDrawContext(this);
    agl::RenderBuffer* framebuffer = al::getSceneFrameBufferMain(this);

    sead::Viewport viewport(*framebuffer);
    viewport.apply(drawContext, *framebuffer);
    framebuffer->bind(drawContext);

    al::setRenderBuffer(getLayoutKit(), framebuffer);
    framebuffer->clear(drawContext, sead::FrameBuffer::cColor | sead::FrameBuffer::cDepth,
                       sead::Color4f::cGray, al::getDepthClearValue(), 0);
    al::setRenderBuffer(getLayoutKit(), framebuffer);

    al::drawLayoutKit(this, "２Ｄバック（メイン画面）");
    al::drawLayoutKit(this, "２Ｄベース（メイン画面）");
    al::drawLayoutKit(this, "２Ｄオーバー（メイン画面）");
}

void FirstSequenceScene::setNoWipe() {
    mBootLayout->endImmidiate();

    al::setNerve(this, &SelectMode);
}

void FirstSequenceScene::exeWipe() {
    rs::updateKitListLayoutOnlyLayoutScene(this);

    if (al::isActionEnd(mBootLayout))
        al::setNerve(this, &SelectMode);
}

void FirstSequenceScene::exeSelectMode() {
    if (al::updateNerveState(this)) {
        GameDataHolder* gameDataHolder = GameDataFunction::getGameDataHolder(this);

        rs::trySavePrepoSettingsState(gameDataHolder->getGameDataFile()->isKidsMode(),
                                      al::getLanguageString(), *gameDataHolder->getGameConfigData(),
                                      GameDataFunction::getSaveDataIdForPrepo(this),
                                      GameDataFunction::getPlayTimeAcrossFile(this));
        kill();
    }
}
