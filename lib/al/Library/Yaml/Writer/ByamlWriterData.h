#pragma once

#include <basis/seadTypes.h>
#include <container/seadTList.h>

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
    virtual void print(s32) const;
    void printIndent(s32) const;
};

class ByamlWriterBool : public ByamlWriterData {
public:
    ByamlWriterBool(bool);
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(s32) const override;

private:
    bool mValue;
};

class ByamlWriterInt : public ByamlWriterData {
public:
    ByamlWriterInt(s32);
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(s32) const override;

private:
    s32 mValue;
};

class ByamlWriterFloat : public ByamlWriterData {
public:
    ByamlWriterFloat(f32);
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(s32) const override;

private:
    f32 mValue;
};

class ByamlWriterUInt : public ByamlWriterData {
public:
    ByamlWriterUInt(u32);
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(s32) const override;

private:
    u32 mValue;
};

class ByamlWriterNull : public ByamlWriterData {
public:
    ByamlWriterNull();
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(s32) const override;
};

class ByamlWriterStringTable;

class ByamlWriterString : public ByamlWriterData {
public:
    ByamlWriterString(const char*, ByamlWriterStringTable*);
    u8 getTypeCode() const override;
    void write(sead::WriteStream*) const override;
    void print(s32) const override;

private:
    const char* mString;
    ByamlWriterStringTable* mStringTable;
};

class ByamlWriterBigDataList;

class ByamlWriterBigData : public ByamlWriterData {
public:
    ByamlWriterBigData(ByamlWriterBigDataList*);
    ~ByamlWriterBigData();
    void write(sead::WriteStream*) const override;
    virtual u32 calcBigDataSize() const;
    virtual void writeBigData(sead::WriteStream*) const;

    void setOffset(s32 offset) { mOffset = offset; }

private:
    ByamlWriterBigDataList* mList;
    s32 mOffset = 0;
};

class ByamlWriterInt64 : public ByamlWriterBigData {
public:
    ByamlWriterInt64(s64, ByamlWriterBigDataList*);
    ~ByamlWriterInt64();
    u8 getTypeCode() const override;
    void writeBigData(sead::WriteStream*) const override;
    void print(s32) const override;

private:
    s64 mValue;
};

class ByamlWriterUInt64 : public ByamlWriterBigData {
public:
    ByamlWriterUInt64(u64, ByamlWriterBigDataList*);
    ~ByamlWriterUInt64();
    u8 getTypeCode() const override;
    void writeBigData(sead::WriteStream*) const override;
    void print(s32) const override;

private:
    u64 mValue;
};

class ByamlWriterDouble : public ByamlWriterBigData {
public:
    ByamlWriterDouble(f64, ByamlWriterBigDataList*);
    ~ByamlWriterDouble();
    u8 getTypeCode() const override;
    void writeBigData(sead::WriteStream*) const override;
    void print(s32) const override;

private:
    f64 mValue;
};

class ByamlWriterHash;
class ByamlWriterArray;
class ByamlWriterStringTable;

class ByamlWriterContainer : public ByamlWriterData {
public:
    bool isContainer() const override;

    virtual void addBool(const char*, bool);
    virtual void addInt(const char*, s32);
    virtual void addUInt(const char*, u32);
    virtual void addFloat(const char*, f32);
    virtual void addInt64(const char*, s64, ByamlWriterBigDataList*);
    virtual void addUInt64(const char*, u64, ByamlWriterBigDataList*);
    virtual void addDouble(const char*, f64, ByamlWriterBigDataList*);
    virtual void addString(const char*, const char*);
    virtual void addHash(const char*, ByamlWriterHash*);
    virtual void addArray(const char*, ByamlWriterArray*);
    virtual void addNull(const char*);

    virtual void addBool(bool);
    virtual void addInt(s32);
    virtual void addUInt(u32);
    virtual void addFloat(f32);
    virtual void addInt64(s64, ByamlWriterBigDataList*);
    virtual void addUInt64(u64, ByamlWriterBigDataList*);
    virtual void addDouble(f64, ByamlWriterBigDataList*);
    virtual void addString(const char*);
    virtual void addHash(ByamlWriterHash*);
    virtual void addArray(ByamlWriterArray*);
    virtual void addNull();

    virtual void writeContainer(sead::WriteStream*) const;
    virtual bool isHash() const;
    virtual bool isArray() const;
    virtual void deleteData();

    s32 getOffset() const { return mOffset; }

    void setOffset(s32 offset) { mOffset = offset; }

private:
    s32 mOffset = 0;  // FIXME shouldn't be public
};

class ByamlWriterArray : public ByamlWriterContainer {
public:
    ByamlWriterArray(ByamlWriterStringTable*);
    ~ByamlWriterArray();

    void deleteData() override;
    u32 calcPackSize() const override;

    void addData(ByamlWriterData*);
    void addBool(bool) override;
    void addInt(s32) override;
    void addUInt(u32) override;
    void addFloat(f32) override;
    void addInt64(s64, ByamlWriterBigDataList*) override;
    void addUInt64(u64, ByamlWriterBigDataList*) override;
    void addDouble(f64, ByamlWriterBigDataList*) override;
    void addString(const char*) override;
    void addHash(ByamlWriterHash*) override;
    void addArray(ByamlWriterArray*) override;
    void addNull() override;

    u8 getTypeCode() const override;
    void writeContainer(sead::WriteStream*) const override;
    void write(sead::WriteStream*) const override;
    void print(s32) const override;
    bool isArray() const override;

private:
    sead::TList<ByamlWriterData*> mList;
    ByamlWriterStringTable* mStringTable;
};

static_assert(sizeof(ByamlWriterArray) == 0x30);

class ByamlWriterHashPair : public sead::ListNode {
public:
    ByamlWriterHashPair(const char*, ByamlWriterData*);

    const char* getKey() { return mKey; }

    ByamlWriterData* getValue() { return mValue; }

private:
    void* mSelfReference = this;
    void* unk = nullptr;
    const char* mKey;
    ByamlWriterData* mValue;
};

static_assert(sizeof(ByamlWriterHashPair) == 0x30);

class ByamlWriterHash : public ByamlWriterContainer {
public:
    ByamlWriterHash(ByamlWriterStringTable*, ByamlWriterStringTable*);
    ~ByamlWriterHash();

    void deleteData() override;  // TODO implementation missing
    u32 calcPackSize() const override;

    void addData(const char*, ByamlWriterData*);  // TODO implementation missing
    void addBool(const char*, bool) override;
    void addInt(const char*, s32) override;
    void addUInt(const char*, u32) override;
    void addFloat(const char*, f32) override;
    void addInt64(const char*, s64, ByamlWriterBigDataList*) override;
    void addUInt64(const char*, u64, ByamlWriterBigDataList*) override;
    void addDouble(const char*, f64, ByamlWriterBigDataList*) override;
    void addString(const char*, const char*) override;
    void addHash(const char*, ByamlWriterHash*) override;
    void addArray(const char*, ByamlWriterArray*) override;
    void addNull(const char*) override;

    u8 getTypeCode() const override;
    void writeContainer(sead::WriteStream*) const override;  // TODO implementation missing
    void write(sead::WriteStream*) const override;
    void print(s32) const override;  // TODO implementation missing
    bool isHash() const override;

private:
    sead::TList<ByamlWriterHashPair> mList;  // TODO not really... it's something different here.
    ByamlWriterStringTable* mStringTable1;
    ByamlWriterStringTable* mStringTable2;
};

static_assert(sizeof(ByamlWriterHash) == 0x38);

}  // namespace al
