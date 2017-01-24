// Murmur3.h
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Core/Env/Types.h"
#include "Core/Env/Assert.h"
#include "Core/Strings/AString.h"

// avoid including xxhash header directly
extern "C"
{
    unsigned int XXH32( const void * input, size_t length, unsigned seed );
    unsigned long long XXH64( const void * input, size_t length, unsigned long long seed );

    typedef struct XXH64_state_s XXH64_state_t;   /* incomplete type */

    typedef enum { XXH_OK=0, XXH_ERROR } XXH_errorcode;
    XXH64_state_t* XXH64_createState(void);
    int XXH64_freeState(XXH64_state_t* statePtr);
    XXH_errorcode XXH64_reset  (XXH64_state_t* statePtr, unsigned long long seed);
    XXH_errorcode XXH64_update (XXH64_state_t* statePtr, const void* input, size_t length);
    uint64_t XXH64_digest (const XXH64_state_t* statePtr);
};

template<typename T>
class Array;

// xxHash
//------------------------------------------------------------------------------
class xxHash
{
public:
    inline static uint32_t  Calc32( const void * buffer, size_t len );
    inline static uint64_t  Calc64( const void * buffer, size_t len );

    inline static uint32_t  Calc32( const AString & string ) { return Calc32( string.Get(), string.GetLength() ); }
    inline static uint64_t  Calc64( const AString & string ) { return Calc64( string.Get(), string.GetLength() ); }
    enum { XXHASH_SEED = 0x0 }; // arbitrarily chosen random seed
};

class xxHash64Stream
{
    public:
        xxHash64Stream()
        {
            m_State = XXH64_createState();
            XXH64_reset (m_State, xxHash::XXHASH_SEED);
        }
        ~xxHash64Stream()
        {
            ASSERT (!m_State);
        }

        void Update(const void* buffer, size_t len)
        {
            ASSERT (m_State);
            XXH64_update (m_State,buffer, len);
        }

        void Update(const AString & string )
        {
            ASSERT (m_State);
            XXH64_update (m_State, string.Get(), string.GetLength());
        }

        template <typename T>
        void Update(const Array<T>& array)
        {
            ASSERT (m_State);
            for(int i=0,size=array.GetSize();i<size;++i){
                Update(array[i]);
            }
        }

        template <typename T>
            void Update(const T v)
            {
                ASSERT (m_State);
                XXH64_update (m_State, const_cast<T*>(&v) , sizeof(T));
            }

        uint64_t Digest()const
        {
            ASSERT (m_State);
            uint64_t ret = XXH64_digest(m_State);
            XXH64_freeState(m_State);
            m_State = nullptr;
            return ret;
        }

    private:
        xxHash64Stream(const xxHash64Stream&);
        mutable XXH64_state_t* m_State;
};

// Calc32
//------------------------------------------------------------------------------
/*static*/ uint32_t xxHash::Calc32( const void * buffer, size_t len )
{
    return XXH32( buffer, len, XXHASH_SEED );
}

// Calc64
//------------------------------------------------------------------------------
/*static*/ uint64_t xxHash::Calc64( const void * buffer, size_t len )
{
    return XXH64( buffer, len, XXHASH_SEED );
}

//------------------------------------------------------------------------------
