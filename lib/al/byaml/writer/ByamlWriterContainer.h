#pragma once

#include <basis/seadTypes.h>
#include "al/byaml/writer/ByamlWriterData.h"

namespace sead {
    class WriteStream;
}

namespace al {

class ByamlWriterHash;
class ByamlWriterArray;

class ByamlWriterContainer : public ByamlWriterData {
public:
    bool isContainer() const override;

    virtual void addBool(const char*,bool);
    virtual void addInt(const char*,s32);
    virtual void addUInt(const char*,u32);
    virtual void addFloat(const char*,float);
    virtual void addInt64(const char*,s64, ByamlWriterBigDataList*);
    virtual void addUInt64(const char*,u64, ByamlWriterBigDataList*);
    virtual void addDouble(const char*,double, ByamlWriterBigDataList*);
    virtual void addString(const char*,const char*);
    virtual void addHash(const char*,ByamlWriterHash*);
    virtual void addArray(const char*,ByamlWriterArray*);
    virtual void addNull(const char*);

    virtual void addBool(bool);
    virtual void addInt(s32);
    virtual void addUInt(u32);
    virtual void addFloat(float);
    virtual void addInt64(s64, ByamlWriterBigDataList*);
    virtual void addUInt64(u64, ByamlWriterBigDataList*);
    virtual void addDouble(double, ByamlWriterBigDataList*);
    virtual void addString(const char*);
    virtual void addHash(ByamlWriterHash*);
    virtual void addArray(ByamlWriterArray*);
    virtual void addNull();

    virtual void writeContainer(sead::WriteStream*) const;
    virtual bool isHash() const;
    virtual bool isArray() const;
    virtual void deleteData();
};

}
