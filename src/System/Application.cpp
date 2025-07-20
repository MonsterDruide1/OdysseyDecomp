#include "System/Application.h"

#include <System/RootTask.h>
#include "Library/Base/StringUtil.h"
#include "Library/Framework/GameFrameworkNx.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Message/LanguageUtil.h"
#include "Library/Network/AccountHolder.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/SaveData/SaveDataFunction.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Shader/ForwardRendering/ShaderHolder.h"
#include "Library/System/GameSystemInfo.h"
#include "Library/System/SystemKit.h"
#include "Project/Memory/MemorySystem.h"
#include "System/GameDataHolder.h"
#include "framework/seadTaskMgr.h"
#include "heap/seadExpHeap.h"
#include "heap/seadFrameHeap.h"
#include "heap/seadHeapMgr.h"

SEAD_SINGLETON_DISPOSER_IMPL(Application)

static const sead::TaskFactory sFactory = sead::TTaskFactory<RootTask>;

const s32 cDefaultPriority = sead::Thread::cDefaultPriority;
const s32 cFileLoaderThreadPriority = cDefaultPriority + 1;
const s32 cSaveSystemThreadPriority = cDefaultPriority + 4;
const s32 cResourceSystemThreadPriority = cDefaultPriority + 2;

Application::Application() = default;

void Application::init(s32 argc, char* argv[]) {
    {
        sead::Heap* rootHeap = sead::HeapMgr::getRootHeap(0);
        sead::ScopedCurrentHeapSetter scopedSetter(rootHeap);

        mSystemKit = new al::SystemKit();
        mSystemKit->createMemorySystem(rootHeap);
    }

    sead::ScopedCurrentHeapSetter scopedSetter(mSystemKit->getMemorySystem()->getStationedHeap());

    sead::Heap* audioHeap = sead::ExpHeap::create(0x12C00000, "AudioHeap", nullptr, 8,
                                                  sead::Heap::cHeapDirection_Forward, false);
    al::addNamedHeap(audioHeap, nullptr);
    sead::Heap* audioStationedResourceHeap =
        sead::ExpHeap::create(0x11800000, "AudioStationedResourceHeap", audioHeap, 8,
                              sead::Heap::cHeapDirection_Forward, false);
    al::addNamedHeap(audioStationedResourceHeap, nullptr);

    {
        // TODO: Issue 1: This struct has different codegen
        sead::GameFrameworkNx::CreateArg createArg = {
            .mVBlankWaitInterval = 1,
            .mClearColor = sead::Color4f::cBlack,
            .mDisplayResolution = sead::Vector2u(1600, 900),
            ._1c = false,
            .mIsTripleBuffer = true,
            .mIsUseDebug = false,
            ._1f = false,
            ._20 = 0x80000,
            ._24 = 0x80000,
            ._28 = 0x1000,
            .mTransferMemorySize = 0x830000,
            .mGraphicsDevToolsMemorySize = 0,
            .mComputeMemorySize = -1,
            .mCommandMemorySize = 0x20000,
            .mControlMemorySize = 0x40000,
            .mDebugLevel = 0,
            ._44 = 16684,
        };
        mGameFramework = new al::GameFrameworkNx(createArg);

        mGameFramework->initializeGraphicsSystem(
            sead::HeapMgr::instance()->getCurrentHeap(),
            {(f32)al::getVirtualDisplayWidth(), (f32)al::getVirtualDisplayHeight()});
        mGameFramework->initAgl(sead::HeapMgr::instance()->getCurrentHeap(),
                                al::getVirtualDisplayWidth(), al::getVirtualDisplayHeight(), 1600,
                                900, 1280, 720);

        // TODO: Issue 2: This struct creation has different codegen
        mDrawSystemInfo = new al::DrawSystemInfo{mGameFramework->getDockedRenderBuffer(),
                                                 mGameFramework->getHandheldRenderBuffer(), false,
                                                 mGameFramework->getDrawContext()};

        mGameFramework->requestChangeUseGPU(false);
    }

    {
        mAccountHolder = new al::AccountHolder();
        al::initRegionAndLanguage();
        mSystemKit->createSaveDataSystem(0xA00400, cSaveSystemThreadPriority);

        GameDataHolder dataHolder;
        al::initSaveDirSync("NoUse.bin", 0xA00400, 1);
        al::readSaveDataSync("Common.bin", 0x400, 1);

        if (al::isSuccessSaveDataSequence()) {
            dataHolder.readFromSaveDataBufferCommonFileOnlyLanguage();
            if (dataHolder.getLanguage() && !al::isEqualString(dataHolder.getLanguage(), ""))
                al::forceInitLanguage(dataHolder.getLanguage());
        }

        if (al::isEqualString(al::getLanguageString(), "EUen"))
            al::forceInitLanguage("USen");
    }

    mSystemKit->createFileLoader(cFileLoaderThreadPriority);
    mSystemKit->createResourceSystem(nullptr, cResourceSystemThreadPriority, 0x400000, true);

    sead::Heap* stationedResourceHeap = sead::ExpHeap::create(
        0x2D500000, "StationedResourceHeap", mSystemKit->getMemorySystem()->getStationedHeap(), 8,
        sead::Heap::cHeapDirection_Forward, false);
    al::addNamedHeap(stationedResourceHeap, nullptr);
    al::addResourceCategory("常駐[システム]", 16, stationedResourceHeap);

    sead::Heap* localizeResourceHeap = sead::FrameHeap::create(
        0x1400000, "LocalizeResourceHeap", mSystemKit->getMemorySystem()->getStationedHeap(), 8,
        sead::Heap::cHeapDirection_Forward, false);
    al::addNamedHeap(localizeResourceHeap, "LocalizeResourceHeap");
    al::addResourceCategory("常駐[ローカライズ]", 80, al::findNamedHeap("LocalizeResourceHeap"));
    al::addResourceCategory("常駐", 512, stationedResourceHeap);
    al::addResourceCategory("常駐[シェーダー]", 64, al::findNamedHeap("StationedResourceHeap"));

    al::ShaderHolder::createInstance(nullptr);

    al::createCategoryResourceAll("常駐[シェーダー]");
    sead::Heap* audioStationedHeap = al::findNamedHeap("AudioStationedResourceHeap");
    al::addResourceCategory("常駐[オーディオ]", 55, audioStationedHeap);
}

void Application::run() {
    sead::TaskBase::CreateArg createArg = {{sead::TaskClassID::cFactory, {.mFactory = sFactory}}};
    mGameFramework->run(mSystemKit->getMemorySystem()->getStationedHeap(), createArg, {});
}

RootTask* Application::getRootTask() const {
    return static_cast<RootTask*>(mGameFramework->mTaskMgr->mRootTask);
}

namespace ApplicationFunction {

void initialize(s32 argc, char** argv) {
    sead::Framework::InitializeArg arg = {};
    arg.heap_size = 0xBFC00000;
    sead::GameFrameworkNx::initialize(arg);
}

}
