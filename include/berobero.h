#pragma once
#include <common.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <spm/effdrv.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_door.h>

namespace mod::bero
{
    using namespace spm;

#define BERO_DOKAN_MAX 100

    struct _Dokan;

    typedef bool(DokanSpawnChkCb)(_Dokan *dokan);

    typedef struct _Dokan
    {
        evt_door::DokanDesc Desc;
        wii::mtx::Vec3 pos;
        f32 scale;
        const char *modelName;
        // const char *tailName;
        DokanSpawnChkCb *spawnCheck;
    } Dokan;

    extern Dokan *Dokans[BERO_DOKAN_MAX];

    Dokan *beroDokanEntry(const char *name, const char *mapName, const char *destMapName, const char *destBeroName, f32 x, f32 y, f32 z,
                          f32 scale, evt_door::DokanScriptNum orientation, const char *modelName, DokanSpawnChkCb *spawnChk, u16 dokanFlags);

    // void beroCreateDokan(Dokan *entry);

    void beroberoInit();

    // EVT_DECLARE_USER_FUNC(evt_bero_dokan, 12)
}
