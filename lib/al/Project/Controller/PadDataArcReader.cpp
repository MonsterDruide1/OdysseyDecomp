#include "Project/Controller/PadDataArcReader.h"

#include "Library/Controller/PadDataPack.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceFunction.h"

namespace al {

PadDataArcReader::PadDataArcReader(const char* path) : mPath(path) {
    findOrCreateResource(path, nullptr);
}

PadDataArcReader::PadDataArcReader(const char* path, const char* resourceName) : mPath(path) {
    readResource(resourceName);
}

void PadDataArcReader::readResource(const char* resourceName) {
    mCursorFrame = 0;
    Resource* resource = findOrCreateResource(mPath, nullptr);
    sead::FixedSafeString<256> filename;
    filename.format("%s.bin", resourceName);
    mDataFrames = (PadDataPack*)resource->getOtherFile(filename);
    checkEnd();
    PadDataPack* checkFrame = mDataFrames;
    while (checkFrame->trig != -1)
        checkFrame++;
    mTotalFrame = checkFrame - mDataFrames;
}

void PadDataArcReader::checkEnd() {
    if (mDataFrames[mCursorFrame].trig == -1)
        mIsEnd = true;
}

void PadDataArcReader::read(PadDataPack* frameData) {
    if (mIsEnd)
        return;

    *frameData = mDataFrames[mCursorFrame++];
    checkEnd();
}

}  // namespace al
