#include <common.h>
#include <lunatic/localize.h>
#include <rfcdrv.h>
#include <rewrite.h>
#include <spm/evtmgr_cmd.h>
#include <spm/romfont.h>
#include <spm/relmgr.h>
#include <spm/seq_mapchange.h>
#include <spm/spmario.h>
#include <spm/swdrv.h>
#include <wii/DEMOInit.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <wii/ipc.h>
#include <wii/os.h>
#include <wii/vi.h>
#include <msl/string.h>

#include "mod.h"
#include "patch.h"
#include "util.h"

namespace mod
{

    /*
        This code is pretty much copied 1:1 from Seeky's Practice Codes mod with some cosmetic changes & other dependencies thrown in.
        I needed my own crash handler so others can help me debug Lunatic Pit, and making one of those is beyond my abilities...
    */

    bool gIsDolphin;

    extern "C"
    {
        bool inOSPanic = false;
        char exceptionWorkingText[256];

        void OSPanicForwarder();
        void exceptionOSReportForwarder();
    }

    static bool inException = false;
    static char exceptionText[4096];
    static u32 head = 0;
    static void (*__OSUnhandledExceptionReal)(u8 p1, wii::os::OSContext *p2, u32 p3, u32 p4);
    static spm::evtmgr::EvtScriptCode *lastScript = nullptr;
    static s32 (*evtmgrCmdReal)(spm::evtmgr::EvtEntry *entry) = nullptr;

#define SCREEN_TOP 228.0f
#define SCREEN_BOTTOM -228.0f
#define TITLE_Y 195.0f
#define LINE_HEIGHT 15.0f
#define TEXT_TOP (TITLE_Y - (LINE_HEIGHT * 3) - 5)
#define TEXT_BOTTOM (SCREEN_BOTTOM - LINE_HEIGHT)
#define TEXT_LEFT -300.0f

    static wii::gx::GXColor titleColour{0xff, 0x60, 0xff, 0xff};

    static wii::gx::GXColor textColour{0xff, 0xff, 0xff, 0xff};

    static void drawTitle(f32 scale)
    {
        spm::romfont::romFontPrintGX(TEXT_LEFT, TITLE_Y, scale, &titleColour,
                                     "Oops! The game crashed...      " MOD_VERSION " on %s Revision %d",
                                     getGameRegion(), getGameRevision());
        if (msl::string::strstr(spm::spmario::gp->mapName, "dan") != nullptr)
        {
            u8 floor = (spm::swdrv::swByteGet(1) - 1);
            FloorData *Data = &Lunatic->Floor[floor];
            if (floor % 10 != 9)
                spm::romfont::romFontPrintGX(TEXT_LEFT, TITLE_Y - LINE_HEIGHT, scale, &titleColour,
                                             "Floor %d - %dx %s, %dx %s, %dx %s, %dx %s", floor + 1, Data->Enemies[0].num, npcGetNameFromTribeId(Data->Enemies[0].name - 1),
                                             Data->Enemies[1].num, npcGetNameFromTribeId(Data->Enemies[1].name - 1),
                                             Data->Enemies[2].num, npcGetNameFromTribeId(Data->Enemies[2].name - 1),
                                             Data->Enemies[3].num, npcGetNameFromTribeId(Data->Enemies[3].name - 1));
            else
                spm::romfont::romFontPrintGX(TEXT_LEFT, TITLE_Y - LINE_HEIGHT, scale, &titleColour,
                                             "Floor %d - Chest is %s, NPC is %s", floor + 1, RFCRarityNames[Lunatic->RFC.chestRarity], restFloorNpcNames[swdrv::swByteGet(1623)]);
        }
        else
            spm::romfont::romFontPrintGX(TEXT_LEFT, TITLE_Y - LINE_HEIGHT, scale, &titleColour,
                                             "Crash occurred in gp \"%s\" - seq_mapchange_wp \"%s\"", spmario::gp->mapName, seq_mapchange::seq_mapchange_wp->mapName);
        spm::romfont::romFontPrintGX(TEXT_LEFT, TITLE_Y - (LINE_HEIGHT * 2), scale, &titleColour,
                                     "Show this crash to Yme in Flipside Tower mod-discussion, please! discord.gg/ndrxwcyCum");
    }

    static void draw(char *msg, f32 yShift, f32 scale)
    {
        char *p = msg;
        bool done = false;
        const f32 x = TEXT_LEFT;
        f32 y = 200.0f - yShift;
        while (!done)
        {
            // Find end of line
            char *q = p;
            while ((*q != '\n') && (*q != '\0'))
                q++;

            // Split line into its own string temporarily
            done = *q == '\0';
            *q = '\0';

            // Draw line if on screen
            if ((y >= TEXT_BOTTOM) && (y <= TEXT_TOP))
                spm::romfont::romFontPrintGX(x, y, scale, &textColour, p);

            // Move to next line
            y -= LINE_HEIGHT;
            p = q + 1;

            // Restore string
            if (!done)
                *q = '\n';
        }
    }

    static f32 getBottomY(char *msg)
    {
        // Count newlines
        int n = 0;
        for (int i = 0; msg[i]; i++)
        {
            if (msg[i] == '\n')
                n++;
        }

        // Calculate
        return TEXT_TOP - (LINE_HEIGHT * n);
    }

    extern "C" void exceptionMessageHandler(char *msg)
    {
        // spm::wpadmgr::wpadAllRumbleOff(0);
        // spm::spmario_snd::spsndExit();

        // Stop all other threads
        wii::os::OSThread *p = wii::os::OS_CURRENT_THREAD->link.prev;
        while (p != nullptr)
        {
            wii::os::OSSuspendThread(p);
            p = p->link.prev;
        }
        p = wii::os::OS_CURRENT_THREAD->link.next;
        while (p != nullptr)
        {
            wii::os::OSSuspendThread(p);
            p = p->link.next;
        }

        // Print to OSReport
        wii::os::OSReport("%s\n", msg);

        const f32 topY = 50.0f;
        f32 bottomY = getBottomY(msg);
        f32 yShift = topY;
        f32 delta = bottomY <= SCREEN_BOTTOM ? 1.0f : 0.0f;
        f32 scale = gIsDolphin ? 0.7f : 0.55f; // dolphin uses a custom font for copyright reasons
        while (true)
        {
            // Check if power button was pressed
            if (spm::spmario::spmario_doShutdown)
            {
                wii::vi::VISetBlack(1);
                wii::vi::VIFlush();
                wii::vi::VIWaitForRetrace();
                wii::vi::VIWaitForRetrace();
                wii::vi::VIWaitForRetrace();
                wii::os::OSShutdownSystem();
                while (true)
                {
                };
            }

            // Start frame
            wii::mtx::Mtx44 mtx;
            wii::DEMOInit::DEMOBeforeRender();
            wii::mtx::C_MTXOrtho(mtx, SCREEN_TOP, SCREEN_BOTTOM, -304.0f, 304.0f, 1.0f, 1000.0f);
            wii::gx::GXSetProjection(mtx, wii::gx::GX_ORTHOGRAPHIC);

            // Draw game & mod version header
            drawTitle(scale);

            // Render main text
            draw(msg, yShift, scale);

            // Scroll for next frame
            if ((yShift >= topY) || (yShift <= bottomY))
                delta *= -1.0f;
            yShift += delta;

            // End frame
            wii::DEMOInit::DEMODoneRender();
        }
    }

    extern "C" void exceptionOSReport(const char *msg)
    {
        // Print to OSReport
        wii::os::OSReport(msg);

        // Store message to be drawn to screen
        size_t len = msl::string::strlen(msg);
        if ((head + len) >= sizeof(exceptionText))
            return;
        msl::string::strcpy(exceptionText + head, msg);
        head += len;
    }

    void exceptionDraw()
    {
        exceptionMessageHandler(exceptionText);
    }

    void checkExceptionFlags()
    {
        if (inException)
        {
            wii::os::OSReport("WARNING: Exception handler has crashed!\n");
            while (true)
            {
            };
        }

        if (inOSPanic)
            exceptionOSReport("WARNING: OSPanic handler has crashed!\n");
    }

    static void checkForDolphin()
    {
        // Thanks to TheLordScruffy for telling me about this
        gIsDolphin = wii::ipc::IOS_Open("/sys", 1) == -106;

        // If they ever fix that, it'll be in a version that's definitely new enough to have /dev/dolphin
        if (!gIsDolphin)
        {
            int ret = wii::ipc::IOS_Open("/dev/dolphin", 0);
            if (ret >= 0)
            {
                gIsDolphin = true;
                wii::ipc::IOS_Close(ret);
            }
        }
    }

    void exceptionPatch()
    {
        checkForDolphin();

        // OSPanic
        writeBranch(wii::os::OSPanic, 0x130, OSPanicForwarder);

        // __OSUnhandledException
        writeBranchLink(wii::os::__OSUnhandledException, 0x50, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x1b0, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x1bc, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x1d8, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x1ec, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x220, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x234, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x24c, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x264, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x274, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x28c, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x2a0, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x2b4, exceptionOSReportForwarder);
        writeBranchLink(wii::os::__OSUnhandledException, 0x2d0, exceptionOSReportForwarder);
        writeBranch(wii::os::__OSUnhandledException, 0x2d4, exceptionDraw);
        __OSUnhandledExceptionReal = patch::hookFunction(wii::os::__OSUnhandledException,
                                                         [](u8 p1, wii::os::OSContext *p2, u32 p3, u32 p4)
                                                         {
                                                             checkExceptionFlags();
                                                             inException = true;
                                                             __OSUnhandledExceptionReal(p1, p2, p3, p4);
                                                         });
        wii::os::__OSUnhandledException_msg1[73] = '\n';
        wii::os::__OSUnhandledException_msg2[75] = '\n';
        wii::os::__OSUnhandledException_msg3[72] = '\n';

        // OSDumpContext
        writeBranchLink(wii::os::OSDumpContext, 0x2c, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0x58, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0x7c, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0x90, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0x9c, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0xc0, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0x120, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0x158, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0x174, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0x1ac, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0x1fc, exceptionOSReportForwarder);
        writeBranchLink(wii::os::OSDumpContext, 0x220, exceptionOSReportForwarder);

        // Track last script
        evtmgrCmdReal = patch::hookFunction(spm::evtmgr_cmd::evtmgrCmd,
                                            [](spm::evtmgr::EvtEntry *entry)
                                            {
                                                lastScript = entry->scriptStart;
                                                return evtmgrCmdReal(entry);
                                            });
    }

}
