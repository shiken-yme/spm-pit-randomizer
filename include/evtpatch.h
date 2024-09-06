#pragma once

#include <evt_cmd.h>
#include <evtopcodes.h>
#include <spm/system.h>
#include <EASTL/stack.h>

namespace mod::evtpatch {

#define CALL(subEvt) \
    EVT_HELPER_CMD(1, EvtOpcode::Call), EVT_HELPER_OP(subEvt),
#define RETURN_FROM_CALL() \
    EVT_HELPER_CMD(0, EvtOpcode::ReturnFromCall) };

void evtmgrDestroyReturnStack(spm::evtmgr::EvtEntry* entry);
eastl::stack<spm::evtmgr::EvtScriptCode*>* getReturnStack(spm::evtmgr::EvtEntry* entry);

s32 evtOpcodeCall(spm::evtmgr::EvtEntry* entry);
s32 evtOpcodeReturnFromCall(spm::evtmgr::EvtEntry* entry);

void evtmgrExtensionInit();
s32 getLineOffset(spm::evtmgr::EvtScriptCode* script, s32 line);
s32 getInstructionBlockLength(spm::evtmgr::EvtScriptCode* instruction, u32 limit);
void insertTrampolineCall(spm::evtmgr::EvtScriptCode* ptr, spm::evtmgr::EvtScriptCode* script);

void hookEvt(spm::evtmgr::EvtScriptCode* script, s32 line, spm::evtmgr::EvtScriptCode* dst);
void hookEvtByOffset(spm::evtmgr::EvtScriptCode* script, s32 offset, spm::evtmgr::EvtScriptCode* dst);
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
    return getNumArgs(instruction) < 0x100 && getOpcode(instruction) < EVT_max; // if you see an evt opcode with more than 256 arguments lmk :)
}
/// @brief Checks if a given EvtScriptCode is likely the start of an instruction (Not 100% guaranteed)
/// @param instruction A pointer to the instruction to validate
/// @return Whether it seems to be a valid start of instruction
inline bool isStartOfInstruction(spm::evtmgr::EvtScriptCode* instruction) {
    if (instruction <= 0x80000000 || instruction >= 0x817fffff) return false;
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
    return instruction + 1;
}
/// @brief Gets an evt instruction from a script and a line number
/// @param script The script
/// @param line The line number
/// @return A pointer to the instruction
inline spm::evtmgr::EvtScriptCode* getEvtInstruction(spm::evtmgr::EvtScriptCode* script, s32 line) {
    return script + getLineOffset(script, line);
}

}
