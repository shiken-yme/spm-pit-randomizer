#pragma once
#include <common.h>
#include <spm/effdrv.h>
#include <spm/evt_door.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/mobjdrv.h>
#include <wii/gx.h>
#include <wii/mtx.h>

namespace mod::bero {
    using namespace spm;

#define BERO_DOKAN_MAX 100

    struct _Dokan;

    typedef bool(DokanSpawnChkCb)(_Dokan * dokan);

    enum CornerOrient : s32 {
        ORIENT_UP_RIGHT,
        ORIENT_UP_LEFT,
        ORIENT_DOWN_LEFT,
        ORIENT_DOWN_RIGHT
    };

    enum CornerShift : s32 {
        SHIFT_NONE,
        SHIFT_UP,
        SHIFT_UP_LEFT,
        SHIFT_UP_RIGHT,
        SHIFT_DOWN,
        SHIFT_DOWN_LEFT,
        SHIFT_DOWN_RIGHT,
        SHIFT_LEFT,
        SHIFT_RIGHT
    };

    typedef struct {
        mobjdrv::MobjEntry * cornerMobj;
        /*
            Object's origin located @ top in the center
            @ 0 rotation is always ⤷ (top is primary connector)
            Ignores dokan orientation entirely
        */
        CornerOrient orientation;
        CornerShift shift;
        mobjdrv::MobjEntry * extraBaseMobj;
        f32 extraBaseObjHeight; // leave 0 to not generate a base on the other end of the corner
    } DokanCorner;

    typedef struct _Dokan {
        evt_door::DokanDesc Desc;
        wii::mtx::Vec3 pos;
        f32 baseObjHeight;
        const char * headModelName;
        const char * baseModelName;
        const char * cornerModelName;
        DokanSpawnChkCb * spawnCheck;
        mobjdrv::MobjEntry * headMobj;
        mobjdrv::MobjEntry * baseMobj;
        DokanCorner * Corners;
        s32 cornerCount;
    } Dokan;

    extern Dokan * Dokans[BERO_DOKAN_MAX];

    Dokan * beroDokanEntry(const char * name, const char * mapName, const char * destMapName, const char * destBeroName, f32 x, f32 y, f32 z,
                           f32 baseObjHeight, evt_door::DokanScriptNum orientation, const char * headModelName, const char * baseModelName, DokanSpawnChkCb * spawnChk, u16 dokanFlags);

    void beroDokanCornerEntry(Dokan * dokan, const char * cornerModelName, CornerOrient orientation, CornerShift shift, f32 extraBaseObjHeight);

    // void beroCreateDokan(Dokan *entry);

    void beroberoInit();

    // EVT_DECLARE_USER_FUNC(evt_bero_dokan, 12)
}
