// Gemini Languages and Virtual Machine
// Copyright 2019 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#pragma once

#include "VmCommon.h"

#include <utility>


enum VmError
{
    ERR_NONE,
    ERR_YIELDED,
    ERR_SWITCH_TO_NATIVE,
    ERR_NOT_RUNING,
    ERR_BAD_ARG,
    ERR_BAD_OPCODE,
    ERR_BAD_ADDRESS,
    ERR_BAD_MODULE,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_BYTECODE_NOT_FOUND,
    ERR_NATIVECODE_NOT_FOUND,
    ERR_DIVIDE,
    ERR_NATIVE_ERROR,
    ERR_BOUND,
};


typedef I32 CELL;
typedef U32 UCELL;
typedef uintptr_t UserContext;

struct Module
{
    const U8*       CodeBase;
    CELL*           DataBase;
    U32             CodeSize;
    U16             DataSize;
};

struct ByteCode
{
    const ::Module* Module;
    U32             Address;
};

class Machine;

typedef int (*NativeFunc)( Machine* machine, U8 argc, CELL* args, UserContext context );

struct NativeCode
{
    NativeFunc  Proc;
};

class IEnvironment
{
public:
    virtual bool FindNativeCode( U32 id, NativeCode* nativeCode ) = 0;
    virtual const Module* FindModule( U8 index ) = 0;
};

struct StackFrame
{
    U16             FrameAddr;
    U8              CallFlags;
    U32             RetAddrWord;
};

class Machine : private IEnvironment
{
private:
    static constexpr uint8_t MAX_NATIVE_NESTING = 255;

private:
    CELL*           mSP;
    CELL*           mStack;
    U16             mStackSize;
    U16             mFramePtr;
    IEnvironment*   mEnv;
    UserContext     mScriptCtx;

    NativeFunc      mNativeContinuation;
    UserContext     mNativeContinuationContext;
    U8              mNativeContinuationFlags;
    U8              mNativeNestingLevel;
    U8              mModIndex;
    U32             mPC;
    const Module*   mMod;
    Module          mStackMod;

public:
    Machine();

    void Init( CELL* stack, U16 stackSize, IEnvironment* environment, UserContext scriptCtx = 0 );
    void Init( CELL* stack, U16 stackSize, U8 modIndex, const Module* module, UserContext scriptCtx = 0 );

    bool IsRunning() const;
    UserContext GetScriptContext() const;
    U32 GetPC() const;
    U8 GetModIndex() const;

    CELL* Start( U8 modIndex, U32 address, U8 argCount );
    CELL* Start( CELL addrWord, U8 argCount );
    void Reset();
    int Run();
    int Yield( NativeFunc proc, UserContext context );
    int PushCell( CELL value );
    int PopCell( CELL& value );

private:
    void Init( CELL* stack, U16 stackSize, UserContext scriptCtx );
    StackFrame* PushFrame( const U8* curCodePtr, U8 argCount );
    int PopFrame();
    int CallPrimitive( U8 func );
    int CallNative( NativeFunc proc, U8 argCount, UserContext context );

    int SwitchModule( U8 newModIndex );

    void Push( CELL word );
    CELL Pop();

    bool WouldOverflow() const;
    bool WouldOverflow( U16 count ) const;
    bool WouldUnderflow() const;
    bool WouldUnderflow( U16 count ) const;

    bool IsCodeInBounds( U32 address ) const;

    std::pair<int, const Module*> GetDataModule( U8 index );
    std::pair<int, CELL*> GetSizedDataPtr( CELL addrWord, CELL size );

    const Module* GetModule( U8 index );

    virtual bool FindNativeCode( U32 id, NativeCode* nativeCode ) override;
    virtual const Module* FindModule( U8 index ) override;
};


int VerifyModule( const Module* mod );
