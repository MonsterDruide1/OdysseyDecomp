#pragma once

#include <basis/seadTypes.h>

namespace sead {
    class WriteStream;
}

namespace al {

class ByamlWriterData {
public:
    virtual ~ByamlWriterData();
    virtual void makeIndex();
    virtual u32 calcPackSize() const;
    virtual u8 getTypeCode() const;
    virtual bool isContainer() const;
    virtual void write(sead::WriteStream*) const;
    virtual void print(int) const;
    void printIndent(int) const;
};

class ByamlWriterBool : public ByamlWriterData {
public:
    ByamlWriterBool(bool);
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(int) const override;
private:
    bool mValue;
};

class ByamlWriterInt : public ByamlWriterData {
public:
    ByamlWriterInt(int);
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(int) const override;
private:
    int mValue;
};

class ByamlWriterFloat : public ByamlWriterData {
public:
    ByamlWriterFloat(float);
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(int) const override;
private:
    float mValue;
};

class ByamlWriterUInt : public ByamlWriterData {
public:
    ByamlWriterUInt(u32);
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(int) const override;
private:
    u32 mValue;
};

class ByamlWriterNull : public ByamlWriterData {
public:
    ByamlWriterNull();
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(int) const override;
};



class ByamlWriterBigDataList;

class ByamlWriterBigData : public ByamlWriterData {
public:
    ByamlWriterBigData(al::ByamlWriterBigDataList*);
    ~ByamlWriterBigData();
    void write(sead::WriteStream*) const override;
    virtual u32 calcBigDataSize() const;
    virtual void writeBigData(sead::WriteStream*) const;

    void setOffset(int offset) {mOffset=offset;}
private:
    al::ByamlWriterBigDataList* mList;
    int mOffset = 0;
};

class ByamlWriterInt64 : public ByamlWriterBigData {
public:
    ByamlWriterInt64(long, ByamlWriterBigDataList*);
    ~ByamlWriterInt64();
    u8 getTypeCode() const override;
    void writeBigData(sead::WriteStream*) const override;
    void print(int) const override;
private:
    long mValue;
};

class ByamlWriterUInt64 : public ByamlWriterBigData {
public:
    ByamlWriterUInt64(u64, ByamlWriterBigDataList*);
    ~ByamlWriterUInt64();
    u8 getTypeCode() const override;
    void writeBigData(sead::WriteStream*) const override;
    void print(int) const override;
private:
    u64 mValue;
};

class ByamlWriterDouble : public ByamlWriterBigData {
public:
    ByamlWriterDouble(double, ByamlWriterBigDataList*);
    ~ByamlWriterDouble();
    u8 getTypeCode() const override;
    void writeBigData(sead::WriteStream*) const override;
    void print(int) const override;
private:
    double mValue;
};



class ByamlWriterHash;
class ByamlWriterArray;
class ByamlWriterStringTable;

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
