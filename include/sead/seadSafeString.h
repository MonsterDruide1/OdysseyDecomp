#pragma once

namespace sead
{
    template<typename T>
    class SafeStringBase
    {
    public:
        virtual ~SafeStringBase();

        virtual SafeStringBase<T>& operator=(const SafeStringBase<T> &);
        virtual void assureTerminationImpl_() const;

        const T* mTop; // _8
    };

    template<typename T>
    class BufferedSafeStringBase : public SafeStringBase<T>
    {
    public:
        virtual ~BufferedSafeStringBase();

        virtual BufferedSafeStringBase<T>& operator=(const SafeStringBase<T> &);
        virtual void assureTerminationImpl_() const;

        int mSize; // _10
    };

    template<typename T, int Len>
    class FixedSafeStringBase : public BufferedSafeStringBase<T>
    {
    public:
        T mStrBuffer[Len]; // _18
    };

    template<int Len>
    class FixedSafeString : public FixedSafeStringBase<char, Len>
    {
    public:
        virtual ~FixedSafeString();
    };
};