#ifndef SEAD_BIT_FLAG_H_
#define SEAD_BIT_FLAG_H_

#include <limits>

#include <basis/seadRawPrint.h>
#include <basis/seadTypes.h>

namespace sead
{
class BitFlagUtil
{
public:
    /// Popcount.
    static int countOnBit(u32 x);
    /// Count trailing zeroes (ctz).
    static int countContinuousOffBitFromRight(u32 x) { return countOnBit((x & -x) - 1); }
    static int countRightOnBit(u32 x, int bit);
    static int findOnBitFromRight(u32 x, int num);

    /// Popcount.
    static int countOnBit64(u64 x)
    {
        return countOnBit(static_cast<u32>(x)) + countOnBit(static_cast<u32>(x >> 32));
    }
    /// Count trailing zeroes (ctz).
    static int countContinuousOffBitFromRight64(u64 x) { return countOnBit64((x & -x) - 1); }
    static int countRightOnBit64(u64 x, int bit);
    static int findOnBitFromRight64(u64 x, int num);
};

template <typename T>
class BitFlag
{
public:
    BitFlag() : mBits(0) {}
    BitFlag(T bits) : mBits(bits) {}

    operator T() const { return mBits; }

    void makeAllZero() { mBits = 0; }
    void makeAllOne() { mBits = std::numeric_limits<T>::max(); }

    void setDirect(T bits) { mBits = bits; }
    T getDirect() const { return mBits; }
    T* getPtr() { return &mBits; }
    const T* getPtr() const { return &mBits; }
    size_t getByteSize() const { return sizeof(T); }

    void set(T val) { mBits |= val; }
    void reset(T val) { mBits &= ~val; }
    void toggle(T val) { mBits ^= val; }
    void change(T val, bool on)
    {
        if (on)
            set(val);
        else
            reset(val);
    }
    bool isZero() const { return mBits == 0; }
    /// Checks if (at least) one of the bits are set.
    bool isOn(T val) const { return (mBits & val) != 0; }
    /// Checks if all of the bits are set.
    bool isOnAll(T val) const { return (mBits & val) == val; }
    bool isOff(T val) const { return !isOn(val); }

    bool testAndClear(T val)
    {
        if (!isOn(val))
            return false;
        reset(val);
        return true;
    }

    // TODO
    T getMask(T v) const;
    static T makeMask(int bit);

    void setBit(int bit) { set(1u << bit); }
    void resetBit(int bit) { reset(1u << bit); }
    void changeBit(int bit, bool on) { change(1u << bit, on); }
    void toggleBit(int bit) { toggle(1u << bit); }
    bool isOnBit(int bit) const { return isOn(1u << bit); }
    bool isOffBit(int bit) const { return isOff(1u << bit); }
    bool testAndClearBit(int bit) { return testAndClear(1u << bit); }

    /// Popcount.
    int countOnBit() const
    {
        if constexpr (sizeof(T) <= 4)
            return BitFlagUtil::countOnBit(mBits);
        else
            return BitFlagUtil::countOnBit64(mBits);
    }
    /// Count trailing zeroes.
    int countContinuousOffBitFromRight() const
    {
        if constexpr (sizeof(T) <= 4)
            return BitFlagUtil::countContinuousOffBitFromRight(mBits);
        else
            return BitFlagUtil::countContinuousOffBitFromRight64(mBits);
    }
    int countRightOnBit(int bit) const
    {
        if constexpr (sizeof(T) <= 4)
            return BitFlagUtil::countRightOnBit(mBits, bit);
        else
            return BitFlagUtil::countRightOnBit64(mBits, bit);
    }
    int findOnBitFromRight(int num) const
    {
        if constexpr (sizeof(T) <= 4)
            return BitFlagUtil::findOnBitFromRight(mBits, num);
        else
            return BitFlagUtil::findOnBitFromRight64(mBits, num);
    }

protected:
    T mBits;
};

typedef BitFlag<u8> BitFlag8;
typedef BitFlag<u16> BitFlag16;
typedef BitFlag<u32> BitFlag32;
typedef BitFlag<u64> BitFlag64;

}  // namespace sead

#endif  // SEAD_BIT_FLAG_H_
