#include <nw4r/snd.hpp>
#include <spm/spmario_snd.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os/OSError.h>
#include <nw4r/snd.hpp>
#include "sndpatch.h"

namespace mod::sndpatch
{
    using namespace spm;

    s32 evt_sndpatch_get_current_buffer(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        u8 getPlayer = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        nw4r::snd::Snd_BasicSound * getBasicSnd = spmario_snd::spsndBgmPlayers[getPlayer].soundHandle.mSound;
        typedef nw4r::snd::BasicPlayer* (*GetBasicPlayerFuncType)(nw4r::snd::Snd_BasicSound *sound);
        GetBasicPlayerFuncType getBasicPlayerFunc = (GetBasicPlayerFuncType)getBasicSnd->__vt->GetBasicPlayer;
        nw4r::snd::BasicPlayer * basicPlayerGet = getBasicPlayerFunc(getBasicSnd);
        evtmgr_cmd::evtSetValue(evtEntry, args[1], basicPlayerGet->bufferCheck1);
        return 2;
    }

    s32 evt_sndpatch_bgm_align(evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        u8 setPlayer = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        u8 getPlayer = evtmgr_cmd::evtGetValue(evtEntry, args[1]);

        nw4r::snd::Snd_BasicSound * setBasicSnd = spmario_snd::spsndBgmPlayers[setPlayer].soundHandle.mSound;
        wii::os::OSReport("setBasicSnd %p.\n", setBasicSnd);
        
        nw4r::snd::Snd_BasicSound * getBasicSnd = spmario_snd::spsndBgmPlayers[getPlayer].soundHandle.mSound;
        wii::os::OSReport("getBasicSnd %p.\n", getBasicSnd);
        typedef nw4r::snd::BasicPlayer* (*GetBasicPlayerFuncType)(nw4r::snd::Snd_BasicSound *sound);
        GetBasicPlayerFuncType getBasicPlayerFunc = (GetBasicPlayerFuncType)setBasicSnd->__vt->GetBasicPlayer;
        wii::os::OSReport("getBasicSnd %p.\n", getBasicPlayerFunc);
        nw4r::snd::BasicPlayer * basicPlayerSet = getBasicPlayerFunc(setBasicSnd);
        getBasicPlayerFunc = (GetBasicPlayerFuncType)getBasicSnd->__vt->GetBasicPlayer;
        nw4r::snd::BasicPlayer * basicPlayerGet = getBasicPlayerFunc(getBasicSnd);

        wii::os::OSReport("BEFORE: basicPlayerGet Progress: %d. basicPlayerSet Progress: %d.\n", basicPlayerGet->progress, basicPlayerSet->progress);
        basicPlayerSet->progress = basicPlayerGet->progress;
        basicPlayerSet->progressLoop = basicPlayerGet->progressLoop;
    //    nw4r::snd::UpdateLoadingBlockIndex(basicPlayerSet);
    //    nw4r::snd::UpdateLoadingBlockIndex(basicPlayerGet);
        wii::os::OSReport("AFTER: basicPlayerGet Progress: %d. basicPlayerSet Progress: %d. basicPlayerSet struct is 0x%x.\n", basicPlayerGet->progress, basicPlayerSet->progress, basicPlayerSet);
    //    spmario_snd::spsndBGMFadein(setPlayer, 1);
        return 2;
    }

}