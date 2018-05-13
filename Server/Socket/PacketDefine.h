#ifndef _PACKET_DEFINE_H_
#define _PACKET_DEFINE_H_

#include "ServerDefine.h"
#include "CommandDefine.h"

enum PACKET_TYPE
{
	PT_MIN,

	// CS琛ㄧずClient->Server
	PT_CS_MIN = 10000,
	PT_CS_HEART_BEAT,                       // 鍚戞湇鍔″櫒鍙戦€佺殑蹇冭烦
	PT_CS_REGISTER,                         // 鍚戞湇鍔″櫒鍙戦€佹敞鍐岃处鍙?
	PT_CS_LOGIN,                            // 鍚戞湇鍔″櫒鍙戦€佺櫥褰曡姹?
	PT_CS_CHECK_NAME,                       // 鍚戞湇鍔″櫒璇锋眰鍒ゆ柇鍚嶅瓧鏄惁宸茬粡瀛樺湪
	PT_CS_CHECK_ACCOUNT,                    // 鍚戞湇鍔″櫒璇锋眰鍒ゆ柇璐﹀彿鏄惁宸茬粡瀛樺湪
	PT_CS_CREATE_ROOM,                      // 鍚戞湇鍔″櫒璇锋眰鍒涘缓鎴块棿
	PT_CS_JOIN_ROOM,                        // 鍚戞湇鍔″櫒璇锋眰鍔犲叆鎴块棿
	PT_CS_READY,                            // 鍚戞湇鍔″櫒鍙戦€佹槸鍚﹀凡鍑嗗
	PT_CS_LEAVE_ROOM,                       // 鍚戞湇鍔″櫒璇锋眰绂诲紑鎴块棿
	PT_CS_DICE_DONE,                        // 鍚戞湇鍔″櫒鍙戦€侀瀛?
	PT_CS_REQUEST_DROP,                     // 鍚戞湇鍔″櫒璇锋眰鎵撳嚭涓€寮犵墝
	PT_CS_CONFIRM_ACTION,                   // 鍚戞湇鍔″櫒纭楹诲皢鎿嶄綔
	PT_CS_CONTINUE_GAME,                    // 鍚戞湇鍔″櫒璇锋眰缁х画娓告垙
	PT_CS_BACK_TO_MAHJONG_HALL,				// 鍚戞湇鍔″櫒璇锋眰杩斿洖澶у巺
	PT_CS_MAX,

	// SC琛ㄧずServer->Client
	PT_SC_MIN = 20000,
	PT_SC_HEART_BEAT_RET,                   // 鍚戝鎴风鍙戝洖鐨勫績璺崇粨鏋?
	PT_SC_START_GAME,                       // 鍚戝鎴风鍙戦€佺殑鍙互寮€濮嬫父鎴忕殑娑堟伅
	PT_SC_REGISTER_RET,                     // 鍚戝鎴风鍙戝洖鐨勬敞鍐岃处鍙风殑缁撴灉
	PT_SC_LOGIN_RET,                        // 鍚戝鎴风鍙戝洖鐨勭櫥褰曠粨鏋?
	PT_SC_OTHER_PLAYER_OFFLINE,             // 閫氱煡瀹㈡埛绔湁鍏朵粬鐜╁鎺夌嚎
	PT_SC_CHECK_NAME_RET,                   // 鍚戝鎴风杩斿洖鐨勬娴嬪悕瀛楃殑缁撴灉
	PT_SC_CHECK_ACCOUNT_RET,                // 鍚戝鎴风杩斿洖鐨勬娴嬭处鍙风殑缁撴灉
	PT_SC_CREATE_ROOM_RET,                  // 鍚戝鎴风杩斿洖鍒涘缓鎴块棿鐨勭粨鏋?
	PT_SC_NOTIFY_BANKER,                    // 閫氱煡瀹㈡埛绔簞瀹跺彉鍖?
	PT_SC_OTHER_PLAYER_LEAVE_ROOM,          // 閫氱煡瀹㈡埛绔湁鍏朵粬鐜╁绂诲紑鎴块棿
	PT_SC_OTHER_PLAYER_JOIN_ROOM,           // 閫氱煡瀹㈡埛绔湁鍏朵粬鐜╁鍔犲叆鎴块棿
	PT_SC_JOIN_ROOM_RET,                    // 閫氱煡瀹㈡埛绔姞鍏ユ埧闂寸殑缁撴灉
	PT_SC_READY_RET,                        // 閫氱煡瀹㈡埛绔帺瀹舵槸鍚﹀噯澶?
	PT_SC_OTHER_PLAYER_READY,               // 閫氱煡瀹㈡埛绔湁鍏朵粬鐜╁鍑嗗
	PT_SC_LEAVE_ROOM_RET,                   // 閫氱煡瀹㈡埛绔寮€鎴块棿鐨勭粨鏋?
	PT_SC_DICE_DONE_RET,                    // 閫氱煡瀹㈡埛绔幏楠板瓙瀹屾瘯,寮€濮嬫嬁鐗?
	PT_SC_NOTIFY_GET_START_MAHJONG,         // 閫氱煡瀹㈡埛绔湁鐜╁寮€灞€鎷跨墝
	PT_SC_NOTIFY_REORDER_MAHJONG,           // 閫氱煡瀹㈡埛绔噸鏂版帓鍒楅夯灏?
	PT_SC_NOTIFY_GET_START_DONE,            // 閫氱煡瀹㈡埛绔紑灞€鎷跨墝瀹屾瘯
	PT_SC_ASK_DROP,                         // 閫氱煡瀹㈡埛绔渶瑕佹墦鍑轰竴寮犵墝
	PT_SC_NOTIFY_GET_MAHJONG,               // 閫氱煡瀹㈡埛绔湁鐜╁鎽镐簡涓€寮犵墝
	PT_SC_ASK_ACTION,                       // 閫氱煡瀹㈡埛绔€夋嫨楹诲皢琛屼负
	PT_SC_OTHER_PLAYER_DROP,                // 閫氱煡瀹㈡埛绔湁鍏朵粬鐜╁鎵撳嚭涓€寮犵墝
	PT_SC_REQUEST_DROP_RET,                 // 閫氱煡瀹㈡埛绔姹傛墦鍑轰竴寮犵墝鐨勭粨鏋?
	PT_SC_PLAYER_HU,                        // 閫氱煡瀹㈡埛绔儭鐗?
	PT_SC_PLAYER_GANG,                      // 閫氱煡瀹㈡埛绔潬鐗?
	PT_SC_PLAYER_PENG,                      // 閫氱煡瀹㈡埛绔鐗?
	PT_SC_PLAYER_PASS,                      // 閫氱煡瀹㈡埛绔繃
	PT_SC_OTHER_PLAYER_GANG,                // 閫氱煡瀹㈡埛绔叾浠栫帺瀹舵潬鐗?
	PT_SC_OTHER_PLAYER_PENG,                // 閫氱煡瀹㈡埛绔叾浠栫帺瀹剁鐗?
	PT_SC_OTHER_PLAYER_PASS,                // 閫氱煡瀹㈡埛绔叾浠栫帺瀹惰繃
	PT_SC_OTHER_PLAYER_ASK_DROP,            // 閫氱煡瀹㈡埛绔瓑寰呭叾浠栫帺瀹舵墦鍑虹墝
	PT_SC_OTHER_PLAYER_ASK_ACTION,          // 閫氱煡瀹㈡埛绔瓑寰呭叾浠栫帺瀹堕€夋嫨鎿嶄綔
	PT_SC_NOTIFY_MAHJONG_END,               // 閫氱煡瀹㈡埛绔湰灞€娓告垙缁撴潫
	PT_SC_CONTINUE_GAME_RET,                // 閫氱煡瀹㈡埛绔户缁父鎴忕殑缁撴灉
	PT_SC_OTHER_PLAYER_CONTINUE_GAME,		// 閫氱煡瀹㈡埛绔湁鍏朵粬鐜╁閫夋嫨缁х画娓告垙
	PT_SC_BACK_TO_MAHJONG_HALL_RET,			// 閫氱煡瀹㈡埛绔繑鍥炲ぇ鍘呯殑缁撴灉
	PT_SC_OTHER_PLAYER_BACK_TO_MAHJONG_HALL,// 閫氱煡瀹㈡埛绔湁鍏朵粬鐜╁閫夋嫨杩斿洖澶у巺
	PT_SC_SHOW_HUA,							// 閫氱煡瀹㈡埛绔憜鍑鸿姳鐗?
	PT_SC_OTHER_PLAYER_SHOW_HUA,			// 閫氱煡瀹㈡埛绔湁鍏朵粬鐜╁鎽嗗嚭鑺辩墝
	PT_SC_MAX,

	PT_MAX,
};

#define PACKET_EMPTY DEBUG_EMPTY
#define PACKET_DEBUG(...) COMMAND_DEBUG(__VA_ARGS__)

#endif
