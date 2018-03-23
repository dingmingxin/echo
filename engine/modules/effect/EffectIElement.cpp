#include "Engine/Core.h"
#include "EffectIElement.h"

namespace Echo
{

	const String IElement::token[TOKEN_Max] = 
	{
		"effect",
		"effect_scale",
		"effect_timescale",
		"effect_lod1",
		"effect_lod2",
		"effect_fallowpair",
		"effect_2d",
		"effect_positionx",
		"effect_positiony",
		"effect_fadetime",

		"layer",
		"layer_name",
		"layer_renderlevel",
		"layer_lod",
		"layer_ispuppet",
		"layer_spacewarp",
		"layer_rendergroup",
		"layer_repeatcount",
		"layer_repeattime",
		"layer_restate",

		"layer_bb_width",
		"layer_bb_height",
		"layer_bb_perpendicular",
		"layer_bb_noscalewidth",
		"layer_bb_noscaleheight",
		"layer_bb_centrewidth",
		"layer_bb_centreheight",
		"layer_bb_offsetviewport",
		"layer_3bb_usegroundnormal",
		"layer_3bb_needupdate",
		"layer_3bb_useterrainheight",

		"layer_co_radiusbottom",
		"layer_co_radiustop",
		"layer_co_height",
		"layer_co_segment",
		"layer_co_noscalewidth",
		"layer_co_noscaleheight",
		"layer_co_keepcenter",
		"layer_co_texclamp",

		"layer_pb_param",
		"layer_pb_height",

		"layer_tr_position1",
		"layer_tr_position2",
		"layer_tr_durationtime",
		"layer_tr_fallowparent",
		"layer_tr_end_time",
		"layer_tr_anim_enable",
		"layer_tr_anim_step",
		"layer_tr_anim_begintime",
		"layer_tr_anim_animname",
		"layer_tr_anim_skelname",
		"layer_tr_anim_bonename",
		"layer_tr_anim_posoffset",
		"layer_tr_anim_orioffset",

		"layer_ln_beginpos",
		"layer_ln_endpos",
		"layer_ln_segment",
		"layer_ln_num",
		"layer_ln_wavelen",
		"layer_ln_timespin",
		"layer_ln_startwidth",
		"layer_ln_endwidth",
		"layer_ln_midderwidth",
		"layer_ln_startalpha",
		"layer_ln_endalpha",
		"layer_ln_midderalpha",
		"layer_ln_usenormal",
		"layer_ln_normal",
		"layer_ln_curvedlen",
		"layer_ln_deviation",
		"layer_ln_filter",
		"layer_ln_extenal",

		"layer_cp_area",

		"layer_sp_area",

		"layer_lp_area",

		"layer_es_rows",
		"layer_es_columns",
		"layer_es_interval",
		"layer_es_offviewport",
		"layer_es_vectors",
		"layer_es_colors",
		"layer_es_decal",

		"layer_smd_transparent",
		"layer_smd_name",
		"layer_mod_name",
		"layer_mod_skeleton",
		"layer_mod_animation",
		"layer_dmd_transparent",
		"layer_dmd_skeleton",
		"layer_dmd_skin",
		"layer_dmd_animation",
		"layer_dmd_attachmodel",
		"layer_dmd_attachbone",
		"layer_dmd_arepeatcount",

		"layer_sa_useg",
		"layer_sa_vseg",
		"layer_sa_width",
		"layer_sa_height",
		"layer_sa_speed",

		"layer_ce_name",
		"layer_ce_loop",
		"layer_ce_speed",

		"layer_pt_prewarm",
		"layer_pt_quota",
		"layer_pt_width",
		"layer_pt_height",
		"layer_pt_3dbillboard",
		"layer_pt_prependicular",
		"layer_pt_3dtrans",
		"layer_pt_notransscale",
		"layer_pt_noscalewidth",
		"layer_pt_noscaleweight",
		"layer_pt_centerwidth",
		"layer_pt_centerheight",
		"layer_pt_emitterrate",
		"layer_pt_emitterangle",
		"layer_pt_speed",
		"layer_pt_maxspeed",
		"layer_pt_addspeed",
		"layer_pt_leavetime",
		"layer_pt_maxleavetime",
		"layer_pt_mincolor",
		"layer_pt_maxcolor",
		"layer_pt_minscale",
		"layer_pt_maxscale",
		"layer_pt_min2dangle",
		"layer_pt_max2dangle",
		"layer_pt_fallowparent",
		"layer_pt_emitterinsurface",

		"layer_pt_animrownum",
		"layer_pt_animcolumnnum",
		"layer_pt_animtimegap",
		"layer_pt_animstartindex",
		"layer_pt_animendindex",
		"layer_pt_animtiled",

		"layer_pt_childeffect_name",
		"layer_pt_childeffect_loop",
		"layer_pt_childeffect_speed",

		"e_material",
		"e_material_alphamode",
		"e_material_depthmode",
		"e_material_zwritting",
		"e_material_texture",
		"e_material_uvswap",
		"e_material_flipu",
		"e_material_flipv",
		"e_material_uspeed",
		"e_material_vspeed",
		"e_material_clampmode",
		"e_material_scriptrows",
		"e_material_scriptcolumn",
		"e_material_scripttime",
		"e_material_uvvalue",
		"e_material_renderpriority",

		"e_material_uv2_enable",
		"e_material_uv2_rgb_blendmode",
		"e_material_uv2_a_blendmode",
		"e_material_texture1",
		"e_material_uvswap1",
		"e_material_flipu1",
		"e_material_flipv1",
		"e_material_uspeed1",
		"e_material_vspeed1",
		"e_material_uvvalue1",
		"e_material_dsl_enable",
		"e_material_dsl_threshold",
		"e_material_dsl_speed",

		"distortion_enable",
		"distortion_texture",
		"distortion_uvvalue",
		"distortion_alpha_threshold",
		"distortion_tc_offsetu",
		"distortion_tc_offsetv",
		"distortion_tc_sacleu",
		"distortion_saclev",
		"distortion_blend_weight",
		"distortion_type",
		"distortion_only",

		"glow_enable",

		"keyframe",
		"keyframe_position",
		"keyframe_oritation",
		"keyframe_pitch",
		"keyframe_yaw",
		"keyframe_roll",
		"keyframe_selfrotate",
		"keyframe_scale",
		"keyframe_color",
		"keyframe_begintime",
		"keyframe_affecttime",
		"keyframe_slerpmode",
		"keyframe_simplespline",
		"keyframe_autooritation",

		"keyframe_audio_enable",
		"keyframe_audio_randomsound1",
		"keyframe_audio_randomsound2",
		"keyframe_audio_volume",
		"keyframe_audio_probability",
		"keyframe_audio_loop",
		"keyframe_audio_3dmode",
		"keyframe_audio_mindistance",
		"keyframe_audio_maxdistance",

		"particlecontrollerset",

		"controller",
		"controller_begintime",
		"controller_endtime",

		"controller_ts_direction",
		"controller_ts_speed",
		"controller_ts_addspeed",

		"controller_rs_speed",
		"controller_rs_addspeed",

		"controller_ra_axispos",
		"controller_ra_axisdir",
		"controller_ra_speed",
		"controller_ra_addspeed",

		"controller_rr_axispos",
		"controller_rr_axisdir",
		"controller_rr_speed",
		"controller_rr_addspeed",

		"controller_oc_center",
		"controller_oc_speed",
		"controller_oc_addspeed",

		"controller_cc_r",
		"controller_cc_g",
		"controller_cc_b",
		"controller_cc_a",

		"controller_cn_swing",
		"controller_cn_wavelen",
		"controller_cn_basecolor",

		"controller_cs_begincolor",
		"controller_cs_setNum",
		"controller_cs_targetcolor1",
		"controller_cs_time1",
		"controller_cs_targetcolor2",
		"controller_cs_time2",
		"controller_cs_targetcolor3",
		"controller_cs_time3",
		"controller_cs_targetcolor4",
		"controller_cs_time4",

		"controller_ss_beginscale",
		"controller_ss_setNum",
		"controller_ss_targetscale1",
		"controller_ss_time1",
		"controller_ss_targetscale2",
		"controller_ss_time2",
		"controller_ss_targetscale3",
		"controller_ss_time3",
		"controller_ss_targetscale4",
		"controller_ss_time4",

		"controller_sc_scale",
		"controller_sc_minscale",
		"controller_sc_maxscale",

		"controller_sn_swing",
		"controller_sn_wavelen",

		"controller_uvss_effecttexture",
		"controller_uvss_beginuvspeed",
		"controller_uvss_setNum",
		"controller_uvss_targetuvspeed1",
		"controller_uvss_time1",
		"controller_uvss_targetuvspeed2",
		"controller_uvss_time2",
		"controller_uvss_targetuvspeed3",
		"controller_uvss_time3",
		"controller_uvss_targetuvspeed4",
		"controller_uvss_time4",
	};
}