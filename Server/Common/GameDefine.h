#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include "ServerDefine.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// 枚举定义

// 加入房间的结果
enum JOIN_ROOM_RESULT
{
	JRR_SUCC,			// 加入成功
	JRR_FULL,			// 房间已满
	JRR_ROOM_LOCKED,	// 房间已锁定,拒绝加入
	JRR_NO_ROOM,		// 房间不存在
	JRR_PLAYER_IN_ROOM,	// 玩家已在房间中
};

// 麻将行为
enum ACTION_TYPE
{
	AT_HU,
	AT_GANG,
	AT_PENG,
	AT_PASS,
	AT_MAX,
};

// 胡牌类型
enum HU_TYPE
{
	HT_NORMAL,		// 平胡
	HT_QINGYISE,	// 清一色
	HT_QUESE,		// 缺一门
	HT_HUA,			// 花牌
	HT_GANG,		// 杠牌
	HT_ANGANG,		// 暗杠
	HT_MENQING,		// 门清
	HT_DUIDUIHU,	// 对对胡
	HT_ANQIDUI,		// 暗七对
	HT_LONGQIDUI,	// 龙七对
	HT_GANGSHANGHUA,// 杠上花
	HT_GANGSHANGPAO,// 杠上炮
	HT_HAIDIHUA,	// 海底花
	HT_HAIDIPAO,	// 海底炮
	HT_TIANHU,		// 天胡
	HT_MAX,
};

// 麻将
enum MAHJONG
{
	// 7个风
	M_FENG_DONG,
	M_FENG_NAN,
	M_FENG_XI,
	M_FENG_BEI,
	M_FENG_ZHONG,
	M_FENG_FA,
	M_FENG_BAI,
	// 9个筒
	M_TONG1,
	M_TONG2,
	M_TONG3,
	M_TONG4,
	M_TONG5,
	M_TONG6,
	M_TONG7,
	M_TONG8,
	M_TONG9,
	// 9个条
	M_TIAO1,
	M_TIAO2,
	M_TIAO3,
	M_TIAO4,
	M_TIAO5,
	M_TIAO6,
	M_TIAO7,
	M_TIAO8,
	M_TIAO9,
	// 9个万
	M_WAN1,
	M_WAN2,
	M_WAN3,
	M_WAN4,
	M_WAN5,
	M_WAN6,
	M_WAN7,
	M_WAN8,
	M_WAN9,
	// 花牌
	M_CHUN,
	M_XIA,
	M_QIU,
	M_DONG,
	M_MEI,
	M_LAN,
	M_ZHU,
	M_JU,

	M_MAX,
};

// 本局麻将的状态
enum MAHJONG_PLAY_STATE
{
	MPS_WAITING,				// 正在等待玩家进入或准备
	MPS_DICE,					// 正在掷骰子
	MPS_GET_START,				// 正在开局拿牌
	MPS_NORMAL_GAMING,			// 正在进行正常的麻将游戏
	MPS_WAIT_FOR_ACTION,		// 正在等待玩家选择对当前打出牌的行为
	MPS_ENDING,					// 本局麻将结束
};

// 花色类型
enum MAHJONG_HUASE
{
	MH_FENG,	// 风牌
	MH_TONG,	// 筒
	MH_TIAO,	// 条
	MH_WAN,		// 万
	MH_MAX,
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// 结构体定义
struct PengGangInfo
{
	ACTION_TYPE mType;
	MAHJONG mMahjong;
};

struct MahInfo
{
	MAHJONG mMah;
	int mCount;
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// 常量数字定义
const int MAX_DICE = 6;						// 骰子的最大值
const int MAX_PLAYER = 4;					// 房间中最大的玩家数量
const float ASSIGN_MAHJONG_INTERVAL = 0.1f;
const int MAX_SINGLE_COUNT = 4;
const int MAX_HAND_IN_COUNT = 14;	// 每名玩家手里最多有14张牌,不包含碰,吃,杠
const int MAX_HU_COUNT = 16;		// 胡牌类型的最大数量

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// 常量字符串定义
// 所有麻将的名字
const std::string MAHJONG_NAME[(int)M_MAX] = 
{
	"Feng0", "Feng1", "Feng2", "Feng3", "Feng4", "Feng5", "Feng6",
	"Tong0", "Tong1", "Tong2", "Tong3", "Tong4", "Tong5", "Tong6", "Tong7", "Tong8",
	"Tiao0", "Tiao1", "Tiao2", "Tiao3", "Tiao4", "Tiao5", "Tiao6", "Tiao7", "Tiao8",
	"Wan0", "Wan1", "Wan2", "Wan3", "Wan4", "Wan5", "Wan6", "Wan7", "Wan8"
};

#endif