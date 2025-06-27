#pragma once

#include <basis/seadTypes.h>
#include <container/seadTList.h>

namespace sead {
class WriteStream;
}

namespace al {

class ByamlWriterBigData;

class ByamlWriterBigDataList {
public:
    ByamlWriterBigDataList();
    virtual ~ByamlWriterBigDataList();
    u32 calcPackSize() const;
    void addData(ByamlWriterBigData* data);
    s32 setOffset(s32 offset);
    void write(sead::WriteStream* stream);

private:
    sead::TList<ByamlWriterBigData*> mList;
};

}  // namespace al
