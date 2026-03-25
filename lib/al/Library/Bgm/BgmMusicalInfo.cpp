#include "Library/Bgm/BgmMusicalInfo.h"

#include "Library/Audio/AudioInfo.h"
#include "Library/Bgm/BgmResourceCategoryInfo.h"
#include "Library/Yaml/ByamlIter.h"
#include "Project/Bgm/BgmInfo.h"

namespace al {

BgmMusicalInfo* BgmMusicalInfo::createInfo(const ByamlIter& iter, const char* name) {
    BgmMusicalInfo* info = new BgmMusicalInfo();
    info->name = name;

    ByamlIter animListIter;
    if (iter.tryGetIterByKey(&animListIter, "AnimList"))
        info->rhythmInfoList = createAudioInfoList<BgmRhythmInfo>(animListIter, 0);

    ByamlIter chordListIter;
    if (iter.tryGetIterByKey(&chordListIter, "ChordList"))
        info->chordInfoList = createAudioInfoList<BgmChordInfo>(chordListIter, 0);

    ByamlIter bpmListIter;
    if (iter.tryGetIterByKey(&bpmListIter, "BpmInfoList"))
        info->bpmInfoList = createAudioInfoList<BgmBpmInfo>(bpmListIter, 0);

    ByamlIter timeSignatureListIter;
    if (iter.tryGetIterByKey(&timeSignatureListIter, "TimeSignatureInfoList"))
        info->timeSignatureInfoList =
            createAudioInfoList<BgmTimeSignatureInfo>(timeSignatureListIter, 0);

    iter.tryGetFloatByKey(&info->beatStartOffsetTime, "BeatStartOffsetTime");
    return info;
}

s32 BgmMusicalInfo::compareInfo(const BgmMusicalInfo* lhs, const BgmMusicalInfo* rhs) {
    return strcmp(lhs->name, rhs->name);
}

BgmMusicalInfo::BgmMusicalInfo() = default;

BgmMusicalInfo::BgmMusicalInfo(const BgmMusicalInfo& info)
    : name(info.name), beatStartOffsetTime(info.beatStartOffsetTime) {
    rhythmInfoList = copyAudioInfoList(info.rhythmInfoList, 0);
    chordInfoList = copyAudioInfoList(info.chordInfoList, 0);
    bpmInfoList = copyAudioInfoList(info.bpmInfoList, 0);
    timeSignatureInfoList = copyAudioInfoList(info.timeSignatureInfoList, 0);
}

}  // namespace al
