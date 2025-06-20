#pragma once

#include <evt_cmd.h>
#include <spm/system.h>
#include "evtopcodes.h"
#include "stack.hh"

namespace mod::evtpatch {

#define CALL(subEvt) \
    EVT_HELPER_CMD(1, EvtOpcode::Call), EVT_HELPER_OP(subEvt),
#define RETURN_FROM_CALL() \
    EVT_HELPER_CMD(0, EvtOpcode::ReturnFromCall) };

// #define PATCH_EVT(script, line, replacement) \
//     spm::evtmgr::EvtScriptCode _##script_##line[] = { replacement }; \
//     evtpatch::patchEvt(script, line, _##script_##line)

// TODO: think of a better name
 #define EVT_CAST(instruction) (spm::evtmgr::EvtScriptCode[]){ instruction }

void evt_patch_make_jump_table(spm::evtmgr::EvtEntry* sourceEntry, spm::evtmgr::EvtScriptCode* destEntry);

void evtmgrDestroyReturnStack(s32 entryId);
Stack<spm::evtmgr::EvtScriptCode*>* getReturnStack(spm::evtmgr::EvtEntry* entry);

s32 evtOpcodeCall(spm::evtmgr::EvtEntry* entry);
s32 evtOpcodeReturnFromCall(spm::evtmgr::EvtEntry* entry);

void evtmgrExtensionInit();
s32 getLineOffset(spm::evtmgr::EvtScriptCode* script, s32 line);
s32 getInstructionBlockLength(spm::evtmgr::EvtScriptCode* instruction, u32 limit);
void insertTrampolineCall(spm::evtmgr::EvtScriptCode* ptr, spm::evtmgr::EvtScriptCode* script);

void hookEvt(spm::evtmgr::EvtScriptCode* script, s32 line, spm::evtmgr::EvtScriptCode* dst);
void hookEvtByOffset(spm::evtmgr::EvtScriptCode* script, s32 offset, spm::evtmgr::EvtScriptCode* dst);
void patchEvtInstruction(spm::evtmgr::EvtScriptCode* script, s32 line, spm::evtmgr::EvtScriptCode* replacement);
void patchEvtInstructionByOffset(spm::evtmgr::EvtScriptCode* script, s32 offset, spm::evtmgr::EvtScriptCode* replacement);
void hookEvtReplace(spm::evtmgr::EvtScriptCode* script, s32 line, spm::evtmgr::EvtScriptCode* dst);
void hookEvtReplaceByOffset(spm::evtmgr::EvtScriptCode* script, s32 offset, spm::evtmgr::EvtScriptCode* dst);
void hookEvtReplaceBlock(spm::evtmgr::EvtScriptCode* script, s32 lineStart, spm::evtmgr::EvtScriptCode* dst, s32 lineEnd);
void hookEvtReplaceBlockByOffset(spm::evtmgr::EvtScriptCode* script, s32 offsetStart, spm::evtmgr::EvtScriptCode* dst, s32 offsetEnd);

/// @brief Gets the number of arguments for a given instruction in an evt script
/// @param instruction The instruction word to check
/// @return The number of arguments in the instruction
inline u32 getNumArgs(spm::evtmgr::EvtScriptCode instruction) {
    return instruction >> 16;
}

/// @brief Gets the opcode for a given instruction in an evt script
/// @param instruction The instruction word to check
/// @return The opcode of the instruction.
inline u32 getOpcode(spm::evtmgr::EvtScriptCode instruction) {
    return instruction & 0xffff;
}

/// @brief Checks if a given EvtScriptCode is likely the start of an instruction (Not 100% guaranteed)
/// @param instruction The instruction word to validate
/// @return Whether it seems to be a valid start of instruction
inline bool isStartOfInstruction(spm::evtmgr::EvtScriptCode instruction) {
    return getNumArgs(instruction) < 0x100 && getOpcode(instruction) < EvtOpcode::Max; // if you see an evt opcode with more than 256 arguments lmk :)
}

/// @brief Checks if a given EvtScriptCode is likely the start of an instruction (Not 100% guaranteed)
/// @param instruction A pointer to the instruction to validate
/// @return Whether it seems to be a valid start of instruction
inline bool isStartOfInstruction(spm::evtmgr::EvtScriptCode* instruction) {
    if ((u32)instruction <= 0x80000000 || (u32)instruction >= 0x817fffff) return false;
    return isStartOfInstruction(*instruction); // if you see an evt opcode with more than 256 arguments lmk :)
}

/// @brief Gets the length of an instruction, in EvtScriptCodes (int32s)
/// @param instruction The instruction word
/// @return The size of the instruction, in EvtScriptCodes (int32s)
inline s32 getInstructionLength(spm::evtmgr::EvtScriptCode instruction) {
    return (1 + getNumArgs(instruction));
}

/// @brief Gets the size of an instruction, in EvtScriptCodes (int32s)
/// @param instruction A pointer to the instruction
/// @return The size of the instruction, in EvtScriptCodes (int32s)
inline s32 getInstructionLength(spm::evtmgr::EvtScriptCode* instruction) {
    return getInstructionLength(*instruction);
}

/// @brief Gets the size of an instruction, in bytes
/// @param instruction A pointer to the instruction
/// @return The size of the instruction, in bytes
inline s32 getInstructionSize(spm::evtmgr::EvtScriptCode* instruction) {
    return getInstructionLength(instruction) * sizeof(spm::evtmgr::EvtScriptCode);
}

/// @brief Gets a pointer to an instruction's args
/// @param instruction A pointer to the instruction
/// @return The pointer to the instruction's args
inline spm::evtmgr::EvtScriptCode* getInstructionArgv(spm::evtmgr::EvtScriptCode* instruction) {
    assert(isStartOfInstruction(instruction), "Cannot hook on non-instruction, what are you doing :sob:");
    return instruction + 1;
}

/// @brief Gets an evt instruction from a script and a line number
/// @param script The script
/// @param line The line number
/// @return A pointer to the instruction
inline spm::evtmgr::EvtScriptCode* getEvtInstruction(spm::evtmgr::EvtScriptCode* script, s32 line) {
    assert(isStartOfInstruction(script), "Cannot hook on non-instruction, what are you doing :sob:"); // Cannot hook on non-instruction, what are you doing :sob:
    return script + getLineOffset(script, line);
}

/// @brief Takes an EvtEntry and returns its index in EvtWork's evtEntries array.
/// @param entry The EvtEntry
/// @return The index of the EvtEntry in EvtWork's evtEntries array
inline s32 getEvtEntryIdx(spm::evtmgr::EvtEntry* entry) {
    return reinterpret_cast<s32>(entry - spm::evtmgr::evtGetWork()->entries);
}

}
