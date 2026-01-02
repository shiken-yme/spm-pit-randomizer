#include "patch.h"
#include "msgpatch.h"

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/msgdrv.h>
#include <spm/system.h>
#include <wii/os.h>
#include <msl/string.h>

namespace mod::msgpatch
{
    /*
        Welcome to MsgPatch, developed by Yme~
        This library rewrites the vanilla functions msgSearch and msgSearchNoFallback with an intuitive system that allows you to easily patch vanilla text.
        Adding an entry to MsgPatch will force the game to use a provided pointer to new text for ANY identifier, provided the ptr to new text is still intact.

        Here's how it works:
        The game reads a message identifier named "ename_000", which causes msgSearch to scan the vanilla text files for a matching message with that identifier.
        It finds a match: "ename_000" corresponds to the string "Goomba", so it prints that text.
        This library's rewritten msgSearch will scan all MsgPatchPtrs entries (msgpatchEntries) before reading vanilla text files.
        If a MsgPatch entry with the identifier "ename_000" is present, it will print its corresponding newText instead of the vanilla text "Goomba".

        MsgPatch allows you to add multiple references to the same identifier ("layering"), and can also halt if the same identifier is already present in MsgPatchPtrs.
        The most recently-added entry with a matching identifier will always take priority in both msgpatchSearch and msgpatchDelEntry.
        Whether or not to allow layering for a given identifier is left up to you. Layering may be helpful for complex variable-dependent text events. 

        -----------------------------------------------------------------------------------------------------------------------------------------------------------------

        To add an entry to MsgPatch, use msgpatchAddEntry() in a function, or evt_msgpatch_add_entry in an EVT script.
        Simply provide the desired identifier (msgName), the pointer to new text to print (newText), and provide whether you'd like to allow layering for that identifier.
        If layering is disallowed and the identifier is already present in msgpatchEntries, the process will end.

        EXAMPLE:
        // Use msgpatchAddEntry() to cause the game to display "Gombert" instead of "Goomba" in the Catch Card menu
        const char *gombertId = "ename_000";
        const char *gombertTxt = "Gombert";
        msgpatch::msgpatchAddEntry(gombertId, gombertTxt, false);
        OR
        // Use evt_msgpatch_add_entry to do the same thing
        USER_FUNC(msgpatch::evt_msgpatch_add_entry, PTR(gombertId), PTR(gombertTxt), 0)

        -----------------------------------------------------------------------------------------------------------------------------------------------------------------

        To remove an entry from MsgPatch, use msgpatchDelEntry() in a function, or evt_msgpatch_del_entry in an EVT script.
        You need only provide the identifier. If a match is not found, the process will end.
        Deleting an entry can be useful if you only want to temporarily replace text for a vanilla string; this is especially useful for making custom item select menus.

        EXAMPLE:
        // Use msgpatchDelEntry() to cause the game to stop displaying "Goomba" as "Gombert", assuming that MsgPatch entry was created
        msgpatch::msgpatchDelEntry(gombertId);
        OR
        // Use evt_msgpatch_del_entry to do the same thing
        USER_FUNC(msgpatch::evt_msgpatch_del_entry, PTR(gombertId))

        -----------------------------------------------------------------------------------------------------------------------------------------------------------------

        Here's how you implement a textbox into MsgPatch:
        const char text[] =
        "This is a test textbox! If\n"
        "<wait 500>you like this library, please\n"
        "buy me some Thai food.<wait 500> <wave>:3\n"
        "</wave><k>";
        const char *pText = text;
        const char *textId = "anna_ehelp_410";
        msgpatch::msgpatchAddEntry(textId, pText, false); // Override the Tippi tattle for the Ice Cherbil

        -----------------------------------------------------------------------------------------------------------------------------------------------------------------
        
        Final notes:
        
        Due to the way different parts of the game choose to display certain item/card descriptions, msgSearchNoFallback has been patched to not only scan for matching msgpatchEntries,
        but to also search for matching "_ex" and "_ex2" appendices, which (potentially) represent different descriptions for each item.
        For ease of use, you need not include these appendices in a MsgPatch entry if you do not wish to differentiate them; this library will automatically handle that for you.

        MsgPatch immensely streamlines the process of replacing vanilla text; before this library, we had to hook into msgSearch and check for every string we wanted individually.
        If you'd like to support my work, I have a Ko-Fi! Donations are always welcome but never expected: https://ko-fi.com/tarttapple

        For feature requests and bug reports, please join the SPM Speedrunning Discord and then either DM me (@tartt) or send a message in SPM Speedrunning #tools-dev.
        Have fun~ :3c
    */

    using namespace spm;

    MsgPatchPtrs msgpatchEntries[512];

    void msgpatchAddEntry(const char *msgName, const char *newText, bool allowDuplicate)
    {
        // msgpatchAddEntry adds an entry to the lowest possible empty slot.
        // In the event of duplicate entries, if allowed, the most recent (highest-numbered) slot takes priority in msgpatchSearch and msgpatchDelEntry.
        if (newText == nullptr)
        {
            wii::os::OSReport("MsgPatch: AddEntry call failed; identifier \"%s\" provides a null pointer to newText.\n", msgName);
            return;
        }
        s32 i = 0;
        while (i < 512)
        {
            if (!allowDuplicate)
            {
                if (msgpatchEntries[i].identifier != nullptr)
                {
                    if (msl::string::strcmp(msgpatchEntries[i].identifier, msgName) == 0)
                    {
                        wii::os::OSReport("MsgPatch: AddEntry call failed; duplicate identifier \"%s\" detected.\n", msgName);
                        return;
                    }
                }
            }
            if (msgpatchEntries[i].identifier == nullptr)
            {
                msgpatchEntries[i].identifier = msgName;
                msgpatchEntries[i].newText = newText;
                wii::os::OSReport("MsgPatch: AddEntry call successful; identifier \"%s\" added to MsgPatchPtrs slot %d.\n", msgName, i);
                return;
            }
            i = i + 1;
        }
        wii::os::OSReport("MsgPatch: AddEntry call failed; maximum limit of 512 MsgPatchPtrs reached, please chill out or reach out to Yme.\n");
        return;
    }

    void msgpatchDelEntry(const char *msgName)
    {
        // msgpatchDelEntry removes the highest-numbered (most recent) matching entry and then sorts the msgpatchEntries array.
        s32 i = 511;
        while (i >= 0)
        {
            if (msgpatchEntries[i].identifier == nullptr)
                goto next;
            if (msl::string::strcmp(msgpatchEntries[i].identifier, msgName) == 0)
            {
                msgpatchEntries[i].identifier = nullptr;
                msgpatchEntries[i].newText = nullptr;
                goto sort;
            }
        next:
            i = i - 1;
        }
        wii::os::OSReport("MsgPatch: DelEntry call failed; identifier %s not found, ending process.\n", msgName);
        return;
    sort:
        wii::os::OSReport("MsgPatch: DelEntry call succesful; %s was removed from MsgPatchPtrs slot %d. Sorting entries.\n", msgName, i);
        while (i < 511) // We don't want this array accessing oob memory lol
        {
            if (msgpatchEntries[i + 1].identifier != nullptr)
            {
                msgpatchEntries[i].identifier = msgpatchEntries[i + 1].identifier;
                msgpatchEntries[i].newText = msgpatchEntries[i + 1].newText;
                i = i + 1;
            }
            else // Eliminate duplicate; already backed up in previous slot
            {
                msgpatchEntries[i].identifier = nullptr;
                msgpatchEntries[i].newText = nullptr;
                break;
            }
        }
        return;
    }

    s32 evt_msgpatch_add_entry(evtmgr::EvtEntry *evtEntry, bool firstCall)
    {
        (void)firstCall;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        const char *identifier = (const char *)evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        const char *newText = (const char *)evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        s32 allowDuplicate = evtmgr_cmd::evtGetValue(evtEntry, args[2]);
        msgpatchAddEntry(identifier, newText, (bool)allowDuplicate);
        return 2;
    }

    s32 evt_msgpatch_del_entry(evtmgr::EvtEntry *evtEntry, bool firstCall)
    {
        (void)firstCall;
        evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
        const char *identifier = (const char *)evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        msgpatchDelEntry(identifier);
        return 2;
    }

    const char *msgpatchSearch(const char *msgName, u32 *containsEx, bool checkForEx)
    {
        // msgpatchSearch starts from the highest entry num and counts down. This is because when duplicate entries are added, we want the most recent one to take priority.
        s32 i = 511;
        bool getOut = false;
        while (i >= 0)
        {
            if (msgpatchEntries[i].identifier == nullptr)
                goto next;
            if (msl::string::strcmp(msgpatchEntries[i].identifier, msgName) == 0)
                getOut = true;
            if (checkForEx)
            {
                if (msl::string::strstr(msgName, "_ex_") != nullptr)
                    goto next;
                if (msl::string::strstr(msgName, "_ex2") != nullptr)
                    *containsEx = 2;
                if (msl::string::strstr(msgName, "_ex") != nullptr)
                    *containsEx = 1;
            }
            if (getOut)
                return msgpatchEntries[i].newText;
        next:
            i = i - 1;
        }
        return nullptr;
    }

    const char *msgSearchNew(const char *msgName)
    {
        // Custom execution sequence
        const char *patchedMsg = msgpatchSearch(msgName, 0, false);
        // wii::os::OSReport("MsgPatch: MsgSearch Call; Init MsgpatchSearch Routine Complete; msgName = %s, patchedMsg = %s\n", msgName, patchedMsg);
        if (patchedMsg != nullptr)
            return patchedMsg;
        // If not custom string, resume vanilla execution
        // wii::os::OSReport("MsgPatch: MsgSearch Call; Vanilla MsgSearch Routine Start; msgName = %s\n", msgName);
        u32 n = 0;
        s32 i = 0;
        s32 idk = 0;
        msgdrv::MsgWork *wp = msgdrv::msgdrv_msgw;
        do
        {
            memory::SmartAllocation *msgs = wp->files[0].messages;
            if (msgs != 0)
            {
                i = 0;
                msgdrv::MsgEntry *entry = (msgdrv::MsgEntry *)msgs->data;
                s32 msgCnt = wp->files[0].messageCount;
                while ((i <= msgCnt && (idk = (i + msgCnt) / 2, idk < wp->files[0].messageCount)))
                {
                    //    wii::os::OSReport("MsgPatch: Vanilla MsgSearch While Loop; i = %d, idk = %d\n", i, idk);
                    s32 strcmp = msl::string::strcmp(msgName, wp->files[0].contents + (s32)entry[idk].nameOffset);
                    if (strcmp == 0)
                        return wp->files[0].contents + (s32)entry[idk].contentsOffset;
                    if (strcmp < 0)
                        msgCnt = idk - 1;
                    if (strcmp > -1)
                        i = idk + 1;
                }
            }
            n = n + 1;
            wp = (msgdrv::MsgWork *)(wp->files + 1);
        } while (n < 9);
        return msgdrv::msgSearch("anna_no_message");
    }

    const char *msgSearchNoFallbackNew(const char *msgName)
    {
        // Custom execution sequence
        u32 containsEx = 0;
        const char *patchedMsg = msgpatchSearch(msgName, &containsEx, true);
        // wii::os::OSReport("MsgPatch: MsgSearchNF Call; Init MsgpatchSearch Routine Complete; msgName = %s, containsEx = %d, patchedMsg = %s\n", msgName, containsEx, patchedMsg);
        if (containsEx > 0)
        {
            if (patchedMsg != nullptr)
            {
                return patchedMsg;
            }
            else
            {
                u32 length = msl::string::strlen(msgName);
                char flexBuf[length];
                msl::string::memset(flexBuf, 0, sizeof(flexBuf));
                if (containsEx == 1)
                {
                    msl::string::strncpy(flexBuf, msgName, (length - 3));
                }
                else
                    msl::string::strncpy(flexBuf, msgName, (length - 4));
                const char *newMsgName = flexBuf;
                // wii::os::OSReport("MsgPatch: MsgSearchNF Call; Msg %s contains EX %d; newMsgName = %s, length = %d.\n", msgName, containsEx, newMsgName, length);
                const char *patchedMsg2 = msgpatchSearch(newMsgName, &containsEx, false);
                if (patchedMsg2 != nullptr)
                    return patchedMsg2;
            }
        }
        else if (patchedMsg != nullptr)
            return patchedMsg;
        // If not custom string, resume vanilla execution
        // wii::os::OSReport("MsgPatch: MsgSearch Call; Vanilla MsgSearch Routine Start; msgName = %s\n", msgName);
        u32 n = 0;
        s32 i = 0;
        s32 idk = 0;
        msgdrv::MsgWork *wp = msgdrv::msgdrv_msgw;
        do
        {
            memory::SmartAllocation *msgs = wp->files[0].messages;
            if (msgs != 0)
            {
                i = 0;
                msgdrv::MsgEntry *entry = (msgdrv::MsgEntry *)msgs->data;
                s32 msgCnt = wp->files[0].messageCount;
                while ((i <= msgCnt && (idk = (i + msgCnt) / 2, idk < wp->files[0].messageCount)))
                {
                    // wii::os::OSReport("MsgPatch: Vanilla MsgSearchNF While Loop; i = %d, idk = %d\n", i, idk);
                    s32 strcmp = msl::string::strcmp(msgName, wp->files[0].contents + (s32)entry[idk].nameOffset);
                    if (strcmp == 0)
                        return wp->files[0].contents + (s32)entry[idk].contentsOffset;
                    if (strcmp < 0)
                        msgCnt = idk - 1;
                    if (strcmp > -1)
                        i = idk + 1;
                }
            }
            n = n + 1;
            wp = (msgdrv::MsgWork *)(wp->files + 1);
        } while (n < 9);
        return (const char *)0;
    }

    void implement()
    {
        patch::hookFunction(msgdrv::msgSearch, msgSearchNew);
        patch::hookFunction(msgdrv::msgSearchNoFallback, msgSearchNoFallbackNew);
    }

    void msgpatchMain()
    {
        implement();
    }
}
