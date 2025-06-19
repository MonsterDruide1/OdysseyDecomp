#pragma once

#include <basis/seadTypes.h>
#include <container/seadTList.h>

namespace sead {
class WriteStream;
}

namespace al {

class ByamlWriterStringTable {
public:
    ByamlWriterStringTable();
    virtual ~ByamlWriterStringTable();
    const char* tryAdd(const char* string);
    u32 calcHeaderSize() const;
    u32 calcContentSize() const;
    u32 calcPackSize() const;
    bool isEmpty() const;
    u32 calcIndex(const char* data) const;
    void write(sead::WriteStream* stream) const;
    void print() const;

private:
    sead::TList<const char*> mList;
};

}  // namespace al
