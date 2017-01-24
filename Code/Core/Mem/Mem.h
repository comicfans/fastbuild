// Mem.h
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Core/Env/Types.h"
#include "Core/Mem/MemTracker.h"
#include <new>

// Placement new/delete
//------------------------------------------------------------------------------
#define INPLACE_NEW new
#ifndef new
//inline void * operator new( size_t, void * ptr ) { return ptr; }
//inline void * operator new[]( size_t, void * ptr ) { return ptr; }
//inline void operator delete( void *, void * ) noexcept {}
//inline void operator delete[]( void *, void * ) noexcept {}
#endif

// new/delete
//------------------------------------------------------------------------------
#if defined( MEMTRACKER_ENABLED )
    #define FNEW( code )        new ( __FILE__, __LINE__ ) code
    #define FNEW_ARRAY( code )  new ( __FILE__, __LINE__ ) code
    #define FDELETE             delete
    #define FDELETE_ARRAY       delete[]

    #define ALLOC( ... )        ::AllocFileLine( __VA_ARGS__, __FILE__, __LINE__ )
    #define FREE( ptr )         ::Free( ptr )
#else
    #define FNEW( code )        new code
    #define FNEW_ARRAY( code )  new code
    #define FDELETE             delete
    #define FDELETE_ARRAY       delete[]

    #define ALLOC( ... )        ::Alloc( __VA_ARGS__ )
    #define FREE( ptr )         ::Free( ptr )
#endif

// Alloc/Free
//------------------------------------------------------------------------------
void * Alloc( size_t size );
void * Alloc( size_t size, size_t alignment );
void * AllocFileLine( size_t size, const char * file, int line );
void * AllocFileLine( size_t size, size_t alignment, const char * file, int line );
void Free( void * ptr );

// global new/delete
//------------------------------------------------------------------------------
#if defined( MEMTRACKER_ENABLED )
    void * operator new( size_t size, const char * file, int line ) ;
    void * operator new[]( size_t size, const char * file, int line ) ;
    void operator delete( void * ptr, const char *, int ) noexcept;
    void operator delete[]( void * ptr, const char *, int ) noexcept;
#elif not defined (new)
//void * operator new( size_t size ) ;
//void * operator new[]( size_t size ) ;
//void operator delete( void * ptr ) noexcept;
//void operator delete[]( void * ptr ) noexcept;
#endif

//------------------------------------------------------------------------------
