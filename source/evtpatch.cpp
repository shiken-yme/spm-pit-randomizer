#include "evtpatch.h"
#include "evtopcodes.h"
#include "patch.h"
#include <msl/string.h>
#include <spm/evtmgr_cmd.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/os.h>

namespace mod::evtpatch {

using namespace spm::evtmgr;

static Stack<EvtScriptCode*>* returnStacks[EVT_ENTRY_MAX];

/// @brief Clears all entries in an EvtEntry's return stack, then destroys the stack and all references to it.
/// @param entryId The EvtEntry's Id
void evtmgrDestroyReturnStack(s32 entryIdx) {
    Stack<EvtScriptCode*>* stack = returnStacks[entryIdx];
    if (stack != nullptr) {
        wii::os::OSReport("evtmgrDestroyReturnStack: Destroying return stack for entry at index [%d]\n", entryIdx);
        stack->clear();
        delete stack;
        returnStacks[entryIdx] = nullptr;
    }
}

Stack<EvtScriptCode*>* getReturnStack(spm::evtmgr::EvtEntry* entry) {
    Stack<EvtScriptCode*>* stack = returnStacks[getEvtEntryIdx(entry)];
    if (stack == nullptr) {
        wii::os::OSReport("getReturnStack: Creating return stack for EvtEntry at offset [%d]\n", getEvtEntryIdx(entry));
        returnStacks[getEvtEntryIdx(entry)] = new Stack<EvtScriptCode*>();
        stack = returnStacks[getEvtEntryIdx(entry)];
    }
    return stack;
}

/// @brief Jumps execution of an EVT entry to a specified location
/// @param entry The EVT entry
/// @return EVT_RET_CONTINUE
s32 evtOpcodeCall(spm::evtmgr::EvtEntry* entry) {
    Stack<EvtScriptCode*>* curReturnStack = getReturnStack(entry);
    curReturnStack->push(entry->pCurInstruction);
    EvtScriptCode* destScript = (EvtScriptCode*)entry->pCurData[0];
    evt_patch_make_jump_table(entry, destScript);
    entry->pCurInstruction = destScript;
    wii::os::OSReport("OpcodeCall: pushed return stack for EvtEntry [%p] at [%p], from: [%p] to: [%p]\n", entry, curReturnStack, entry->pPrevInstruction, entry->pCurInstruction);
    return EVT_RET_CONTINUE;
}

/// @brief Returns execution back to the previous location after EvtOpcodeCall
/// @param entry The EVT entry
/// @return EVT_RET_CONTINUE
s32 evtOpcodeReturnFromCall(spm::evtmgr::EvtEntry* entry) {
    Stack<EvtScriptCode*>* curReturnStack = getReturnStack(entry);
    wii::os::OSReport("OpcodeReturnFromCall: return stack: [%p], from: [%p] to: [%p]\n", curReturnStack, entry->pCurInstruction, curReturnStack->peek());
    entry->pCurInstruction = curReturnStack->pop();
    if (curReturnStack->isEmpty()) {
        wii::os::OSReport("Return stack for evtEntry [%p] is empty; It will now be freed.\n", entry);
        evtmgrDestroyReturnStack(getEvtEntryIdx(entry));
    }
    return EVT_RET_CONTINUE;
}

/// @brief Handles custom opcodes in evtmgrCmd
/// @param entry The EVT entry being processed by evtmgrCmd
/// @return The return value of the opcode's handler function
static s32 evtmgrCmdExtraCases(spm::evtmgr::EvtEntry* entry) {
    switch (entry->curOpcode) {
        case EvtOpcode::Call:
            return evtOpcodeCall(entry);
        case EvtOpcode::ReturnFromCall:
            return evtOpcodeReturnFromCall(entry);
        default:
            assert(0, "evtmgrCmdExtraCases error");
            __builtin_unreachable();
    }
}

/// @brief Updates the jump table of a parent script to include GOTOs used by evtpatch child scripts
/// @param parentEntry The EvtEntry of the script being hooked on
/// @param childEntry The evtpatch child script
void evt_patch_make_jump_table(EvtEntry* parentEntry, EvtScriptCode* childScript)
{
    s32 n;
    EvtScriptCode * pScriptHead;
    s32 cmd;
    s32 cmdn;
    s32 id;
    pScriptHead = childScript;
    n = 0;

    while (true)
    {
        cmd = *pScriptHead & 0xffff;
        cmdn = *pScriptHead >> 16;
        pScriptHead++;

        id = *pScriptHead;
        pScriptHead += cmdn;

        switch (cmd)
        {
            case 1:
                goto end;

            case EvtOpcode::ReturnFromCall:
                goto end;

            case 3:
                parentEntry->labelIds[n] = (s8) id;
                parentEntry->jumptable[n] = pScriptHead;
                n++;
            break;
        }

    }
    end: return;
}

static void evtmgrCmdExtensionPatch() {
    writeWord(spm::evtmgr_cmd::evtmgrCmd, 0x7B8, 0x7f63db78); // mr r3, r27
    writeBranchLink(spm::evtmgr_cmd::evtmgrCmd, 0x7BC, evtmgrCmdExtraCases);
    writeWord(spm::evtmgr_cmd::evtmgrCmd, 0x7C0, 0x7c7c1b78); // mr r28, r3
    writeWord(spm::evtmgr_cmd::evtmgrCmd, 0x7C4, 0x4800000c); // blt 0xc -> b 0xc, bypassing 0x77 max opcode check
    writeWord(spm::evtmgr::make_jump_table, 0xe0, 0x48000020); // blt 0x20 -> b 0x20, bypassing 0x77 max opcode check
}

static void (*evtDeleteReal)(EvtEntry*);
static void evtDeletePatch() {
    evtDeleteReal = patch::hookFunction(spm::evtmgr::evtDelete, [](EvtEntry* entry) {
        evtmgrDestroyReturnStack(getEvtEntryIdx(entry));
        return evtDeleteReal(entry);
    });
}

static void (*evtmgrReInitReal)();
static void evtmgrReInitPatch() {
    evtmgrReInitReal = patch::hookFunction(spm::evtmgr::evtmgrReInit, []() {
        for (s32 i = 0; i < EVT_ENTRY_MAX; i++) {
            if (returnStacks[i] != nullptr) {
                evtmgrDestroyReturnStack(i);
            }
        }
        return evtmgrReInitReal();
    });
}

/// @brief Patches evtmgrCmd and other related code to allow and handle custom opcodes
void evtmgrExtensionInit() {
    evtmgrCmdExtensionPatch();
    evtDeletePatch();
    evtmgrReInitPatch();
}

const EvtScriptCode trampolineCall[] = { CALL(0) };
EVT_BEGIN(trampolineReturnFromCall)
RETURN_FROM_CALL()

#define TRAMPOLINE_CALL_EVT_OFFSET_BYTES 0x4 // first instruction, first argument
#define TRAMPOLINE_CALL_EVT_OFFSET TRAMPOLINE_CALL_EVT_OFFSET_BYTES / sizeof(EvtScriptCode) // first instruction, first argument
#define TRAMPOLINE_CALL_SIZE sizeof(trampolineCall)
#define TRAMPOLINE_CALL_LENGTH TRAMPOLINE_CALL_SIZE / sizeof(EvtScriptCode)
#define TRAMPOLINE_RETURN_FROM_CALL_SIZE sizeof(trampolineReturnFromCall)
#define TRAMPOLINE_RETURN_FROM_CALL_LENGTH TRAMPOLINE_RETURN_FROM_CALL_SIZE / sizeof(EvtScriptCode)

/// @brief Gets the offset in EvtScriptCodes of a line from the start of an evt script
/// @param script The evt script to look through
/// @param line The line number to find the offset of, 1-indexed
/// @return The offset of the line, in EvtScriptCodes, from the start of the script
s32 getLineOffset(EvtScriptCode* script, s32 line) {
    assert(isStartOfInstruction(script), "Cannot hook on non-instruction, what are you doing :sob:");
    wii::os::OSReport("getLineOffset(): script: [%p], line: %d\n", script, line);
    EvtScriptCode* instruction = script;
    s32 offset = 0;
    for (int i = 0; i < line-1; i++) { // 1-indexed
        do {
            s32 length = getInstructionLength(instruction);
            offset += length;
            instruction = script + offset;
        } while (*instruction == 0x0);
    }
    wii::os::OSReport("%d, %d\n", line, offset);
    return offset;
}

/// @brief Gets the length of a block of instructions until it reaches a limit
/// @param instruction A pointer to the starting instruction
/// @param limit The limit
/// @return The length
s32 getInstructionBlockLength(spm::evtmgr::EvtScriptCode* instruction, u32 limit) {
    u32 len = 0;
    s32 curInstLen;
    do {
        curInstLen = getNumArgs(instruction[len]);
        len += (1 + curInstLen);
    } while (len < limit);
    return len;
}

/// @brief Jumps execution of an EVT entry to a specified location
/// @param ptr The place to write the instruction at
/// @param script The script to branch to
void insertTrampolineCall(EvtScriptCode* ptr, EvtScriptCode* script) {
    msl::string::memcpy(ptr, trampolineCall, TRAMPOLINE_CALL_SIZE);
    ptr[TRAMPOLINE_CALL_EVT_OFFSET] = reinterpret_cast<s32>(script);
}

/// @brief Hooks into an evt script, automatically preserving original instructions
/// @param script The evt script that will be hooked into
/// @param line The line number to hook at, 1-indexed
/// @param dst The evt script that will be executed
void hookEvt(EvtScriptCode* script, s32 line, EvtScriptCode* dst) {
    hookEvtByOffset(script, getLineOffset(script, line), dst);
}

/// @brief Hooks into an evt script, automatically preserving original instructions
/// @param script The evt script that will be hooked into
/// @param offset The offset to hook at, in EvtScriptCodes, from the start of the script
/// @param dst The evt script that will be executed
void hookEvtByOffset(EvtScriptCode* script, s32 offset, EvtScriptCode* dst) {
    EvtScriptCode* src = script + offset;
    assert(isStartOfInstruction(src), "Cannot hook on non-instruction, what are you doing :sob:");

    u32 lenOriginalInstructions = getInstructionBlockLength(src, TRAMPOLINE_CALL_LENGTH);
    u32 sizeOriginalInstructions = lenOriginalInstructions * sizeof(EvtScriptCode);

    EvtScriptCode* dynamicEvtForwarder = new EvtScriptCode[TRAMPOLINE_CALL_LENGTH + lenOriginalInstructions + sizeof(EvtScriptCode)];
    insertTrampolineCall(dynamicEvtForwarder, dst);
    msl::string::memcpy(dynamicEvtForwarder + TRAMPOLINE_CALL_LENGTH, src, sizeOriginalInstructions); // add original instructions
    msl::string::memcpy(dynamicEvtForwarder + TRAMPOLINE_CALL_LENGTH + lenOriginalInstructions, trampolineReturnFromCall, TRAMPOLINE_RETURN_FROM_CALL_SIZE); // add return
    msl::string::memset(src, 0, sizeOriginalInstructions); // pad anything left with 0s
    insertTrampolineCall(src, dynamicEvtForwarder);
}

/// @brief Patches a single instruction in a script with another instruction of the same size or smaller
/// @param script The script that will be patched
/// @param line The line number of the instruction to patch, 1-indexed
/// @param replacement The instruction
void patchEvtInstruction(EvtScriptCode* script, s32 line, EvtScriptCode* replacement) {
    patchEvtInstructionByOffset(script, getLineOffset(script, line), replacement);
}

/// @brief Patches a single instruction in a script with another instruction of the same size or smaller
/// @param script The script that will be patched
/// @param line The line number of the instruction to patch, 1-indexed
/// @param replacement The patched instruction
void patchEvtInstructionByOffset(EvtScriptCode* script, s32 offset, EvtScriptCode* replacement) {
    EvtScriptCode* src = script + offset;
    assert(isStartOfInstruction(src), "Cannot hook on non-instruction, what are you doing :sob:");
    assert(getInstructionLength(src) >= getInstructionLength(replacement), "Replacement instruction too large; use hookEvtReplace");

    msl::string::memset(src, 0, getInstructionLength(src)); // pad anything left with 0s
    msl::string::memcpy(src, replacement, getInstructionSize(replacement));
}

/// @brief Adds a hook to another evt script, without preserving original instructions
/// @param script The evt script that will be hooked into
/// @param line The line number to hook at, 1-indexed
/// @param dst The evt script that will be executed
void hookEvtReplace(EvtScriptCode* script, s32 line, EvtScriptCode* dst) {
    hookEvtReplaceByOffset(script, getLineOffset(script, line), dst);
}

/// @brief Adds a hook to another evt script, without restoring original instructions
/// @param script The evt script that will be hooked into
/// @param offset The offset to hook at, in EvtScriptCodes, from the start of the script
/// @param dst The evt script that will be executed
void hookEvtReplaceByOffset(EvtScriptCode* script, s32 offset, EvtScriptCode* dst) {
    EvtScriptCode* src = script + offset;
    assert(isStartOfInstruction(src), "Cannot hook on non-instruction, what are you doing :sob:");

    u32 lenOriginalInstructions = getInstructionBlockLength(src, TRAMPOLINE_CALL_LENGTH);
    u32 sizeOriginalInstructions = lenOriginalInstructions * sizeof(EvtScriptCode);

    msl::string::memset(src, 0, sizeOriginalInstructions); // pad anything left with 0s
    insertTrampolineCall(src, dst);
}


/// @brief Adds a hook to another evt script, without restoring original instructions and comes back at a specified offset
/// @param script The evt script that will be hooked into
/// @param lineStart The line number to hook at, 1-indexed
/// @param dst The evt script that will be executed
/// @param lineEnd The line number to come back to once `dst` is done executing, 1-indexed
void hookEvtReplaceBlock(EvtScriptCode* script, s32 lineStart, EvtScriptCode* dst, s32 lineEnd) {
    hookEvtReplaceBlockByOffset(script, getLineOffset(script, lineStart), dst, getLineOffset(script, lineEnd));
}

/// @brief Adds a hook to another evt script, without restoring original instructions and comes back at a specified offset
/// @param script The evt script that will be hooked into
/// @param offsetStart The offset to hook at, in EvtScriptCodes, from the start of the script
/// @param dst The evt script that will be executed
/// @param offsetEnd The offset to come back to once `dst` is done executing, in EvtScriptCodes, from the start of the script
void hookEvtReplaceBlockByOffset(EvtScriptCode* script, s32 offsetStart, EvtScriptCode* dst, s32 offsetEnd) {
    EvtScriptCode* src = script + offsetStart;
    s32 length = offsetEnd-offsetStart;
    assert(isStartOfInstruction(src), "Cannot hook on non-instruction, what are you doing :sob:");
    msl::string::memset(src, 0, length * sizeof(EvtScriptCode)); // if i have time i'll change offsets to be in bytes like a normal person
    insertTrampolineCall(src, dst);
}

}
