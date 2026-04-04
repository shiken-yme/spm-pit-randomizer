#pragma once

#include <common.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <spm/camdrv.h>

CPP_WRAPPER(spm::framedrv)

USING(wii::gx::GXColor)
USING(wii::mtx::Vec2)
USING(wii::mtx::Vec2i)
USING(wii::mtx::Vec3)
USING(spm::camdrv::CameraId)

#define FRAME_ACTIVE 0x1
#define FRAME_LINE_THOREAU_GRAB 0x2
#define FRAME_CURSOR_DISP 0x4
#define FRAME_CURSOR_FADEOUT 0x8
#define FRAME_WIRE_DRAW_FINISHED 0x10
#define FRAME_FLAG_0x20 0x20
#define FRAME_OFFSCREEN_DRAW 0x40
#define FRAME_OFFSCREEN_DRAW_FINISHED 0x80
#define FRAME_LINE_DRAW_FINISHED 0x100
#define FRAME_FLAG_0x200 0x200
#define FRAME_WIRE_DRAW 0x400
#define FRAME_FLAG_0x800 0x800
#define FRAME_OFFSCREEN_FUNC_RUNNING 0x1000

enum FrameType : u16
{
/* 0x0 */ FRAME_TYPE_OFFSCREEN,
/* 0x1 */ FRAME_TYPE_WIRE,
/* 0x2 */ FRAME_TYPE_LINE,
/* 0x3 */ FRAME_TYPE_EVT,
/* 0x4 */ FRAME_TYPE_MAX
};

typedef struct
{
/* 0x00 */ u16 flags; // undocumented
/* 0x02 */ u16 pad_0x02;
/* 0x04 */ f32 unknown_0x04;
/* 0x08 */ s32 grpIndices;
/* 0x0C */ s32 unknown_0x0c;
/* 0x10 */ void * grpIndexMap; // probably a pointer to a struct, members unknown; total allocated size is (grpIndices * 2)
} FrameWireSub;
SIZE_ASSERT(FrameWireSub, 0x14)

typedef struct
{
/* 0x000 */ u16 flags; // see defines above
/* 0x002 */ FrameType type;
/* 0x004 */ char instanceName[16];
/* 0x014 */ s32 animPoseId;
/* 0x018 */ char animPoseName[32];
/* 0x038 */ char animDef[32];
/* 0x058 */ Vec2 screenPos; // origin @ center of screen
/* 0x060 */ Vec2 size; // (length, height)
/* 0x068 */ Vec2 destScreenPos;
/* 0x070 */ Vec2 destSize;
/* 0x078 */ Vec2i lineScissorScreenPos; // origin @ top-left corner of screen
/* 0x080 */ Vec2i lineScissorDestSize;
/* 0x088 */ s32 drawState; // functionality depends on frame type
/* 0x08C */ s32 drawIntplProgress;
/* 0x090 */ u32 unused_0x90;
/* 0x094 */ GXColor color;
/* 0x098 */ s32 cursorAlpha;
/* 0x09C */ s32 lineWidth;
/* 0x0A0 */ s32 offscreenId;
/* 0x0A4 */ char offscreenName[16];
/* 0x0B4 */ void * offscreenCb; // initiates offscreen ID @ relevant drivers & clears them
/* 0x0B8 */ Vec3 rotation;
/* 0x0C4 */ Vec3 evtWorldPos;
/* 0x0D0 */ Vec3 wireWorldPos;
/* 0x0DC */ f32 wireDrawScale;
/* 0x0E0 */ s32 wireLineWidth;
/* 0x0E4 */ GXColor wireColor;
/* 0x0E8 */ f32 drawSpeed;
/* 0x0EC */ Vec3 wireDrawRotation;
/* 0x0F8 */ CameraId wireCamId;
/* 0x0FC */ FrameWireSub * wireSubs[1]; // pointer to a VLA of size wireSubCount
/* 0x100 */ s32 wireSubCount;
} FrameEntry;
SIZE_ASSERT(FrameEntry, 0x104)

typedef struct
{
/* 0x00 */ u8 unknown_0x00[0x34 - 0x00];
} SpinDeviceWork;
SIZE_ASSERT(SpinDeviceWork, 0x34)

typedef struct
{
/* 0x0 */ s32 num;
/* 0x4 */ FrameEntry * entries;
} FrameWork;
SIZE_ASSERT(FrameWork, 0x8)

DECOMP_STATIC(FrameWork * framedrv_wp)

void frameInit();

void frameReInit();

/*
    Runs a display callback or draw function for all frame entries
*/
void frameMain();

/*
    Handles draw logic for frame type Offscreen, allocates capture for the bound image entry
*/
void frameDrawOffscreen(FrameEntry * frame);

/*
    Handles draw logic for frame type Line
*/
void frameDrawLine(FrameEntry * frame);

/*
    Gets a frame entry from its instance name
*/
FrameEntry * frameNameToPtr(const char * instanceName);

void frameOffscreenGX(s32 p1);

/*
    Creates a frame of type Offscreen
    Screen position parameters have an origin at the top-left corner of the screen, with positive X going right and positive Y going down
    Offscreen frames are used for flipboxes that have bound offscreen/img entries, e.g. flipping, switching char/pixl, respawning, using Fleep
*/
FrameEntry * frameOffscreenEntry(const char * instanceName, const char * animPoseName, const char * animDef, f32 scrPosX, f32 scrPosY, f32 destWidth, f32 destHeight);

/*
    Creates a frame of type Wire
    The draw scale parameter handles the size of anim def drawing, in case the model is scaled up at runtime; position parameter is a world position
    Wire frames are used for drawing lines around a model's animation definition, e.g. Mega Star animation
*/
FrameEntry * frameWireEntry(const char * instanceName, const char * animPoseName, const char * animDef, Vec3 * position, f32 drawScale);

/*
    Creates a frame of type Line
    Screen position parameters have an origin at the top-left corner of the screen, with positive X going right and positive Y going down
    Line frames are used for drawing boxes that don't need offscreen entries but maybe necessitate a dispScissor, e.g. item events, Thoreau grab, Dottie shrink/grow
*/
FrameEntry * frameLineEntry(const char * instanceName, f32 scrPosX, f32 scrPosY, f32 destWidth, f32 destHeight);

/*
    Creates a FrameLine entry and assigns it the FRAME_LINE_THOREAU_GRAB flag; used exclusively in mot_grab
*/
void frameLineGrabEntry(const char * instanceName, f32 scrPosX, f32 scrPosY, f32 destWidth, f32 destHeight);

/*
    Creates a frame of type Evt
    Position parameter is a world position
    Evt frames are almost exclusively used for spinning devices, but they can also be used as an alternative to a line frame that doesn't need a dispScissor, e.g. pre-2 Peach frame
*/
void frameEvtEntry(const char * instanceName, Vec3 * position, f32 width, f32 height);

/*
    Updates the screen position of an offscreen-type frame and its img entry if the frame is no longer being drawn
    Pos origin at top-left
*/
void frameOffscreenUpdateScreenPos(const char * instanceName, f32 scrPosX, f32 scrPosY);

/*
    Binds an offscreen entry to an offscreen-type frame
*/
void frameBindOffscreenEntry(const char * instanceName, const char * offsInstanceName);

/*
    Binds a callback function to an offscreen-type frame
    This function is called to initiate the offscreen ID at any relevant drivers so that an image entry can capture their visual data, and called again to clear them when necessary
*/
void frameOffscreenSetCallback(const char * instanceName, void * callback);

/*
    Sets the animation definition for an offscreen-type frame's img entry
    Animation does not loop by default, displays immediately
*/
void frameOffscreenSetImgAnim(const char * instanceName, const char * imgAnimDef);

/*
    Allocates a new capture for an offscreen-type frame's img entry
*/
void frameOffscreenUpdateImg(const char * instanceName);

/*
    Returns true if an offscreen-type frame's img entry's animation has finished
*/
bool frameOffscreenCheckImgAnimFinished(const char * instanceName);

/*
    Returns true if an offscreen-type frame has been fully drawn
*/
bool frameOffscreenCheckDrawFinished(const char * instanceName);

/*
    Returns true if a wire-type frame has been fully drawn
*/
bool frameWireCheckDrawFinished(const char * instanceName);

/*
    Returns true if a line-type frame has been fully drawn
*/
bool frameLineCheckDrawFinished(const char * instanceName);

/*
    Clears data for and deactivates a frame entry of any type
*/
void frameDeleteEntry(const char * instanceName);

/*
    Returns a Z scale variable that gets passed into GXSetZScaleOffset
    Position parameter is a world position
*/
f32 frameEvtGetZScale(Vec3 * position);

/*
    Used as a dispEntry callback for all frame types except for Wire
*/
void frameDisp(CameraId camId, FrameEntry * frame);

/*
    Displays the cursor that "draws" line- and offscreen-type frames
*/
void frameCursorDisp(CameraId camId, FrameEntry * frame);

/*
    Used as a dispEntry callback for wire-type frames
*/
void frameWireDisp(CameraId camId, FrameEntry * frame);

/*
    Sets a world position to begin drawing a wire-type frame at @ posOut
*/
void frameWireGetDrawStartPos(const char * instanceName, const char * animDef, Vec3 * posOut);

CPP_WRAPPER_END()
