/*
 * Portions of this file are copyright Rebirth contributors and licensed as
 * described in COPYING.txt.
 * Portions of this file are copyright Parallax Software and licensed
 * according to the Parallax license below.
 * See COPYING.txt for license details.

THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

/*
 *
 * Header for fireball.c
 *
 */

#pragma once

#include <physfs.h>

#ifdef __cplusplus
#include "maths.h"
#include "fwd-partial_range.h"
#include "fwd-object.h"
#include "fwd-segment.h"
#include "fwd-vecmat.h"
#include "pack.h"

enum powerup_type_t : uint8_t;

namespace dcx {

struct expl_wall : prohibit_void_ptr<expl_wall>
{
	segnum_t segnum;
	uint8_t sidenum;
	fix time;
};

struct disk_expl_wall
{
	int segnum, sidenum;
	fix time;
};
static_assert(sizeof(disk_expl_wall) == 12, "sizeof(disk_expl_wall) wrong");

// data for exploding walls (such as hostage door)

#define MAX_EXPLODING_WALLS     10
extern array<expl_wall, MAX_EXPLODING_WALLS> expl_wall_list;

}

#ifdef dsx
namespace dsx {
objptridx_t object_create_explosion(vsegptridx_t segnum, const vms_vector &position, fix size, int vclip_type);
void object_create_muzzle_flash(vsegptridx_t segnum, const vms_vector &position, fix size, int vclip_type);

objptridx_t object_create_badass_explosion(objptridx_t objp, vsegptridx_t segnum, const vms_vector &position, fix size, int vclip_type,
		fix maxdamage, fix maxdistance, fix maxforce, cobjptridx_t parent);

// blows up a badass weapon, creating the badass explosion
// return the explosion object
void explode_badass_weapon(vobjptridx_t obj,const vms_vector &pos);

// blows up the player with a badass explosion
void explode_badass_player(vobjptridx_t obj);

void explode_object(vobjptridx_t obj,fix delay_time);
void do_explosion_sequence(vobjptr_t obj);
void do_debris_frame(vobjptridx_t obj);      // deal with debris for this frame

void draw_fireball(vobjptridx_t obj);

void explode_wall(vsegptridx_t segnum, int sidenum);
void do_exploding_wall_frame();
void maybe_drop_net_powerup(powerup_type_t powerup_type);
void maybe_replace_powerup_with_energy(vobjptr_t del_obj);
}

namespace dcx {
void init_exploding_walls();
enum class explosion_vclip_stage : int
{
	s0,
	s1,
};
}

namespace dsx {
int get_explosion_vclip(vcobjptr_t obj, explosion_vclip_stage stage);

#if defined(DXX_BUILD_DESCENT_II)
objptridx_t drop_powerup(int type, int id, int num, const vms_vector &init_vel, const vms_vector &pos, vsegptridx_t segnum);

// creates afterburner blobs behind the specified object
void drop_afterburner_blobs(vobjptr_t obj, int count, fix size_scale, fix lifetime);

/*
 * reads n expl_wall structs from a PHYSFS_File and swaps if specified
 */
void expl_wall_read_n_swap(PHYSFS_File *fp, int swap, partial_range_t<expl_wall *>);
extern fix	Flash_effect;
#endif

segidx_t pick_connected_segment(vcobjptr_t objp, int max_depth);
}
#endif

#endif
