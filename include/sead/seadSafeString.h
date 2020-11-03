#pragma once

namespace sead
{
    template<typename T>
    class SafeStringBase
    {
    public:
        virtual ~SafeStringBase();
        virtual SafeStringBase<T> operator=(const SafeStringBase<T> &);
        virtual void assureTerminationImpl_() const;

        T* mStringTop; // _8
    };

    template<typename T>
    class BufferedSafeStringBase : public SafeStringBase<T>
    {
    public:
        virtual BufferedSafeStringBase<T> operator=(const BufferedSafeStringBase<T> &);
        virtual void assureTerminationImpl_() const;

        bool format(const char *, ...);

        int mBufferSize; // _10
    };

    template<typename T1, int T2>
    class FixedSafeStringBase : public BufferedSafeStringBase<T1>
    {
    public:
        char mBuffer[T2]; // _18
    };
};