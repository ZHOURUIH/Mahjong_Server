﻿#ifndef _DATA_DEFINE_H_
#define _DATA_DEFINE_H_

// 数据类型
enum DATA_TYPE
{
	DT_MIN,
#if (defined _CLIENT)
	DT_CHARACTER_STATE,
	DT_EFFECT,
	DT_IMPACT,
	DT_CHARACTER_SOUND,
	DT_UI_SOUND,
	DT_EFFECT_HOOK,
	DT_JUMP_POINT,
	DT_SCENE_LOCATE_DEFINE,
	DT_FIXED_BROKEN_ELEMENT,
	DT_ROLE_ATTACK_KEYFRAME,
	DT_SOUND,
	DT_CHARACTER_RUN_SOUND,
#elif (defined _SERVER)
	DT_PLAYER_INFO,
#endif
	DT_MAX,
};

#endif