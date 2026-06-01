#include "acpatch.h"
#include "patch.h"

#include <common.h>
#include <msl/string.h>
#include <spm/acdrv.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/os.h>

namespace mod::acpatch {
    using namespace spm;
    using namespace spm::acdrv;
    using namespace spm::memory;
    using namespace msl::string;

    AcDef * acDefs = nullptr;

    AcDef * acpatchGetDefDirect(AcEntry * entry) {
        return &acDefs[entry->type];
    }

    void acpatchAssignCallbacks(AcEntry * entry) {
        AcDef * def = &acDefs[entry->type];
        entry->initFunc = def->initFunc;
        entry->mainFunc = def->mainFunc;
        entry->deleteFunc = def->deleteFunc;
        entry->dispFunc = def->dispFunc;
        if (def->initFunc != nullptr)
            (def->initFunc)(entry);
        return;
    }

    void acpatchInit() {
        /*
            Patch calls to acDefs
        */
        // 0x8003F388 -> 0x8003F47C
        writeWord(acEntry, 0xF4, MR(3, 31));
        writeBranchLink(acEntry, 0xFC, acpatchGetDefDirect);
        // 0x8003F388 -> 0x8003f4E8
        writeWord(acEntry, 0x160, MR(3, 31));
        writeBranchLink(acEntry, 0x164, acpatchGetDefDirect);
        // 0x8003F388 -> 0x8003F544
        writeWord(acEntry, 0x1BC, MR(3, 31));
        writeBranchLink(acEntry, 0x1C0, acpatchAssignCallbacks);
        writeWord(acEntry, 0x1C4, B(0x3C));
        // 0x8003EE44 -> 0x8003F0E4
        writeWord(acMain, 0x2A0, MR(3, 16));
        writeBranchLink(acMain, 0x2A4, acpatchGetDefDirect);
        /*
            Reallocate acDefs to a larger block
        */
        acDefs = (AcDef *)__memAlloc(HEAP_MAIN, (u32)(sizeof(AcDef) * ACPATCH_DEF_MAX));
        memcpy(&acDefs[0], &acdrv_acDefs[0], (u32)(sizeof(AcDef) * 19));
        wii::os::OSReport("ACPATCH Init - acDefs @ %p\n", acDefs);
        return;
    }
}
