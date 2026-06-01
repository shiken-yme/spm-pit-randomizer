#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_mario)

// evt_mario_flag_onoff(bool onOff, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mario_flag_onoff, 2)

// evt_mario_misc_flag_onoff(bool onOff, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mario_misc_flag_onoff, 2)

// evt_mario_disp_flag_onoff(bool onOff, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mario_disp_flag_onoff, 2)

// evt_mario_get_flags(s32 type, u32 flags)
// type 0 is flags, 1 is misc flags, 2 is disp flags
EVT_DECLARE_USER_FUNC(evt_mario_get_flags, 2)

// evt_mario_ctrl_onoff(bool onOff)
EVT_DECLARE_USER_FUNC(evt_mario_ctrl_onoff, 1)

// evt_mario_key_on()
EVT_DECLARE_USER_FUNC(evt_mario_key_on, 0)

// evt_mario_key_off(s32)
EVT_DECLARE_USER_FUNC(evt_mario_key_off, 1)

// evt_mario_key_off_immediate()
EVT_DECLARE_USER_FUNC(evt_mario_key_off_immediate, 0)

EVT_DECLARE_USER_FUNC(func_800ef814, 1)

// evt_mario_bg_mode_onoff(bool onOff)
EVT_DECLARE_USER_FUNC(evt_mario_bg_mode_onoff, 1)

// evt_mario_get_character(s32& ret)
EVT_DECLARE_USER_FUNC(evt_mario_get_character, 1)

// evt_mario_set_character(s32 character)
EVT_DECLARE_USER_FUNC(evt_mario_set_character, 1)

// evt_mario_set_pos(f32 x, f32 y, f32 z)
EVT_DECLARE_USER_FUNC(evt_mario_set_pos, 3)

// evt_mario_get_pos(f32& x, f32& y, f32& z)
EVT_DECLARE_USER_FUNC(evt_mario_get_pos, 3)

EVT_DECLARE_USER_FUNC(func_800efac4, 3)

// evt_mario_set_scale(f32 x, f32 y, f32 z)
EVT_DECLARE_USER_FUNC(evt_mario_set_scale, 3)

// evt_mario_get_scale(f32& x, f32& y, f32& z)
EVT_DECLARE_USER_FUNC(evt_mario_get_scale, 3)

// evt_mario_set_axis_rotation(f32 x, f32 y, f32 z)
EVT_DECLARE_USER_FUNC(evt_mario_set_axis_rotation, 3)

// evt_mario_get_height(f32& ret)
EVT_DECLARE_USER_FUNC(evt_mario_get_height, 1)

// evt_mario_face_left()
EVT_DECLARE_USER_FUNC(evt_mario_face_left, 0)

// evt_mario_face_right()
EVT_DECLARE_USER_FUNC(evt_mario_face_right, 0)

// evt_mario_direction_face(f32 degrees, s32 time)
EVT_DECLARE_USER_FUNC(evt_mario_direction_face, 2)

// evt_mario_get_direction_view(f32& ret)
EVT_DECLARE_USER_FUNC(evt_mario_get_direction_view, 1)

// evt_mario_face_npc(const char * name)
EVT_DECLARE_USER_FUNC(evt_mario_face_npc, 1)

// evt_mario_face_coords(f32 x, f32 z)
EVT_DECLARE_USER_FUNC(evt_mario_face_coords, 2)

// evt_mario_adjust_move_dir()
EVT_DECLARE_USER_FUNC(evt_mario_adjust_move_dir, 0)

// evt_mario_face_away()
EVT_DECLARE_USER_FUNC(evt_mario_face_away, 0)

// evt_mario_set_disp_direction(f32 degrees)
EVT_DECLARE_USER_FUNC(evt_mario_set_disp_direction, 1)

// evt_mario_get_disp_direction(f32& degrees)
EVT_DECLARE_USER_FUNC(evt_mario_get_disp_direction, 1)

// evt_mario_face(f32 x, f32 y, f32 z)
EVT_DECLARE_USER_FUNC(evt_mario_face, 3)

// evt_mario_face_free()
EVT_DECLARE_USER_FUNC(evt_mario_face_free, 0)

// evt_mario_walk_to(f32 x, f32 z, s32 msec)
EVT_DECLARE_USER_FUNC(evt_mario_walk_to, 3)

EVT_DECLARE_USER_FUNC(evt_mario_pos_change, 3)

// evt_mario_walk_to2(f32 x, f32 y, f32 z, f32 v)
EVT_DECLARE_USER_FUNC(evt_mario_walk_to2, 4)

EVT_DECLARE_USER_FUNC(evt_mario_move_pos_look, 6)

EVT_DECLARE_USER_FUNC(evt_mario_move_pos_look2, 6)

bool evtMarioChkLandedFromJump(f32 jumpSpdY);

// evt_mario_jump_to(f32 x, f32 y, f32 z, f32 jumpHeight, s32 msec)
EVT_DECLARE_USER_FUNC(evt_mario_jump_to, 5)

EVT_DECLARE_USER_FUNC(evt_mario_jump_to2, 5)

// evt_mario_wait_grounded()
EVT_DECLARE_USER_FUNC(evt_mario_wait_grounded, 0)

UNKNOWN_FUNCTION(func_800f1778)
UNKNOWN_FUNCTION(func_800f1810)
UNKNOWN_FUNCTION(func_800f1858)

// evt_mario_set_pose(const char * name, s16 time)
EVT_DECLARE_USER_FUNC(evt_mario_set_pose, 2)

EVT_DECLARE_USER_FUNC(evt_mario_wait_anim, 0)
UNKNOWN_FUNCTION(func_800f1a08)
EVT_UNKNOWN_USER_FUNC(func_800f1a4c)
UNKNOWN_FUNCTION(func_800f1abc)
UNKNOWN_FUNCTION(func_800f1b08)
UNKNOWN_FUNCTION(func_800f1ba8)
UNKNOWN_FUNCTION(func_800f1c1c)
UNKNOWN_FUNCTION(func_800f1c88)
UNKNOWN_FUNCTION(func_800f1d0c)
UNKNOWN_FUNCTION(func_800f1d80)
UNKNOWN_FUNCTION(func_800f1e30)
UNKNOWN_FUNCTION(func_800f1eb0)
UNKNOWN_FUNCTION(func_800f1f30)
UNKNOWN_FUNCTION(func_800f1f9c)
UNKNOWN_FUNCTION(func_800f2008)
UNKNOWN_FUNCTION(func_800f2074)
UNKNOWN_FUNCTION(func_800f2124)
UNKNOWN_FUNCTION(func_800f212c)
UNKNOWN_FUNCTION(func_800f2144)
UNKNOWN_FUNCTION(func_800f2310)
EVT_UNKNOWN_USER_FUNC(func_800f23e4)
EVT_UNKNOWN_USER_FUNC(func_800f240c)

EVT_DECLARE_USER_FUNC(evt_mario_fairy_reset, 0)

UNKNOWN_FUNCTION(evt_mario_swim_onoff)
UNKNOWN_FUNCTION(func_800f24d8)
UNKNOWN_FUNCTION(func_800f2544)
EVT_UNKNOWN_USER_FUNC(evt_mario_set_gravity)
UNKNOWN_FUNCTION(evt_get_gravity)
UNKNOWN_FUNCTION(func_800f262c)
UNKNOWN_FUNCTION(func_800f267c)
UNKNOWN_FUNCTION(func_800f26c0)
UNKNOWN_FUNCTION(func_800f27f4)
UNKNOWN_FUNCTION(func_800f2974)

// evt_mario_take_damage(s32 type, f32 x, f32 y, f32 z, s32 dmgFlags, s32 damage)
// type 1 = no damage vector, flags = 0, dmg = 1
// type 2 = no damage vector; flags and dmg taken from params
// type 3 = all data taken from params
EVT_DECLARE_USER_FUNC(evt_mario_take_damage, 6)

UNKNOWN_FUNCTION(evt_mario_tamara_onoff)
UNKNOWN_FUNCTION(evt_mario_tamara_chg_mode)
UNKNOWN_FUNCTION(func_800f2c00)
UNKNOWN_FUNCTION(func_800f2c98)
UNKNOWN_FUNCTION(func_800f2cbc)
UNKNOWN_FUNCTION(func_800f2cfc)
UNKNOWN_FUNCTION(func_800f2d74)
UNKNOWN_FUNCTION(func_800f2df4)
UNKNOWN_FUNCTION(func_800f2e30)
UNKNOWN_FUNCTION(func_800f2e54)
UNKNOWN_FUNCTION(evt_mario_set_bottomless_cb)
UNKNOWN_FUNCTION(evt_mario_get_bottomless_cb)

// evt_mario_set_anim_change_handler(MarioAnimChangeHandler * handler)
EVT_DECLARE_USER_FUNC(evt_mario_set_anim_change_handler, 1)

UNKNOWN_FUNCTION(func_800f2fa8)
UNKNOWN_FUNCTION(func_800f2fec)
UNKNOWN_FUNCTION(func_800f3074)
UNKNOWN_FUNCTION(func_800f30bc)
UNKNOWN_FUNCTION(func_800f315c)
UNKNOWN_FUNCTION(evt_mario_set_pane_boundaries)
UNKNOWN_FUNCTION(evt_mario_get_pane_for_pos)
UNKNOWN_FUNCTION(evt_mario_set_pane)
UNKNOWN_FUNCTION(evt_mario_pane_change_func)
UNKNOWN_FUNCTION(evt_mario_get_pane_change_func)
UNKNOWN_FUNCTION(func_800f3310)
UNKNOWN_FUNCTION(func_800f3334)
EVT_DECLARE_USER_FUNC(evt_mario_check_3d, 1)
UNKNOWN_FUNCTION(func_800f33b0)
UNKNOWN_FUNCTION(evt_mario_calc_damage_to_enemy)

CPP_WRAPPER_END()
