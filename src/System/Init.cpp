#include <heap/seadHeapMgr.h>
#include <nn/init.h>
#include <nn/mem.h>
#include <nn/oe.h>

#include "System/Application.h"

extern "C" void nninitStartup() {
    uintptr_t allocatorHeap;
    uintptr_t recordingHeap;

    nn::os::SetMemoryHeapSize(0xc8000000);
    nn::os::AllocateMemoryBlock(&allocatorHeap, 0x2400000);
    nn::init::InitializeAllocator(reinterpret_cast<void*>(allocatorHeap), 0x2400000);
    nn::os::AllocateMemoryBlock(&recordingHeap, 0x6000000);
    nn::oe::EnableGamePlayRecording(reinterpret_cast<void*>(recordingHeap), 0x6000000);
}

extern "C" void nnMain() {
    s32 argc = nn::os::GetHostArgc();
    char** argv = nn::os::GetHostArgv();
    ApplicationFunction::initialize(argc, argv);
    if (sead::HeapMgr::getRootHeapNum() == 0 || !sead::HeapMgr::getRootHeap(0))
        Application::createInstance(nullptr);
    else {
        sead::ScopedCurrentHeapSetter setter(sead::HeapMgr::getRootHeap(0));
        Application::createInstance(nullptr);
    }
    Application::instance()->init(argc, argv);
    Application::instance()->run();
}
