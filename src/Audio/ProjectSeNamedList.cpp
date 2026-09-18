#include "Audio/ProjectSeNamedList.h"

static const char* const sMuteSeInPVList[] = {
    "WsEmMeganeOnStart",
    "WsEvMeganeOnStart",
    "SeMoMeElecWireMoveLv",
    "SeBmMeBossKnuckleHackMoveLv",
    "SeBmMeBossKnuckleHackMoveFastLv",
    "SeMoMeCapFlowerOpen",
    "SeMoMeCapFlowerOpenAll",
    "WsUiTalkUiAppearSquare",
    "WsdSyMapWindowSubClose",
    "RcWsdSyTestWindowClose",
    "WsSyJumpingRopeCount",
    "RcWsdSyTestWindowAppear",
    "WsdSyTestWindowAppear",
    "RcSeGmCsAppear",
    "WsSyPlayerLifeAlertTrg",
    "WsUiInvalid03",
    "WsdSySelectButtonMove",
    "WsUiSkip",
    "WsNvCapManHeroTalkInfo_",
    "SeBmMe",
    "SeMoMe",
    "SeEmMe",
};

ProjectSeNamedList::ProjectSeNamedList() = default;

s32 ProjectSeNamedList::getMuteSeInPVListSize() {
    return 0x16;
}

const char* const* ProjectSeNamedList::getMuteSeInPVList() {
    return sMuteSeInPVList;
}
