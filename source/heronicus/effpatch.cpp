#include "effpatch.h"
#include "patch.h"
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os/OSError.h>
#include <wii/gx.h>
#include <spm/effdrv.h>
#include <spm/eff_sub.h>
#include <spm/dispdrv.h>

namespace mod::effpatch
{
    using namespace spm;

    EffPatchColorMask effpatchColorMaskEntries[EFFPATCH_COLOR_MASK_ENTRY_MAX];

    void effpatchColorMaskEntry(effdrv::EffEntry *eff, wii::gx::GXColor col1, wii::gx::GXColor col2, EffpatchColorMaskUpdateFunc *updateFunc)
    {
        //    wii::os::OSReport("EffPatch: Entries are located at %p.\n", &effpatchColorMaskEntries);
        s32 i;
        for (i = 0; i < EFFPATCH_COLOR_MASK_ENTRY_MAX; i += 1)
        {
            if (effpatchColorMaskEntries[i].eff == nullptr)
            {
                effpatchColorMaskEntries[i].eff = eff;
                effpatchColorMaskEntries[i].col1 = col1;
                effpatchColorMaskEntries[i].col2 = col2;
                if (updateFunc != nullptr)
                {
                    effpatchColorMaskEntries[i].updateFunc = updateFunc;
                }
                break;
            }
            else if (effpatchColorMaskEntries[i].eff == eff)
            {
                effpatchColorMaskEntries[i].col1 = col1;
                effpatchColorMaskEntries[i].col2 = col2;
                break;
            }
        }
        return;
    }

    u8 effpatchChgColor(u8 color, u8 num, bool positive)
    {
        if (positive)
        {
            if ((color + num) < 255)
            {
                color = color + num;
            }
            else
                color = 255;
        }
        else
        {
            if ((color - num) > 0)
            {
                color = color - num;
            }
            else
                color = 0;
        }
        return color;
    }

    void dispDrawNew(s32 camId)
    {
        dispdrv::dispdrv_currentCallbackPtr = nullptr;
        s32 i = 0;
        s32 n = 0;
        while (dispdrv::dispdrv_entry_n > i)
        {
            dispdrv::DispEntry *entry = *(dispdrv::DispEntry **)((s32)dispdrv::dispdrv_pSortWork + n); // I really hate this.
            if (entry->cameraId == camId)
            {
                // Check if the callback param is an eff entry with an effpatch colormask entry
                s32 j = 0;
                for (j = 0; j < EFFPATCH_COLOR_MASK_ENTRY_MAX; j += 1)
                {
                    if (effpatchColorMaskEntries[j].eff != nullptr)
                    {
                        if ((u32)entry->callbackParam == (u32)effpatchColorMaskEntries[j].eff)
                        {
                            if (effpatchColorMaskEntries[j].updateFunc != nullptr)
                            {
                                (effpatchColorMaskEntries[j].updateFunc)(&effpatchColorMaskEntries[j]);
                            }
                            eff_sub::effSubSetColorOverride(true, &effpatchColorMaskEntries[j].col1, &effpatchColorMaskEntries[j].col2);
                            break;
                        }
                    }
                }
                u32 scissor = (u32)entry->scissorId;
                // thank you vabold for translating this if statement in advance of this project needing it. ghidra fucked up this output BADLY
                if (scissor != 0)
                {
                    wii::gx::GXSetScissor(dispdrv::dispdrv_dispScissors[scissor - 1].left, dispdrv::dispdrv_dispScissors[scissor - 1].top,
                                          dispdrv::dispdrv_dispScissors[scissor - 1].width, dispdrv::dispdrv_dispScissors[scissor - 1].height);
                }
                wii::gx::GXSetColorUpdate(1);
                if (camId == 5 || camId == 7 || camId == 9)
                {
                    wii::gx::GXSetAlphaUpdate(0);
                }
                else
                    wii::gx::GXSetAlphaUpdate(1);
                wii::gx::GXSetZScaleOffset(1.0, 0.0);
                switch (entry->renderMode)
                {
                case 0:
                    wii::gx::GXSetBlendMode(0, 1, 0, 0);
                    wii::gx::GXSetZCompLoc(1);
                    wii::gx::GXSetAlphaCompare(7, 0, 0, 7, 0);
                    wii::gx::GXSetZMode(0, 3, 0);
                    break;
                case 1:
                    wii::gx::GXSetBlendMode(0, 1, 0, 0);
                    wii::gx::GXSetZCompLoc(0);
                    wii::gx::GXSetAlphaCompare(6, 128, 1, 0, 0);
                    wii::gx::GXSetZMode(0, 3, 0);
                    break;
                case 2:
                case 9:
                case 10:
                case 11:
                    wii::gx::GXSetBlendMode(1, 4, 5, 0);
                    wii::gx::GXSetZCompLoc(1);
                    wii::gx::GXSetAlphaCompare(7, 0, 0, 7, 0);
                    wii::gx::GXSetZMode(1, 3, 0);
                    break;
                case 3:
                case 4:
                case 5:
                    wii::gx::GXSetBlendMode(0, 1, 0, 7);
                    wii::gx::GXSetZCompLoc(1);
                    wii::gx::GXSetAlphaCompare(7, 0, 0, 7, 0);
                    wii::gx::GXSetZMode(1, 3, 1);
                    break;
                case 6:
                case 7:
                case 8:
                    wii::gx::GXSetBlendMode(0, 1, 0, 0);
                    wii::gx::GXSetZCompLoc(0);
                    wii::gx::GXSetAlphaCompare(6, 128, 1, 0, 0);
                    wii::gx::GXSetZMode(1, 3, 1);
                    break;
                }
                dispdrv::dispdrv_currentWorkPtr = entry;
                (entry->callback)(camId, entry->callbackParam);
                dispdrv::dispdrv_currentCallbackPtr = entry->callback;
                if (entry->scissorId != 0)
                {
                    wii::gx::GXSetScissor(0, 0, 608, 480);
                }
                eff_sub::effSubSetColorOverride(false, nullptr, nullptr);
            }
            n += 4;
            i += 1;
        }
        return;
    }

    void (*effDelete)(effdrv::EffEntry *eff);
    void (*effSoftDelete)(effdrv::EffEntry *eff);
    static void effpatchDeletePatch()
    {
        effDelete = patch::hookFunction(effdrv::effDelete,
                                        [](effdrv::EffEntry *eff)
                                        {
                                            s32 i;
                                            for (i = 0; i < EFFPATCH_COLOR_MASK_ENTRY_MAX; i += 1)
                                            {
                                                if (effpatchColorMaskEntries[i].eff != nullptr)
                                                {
                                                    if (effpatchColorMaskEntries[i].eff == eff)
                                                    {
                                                        effpatchColorMaskEntries[i].eff = nullptr;
                                                        effpatchColorMaskEntries[i].frmCtr = 0;
                                                        if (effpatchColorMaskEntries[i].updateFunc != nullptr)
                                                        {
                                                            effpatchColorMaskEntries[i].updateFunc = nullptr;
                                                        }
                                                        eff_sub::effSubSetColorOverride(false, nullptr, nullptr);
                                                        break;
                                                    }
                                                }
                                            }
                                            effDelete(eff);
                                        });
        effSoftDelete = patch::hookFunction(effdrv::effSoftDelete,
                                            [](effdrv::EffEntry *eff)
                                            {
                                                s32 i;
                                                for (i = 0; i < EFFPATCH_COLOR_MASK_ENTRY_MAX; i += 1)
                                                {
                                                    if (effpatchColorMaskEntries[i].eff != nullptr)
                                                    {
                                                        if (effpatchColorMaskEntries[i].eff == eff)
                                                        {
                                                            effpatchColorMaskEntries[i].eff = nullptr;
                                                            effpatchColorMaskEntries[i].frmCtr = 0;
                                                            if (effpatchColorMaskEntries[i].updateFunc != nullptr)
                                                            {
                                                                effpatchColorMaskEntries[i].updateFunc = nullptr;
                                                            }
                                                            eff_sub::effSubSetColorOverride(false, nullptr, nullptr);
                                                            break;
                                                        }
                                                    }
                                                }
                                                effSoftDelete(eff);
                                            });
    }

    void effpatchInit()
    {
        // Rewrite eff delete functions to remove effpatch color mask entries automatically
        effpatchDeletePatch();
        // Rewrite dispDraw to allow effs to use effpatch callbacks when displayed
        patch::hookFunction(dispdrv::dispDraw, dispDrawNew);
    }

}