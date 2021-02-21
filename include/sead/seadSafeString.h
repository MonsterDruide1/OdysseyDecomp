#pragma once

namespace sead
{
    template<typename T>
    class SafeStringBase
    {
    public:
        SafeStringBase(const T* pStr) : mTop(pStr) { }
        virtual ~SafeStringBase();

        virtual SafeStringBase<T>& operator=(const SafeStringBase<T> &);
        virtual void assureTerminationImpl_() const;

        inline int calcLength() const;

        const T* cstr() const
        {
            assureTerminationImpl_();
            return mTop;
        }

        static const T cNullChar;
        static const int cMaximumLength = 0x80000;

    protected:
        const T* mTop; // _8
    };

    template<typename T>
    class BufferedSafeStringBase : public SafeStringBase<T>
    {
    public:
        __attribute__((always_inline))
        BufferedSafeStringBase(T* buf, int size)
            : SafeStringBase<T>(buf)
        {
            mSize = size;

            if (size <= 0)
            {
                this->mTop = 0;
                this->mSize = 0;
            }
            else
            {
                assureTerminationImpl_();
            }
        };

        virtual ~BufferedSafeStringBase();

        virtual BufferedSafeStringBase<T>& operator=(const SafeStringBase<T> &);
        virtual void assureTerminationImpl_() const;

        inline int copy(const SafeStringBase<T>& src, int copyLength = -1);

        T* getBuffer()
        {
            assureTerminationImpl_();
            return getMutableStringTop_();
        }

        T* getMutableStringTop_() 
        { 
            return const_cast<T*>(this->mTop); 
        }

        inline void clear() 
        { 
            getMutableStringTop_()[0] = this->cNullChar; 
        }

        int mSize; // _10
    };

    template<typename T, int Len>
    class FixedSafeStringBase : public BufferedSafeStringBase<T>
    {
    public:
        FixedSafeStringBase(const SafeStringBase<T>& rStr) : BufferedSafeStringBase<T>(mStrBuffer, Len)
        {
            this->copy(rStr);
        }

        T mStrBuffer[Len]; // _18
    };

    template<int Len>
    class FixedSafeString : public FixedSafeStringBase<char, Len>
    {
    public:
        FixedSafeString(const SafeStringBase<char> &rStr) : FixedSafeStringBase<char, Len>(rStr) { }

        virtual ~FixedSafeString();
    };
};