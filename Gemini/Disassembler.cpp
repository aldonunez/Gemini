// Gemini Languages and Virtual Machine
// Copyright 2019 Aldo Jose Nunez
//
// Licensed under the Apache License, Version 2.0.
// See the LICENSE.txt file for details.

#include "pch.h"
#include "Disassembler.h"
#include "OpCodes.h"
#include <stdexcept>
#include <stdio.h>


static const char* gOpCodes[] = 
{
    "POP",
    "DUP",
    "OVER",
    "PUSH",
    "NOT",
    "LDARGA",
    "LDARG",
    "STARG",
    "LDLOCA",
    "LDLOC",
    "STLOC",
    "LDMOD",
    "STMOD",
    "LDC",
    "LDC.S",
    "LOADI",
    "STOREI",
    "PRIM",
    "B",
    "BFALSE",
    "BTRUE",
    "RET",
    "CALL",
    "CALLI",
    "CALLM",
    "CALLNATIVE",
    "CALLNATIVE.S",
    "INDEX",
    "INDEX.S",
    "COPYBLOCK",
    "BOUND",
    "BOUNDOPEN",
    "BOUNDOPENSLICE",
    "BOUNDOPENCLOSEDSLICE",
    "BOUNDSLICE",
    "YIELD",
};

static const char* gPrimitives[] = 
{
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD",
    "EQ",
    "NE",
    "LT",
    "LE",
    "GT",
    "GE",
};


Disassembler::Disassembler( const U8* code, bool showInstAddr )
    :   mCodeBin( code ),
        mCodePtr( code ),
        mShowInstAddr( showInstAddr )
{
    if ( code == nullptr )
        throw std::invalid_argument( "code" );
}

int32_t Disassembler::Disassemble( char* disassembly, size_t capacity )
{
    if ( disassembly == nullptr )
        throw std::invalid_argument( "disassembly" );

    U32 addr = static_cast<int32_t>(mCodePtr - mCodeBin);

    const U8* origCodePtr = mCodePtr;
    U8 op = *mCodePtr++;

    const char* opName = nullptr;
    int charsWritten = 0;
    int totalCharsWritten = 0;

    if ( op < OP_MAXOPCODE )
        opName = gOpCodes[op];
    else if ( op == OP_SENTINEL )
        opName = "SENTINEL";
    else
        opName = "*Invalid opcode";

    if ( mShowInstAddr )
    {
        charsWritten = snprintf( disassembly, (capacity - totalCharsWritten), "%06X:", addr );
        if ( charsWritten < 0 )
            return -1;

        disassembly += charsWritten;
        totalCharsWritten += charsWritten;
    }

    charsWritten = snprintf( disassembly, (capacity - totalCharsWritten), "  %s", opName );
    if ( charsWritten < 0 )
        return -1;

    disassembly += charsWritten;
    totalCharsWritten += charsWritten;

    charsWritten = 0;

    switch ( op )
    {
    case OP_POP:
    case OP_DUP:
    case OP_OVER:
    case OP_NOT:
    case OP_LOADI:
    case OP_STOREI:
    case OP_RET:
    case OP_BOUNDOPEN:
    case OP_BOUNDOPENSLICE:
    case OP_BOUNDOPENCLOSEDSLICE:
    case OP_YIELD:
        break;

    case OP_PUSH:
    case OP_LDARGA:
    case OP_LDARG:
    case OP_STARG:
    case OP_LDLOCA:
    case OP_LDLOC:
    case OP_STLOC:
    case OP_INDEX_S:
        {
            int value = *(U8*) mCodePtr++;
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten), " %u", value );
        }
        break;

    case OP_LDC_S:
        {
            int value = *(I8*) mCodePtr++;
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten), " %d", value );
        }
        break;

    case OP_LDMOD:
    case OP_STMOD:
        {
            int iMod = ReadU8( mCodePtr );
            U16 dataAddr = ReadU16( mCodePtr );
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten), " $%02X, $%04X", iMod, dataAddr );
        }
        break;

    case OP_INDEX:
    case OP_COPYBLOCK:
    case OP_BOUND:
    case OP_BOUNDSLICE:
        {
            int value = ReadU24( mCodePtr );
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten), " %u", value );
        }
        break;

    case OP_LDC:
        {
            int value = ReadI32( mCodePtr );
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten), " %d", value );
        }
        break;

    case OP_CALL:
        {
            U8 callFlags = *mCodePtr++;
            U32 funcAddr = ReadU24( mCodePtr );
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten),
                "%s(%d) $%06X",
                (CallFlags::GetAutoPop( callFlags ) ? ".POP" : ""),
                CallFlags::GetCount( callFlags ), funcAddr );
        }
        break;

    case OP_CALLI:
        {
            U8 callFlags = *mCodePtr++;
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten),
                "%s(%d)",
                (CallFlags::GetAutoPop( callFlags ) ? ".POP" : ""),
                CallFlags::GetCount( callFlags ) );
        }
        break;

    case OP_PRIM:
        {
            int primitive = *(U8*) mCodePtr++;
            if ( primitive >= PRIM_MAXPRIMITIVE )
                return -1;
            const char* primitiveName = gPrimitives[primitive];
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten),
                " %s",
                primitiveName );
        }
        break;

    case OP_CALLM:
    case OP_CALLNATIVE:
        {
            U8 callFlags = *mCodePtr++;
            U32 id = ReadU32( mCodePtr );
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten),
                "%s(%d) $%08X",
                (CallFlags::GetAutoPop( callFlags ) ? ".POP" : ""),
                CallFlags::GetCount( callFlags ), id );
        }
        break;

    case OP_CALLNATIVE_S:
        {
            U8 callFlags = *mCodePtr++;
            int id = *mCodePtr;
            mCodePtr += 1;
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten),
                "%s(%d) $%02X",
                (CallFlags::GetAutoPop( callFlags ) ? ".POP" : ""),
                CallFlags::GetCount( callFlags ), id );
        }
        break;

    case OP_B:
    case OP_BFALSE:
    case OP_BTRUE:
        {
            int offset = BranchInst::ReadOffset( mCodePtr );
            int target = static_cast<int32_t>(mCodePtr - mCodeBin) + offset;
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten), " $%06X", target );
        }
        break;

    default:
        {
            charsWritten = snprintf( disassembly, (capacity - totalCharsWritten), " $%02X", op );
        }
        break;
    }

    if ( charsWritten < 0 )
        return -1;

    disassembly += charsWritten;
    totalCharsWritten += charsWritten;

    return static_cast<int32_t>(mCodePtr - origCodePtr);
}
