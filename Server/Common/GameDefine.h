#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include "ServerDefine.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// ö�ٶ���

// ���뷿��Ľ��
enum JOIN_ROOM_RESULT
{
	JRR_SUCC,			// ����ɹ�
	JRR_FULL,			// ��������
	JRR_ROOM_LOCKED,	// ����������,�ܾ�����
	JRR_NO_ROOM,		// ���䲻����
	JRR_PLAYER_IN_ROOM,	// ������ڷ�����
};

// �齫��Ϊ
enum ACTION_TYPE
{
	AT_HU,
	AT_GANG,
	AT_PENG,
	AT_PASS,
	AT_MAX,
};

// ��������
enum HU_TYPE
{
	HT_NORMAL,		// ƽ��
	HT_QINGYISE,	// ��һɫ
	HT_QUESE,		// ȱһ��
	HT_HUA,			// ����
	HT_GANG,		// ����
	HT_ANGANG,		// ����
	HT_MENQING,		// ����
	HT_DUIDUIHU,	// �ԶԺ�
	HT_ANQIDUI,		// ���߶�
	HT_LONGQIDUI,	// ���߶�
	HT_GANGSHANGHUA,// ���ϻ�
	HT_GANGSHANGPAO,// ������
	HT_HAIDIHUA,	// ���׻�
	HT_HAIDIPAO,	// ������
	HT_TIANHU,		// ���
	HT_MAX,
};

// �齫
enum MAHJONG
{
	// 9��Ͳ
	M_TONG1,
	M_TONG2,
	M_TONG3,
	M_TONG4,
	M_TONG5,
	M_TONG6,
	M_TONG7,
	M_TONG8,
	M_TONG9,
	// 9����
	M_TIAO1,
	M_TIAO2,
	M_TIAO3,
	M_TIAO4,
	M_TIAO5,
	M_TIAO6,
	M_TIAO7,
	M_TIAO8,
	M_TIAO9,
	// 9����
	M_WAN1,
	M_WAN2,
	M_WAN3,
	M_WAN4,
	M_WAN5,
	M_WAN6,
	M_WAN7,
	M_WAN8,
	M_WAN9,
	// 7����
	M_FENG_DONG,
	M_FENG_NAN,
	M_FENG_XI,
	M_FENG_BEI,
	M_FENG_ZHONG,
	M_FENG_FA,
	M_FENG_BAI,
	// ����
	M_HUA_CHUN,
	M_HUA_XIA,
	M_HUA_QIU,
	M_HUA_DONG,
	M_HUA_MEI,
	M_HUA_LAN,
	M_HUA_ZHU,
	M_HUA_JU,

	M_MAX,
};

// �����齫��״̬
enum MAHJONG_PLAY_STATE
{
	MPS_WAITING,				// ���ڵȴ���ҽ����׼��
	MPS_DICE,					// ����������
	MPS_GET_START,				// ���ڿ�������
	MPS_NORMAL_GAMING,			// ���ڽ����������齫��Ϸ
	MPS_WAIT_FOR_ACTION,		// ���ڵȴ����ѡ��Ե�ǰ����Ƶ���Ϊ
	MPS_ENDING,					// �����齫����
};

// ��ɫ����
enum MAHJONG_HUASE
{
	MH_FENG,	// ����
	MH_TONG,	// Ͳ
	MH_TIAO,	// ��
	MH_WAN,		// ��
	MH_MAX,
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// �ṹ�嶨��
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
// �������ֶ���
const int MAX_DICE = 6;									// ���ӵ����ֵ
const int MAX_PLAYER = 4;								// �����������������
const float ASSIGN_MAHJONG_INTERVAL = 0.1f;
const int MAX_SINGLE_COUNT = 4;
const int MAX_HAND_IN_COUNT = 14;						// ÿ��������������14����,��������,��,��
const int MAX_HU_COUNT = 16;							// �������͵��������
const int MAX_HUA_COUNT = M_HUA_JU - M_HUA_CHUN + 1;			// ���Ƶ�����
const int MAX_FENG_COUNT = M_FENG_BAI - M_FENG_DONG + 1;// ���Ƶ�����
const int MAX_MAHJONG_COUNT = M_MAX - MAX_HUA_COUNT - MAX_FENG_COUNT;	// �齫�г��˷�ͻ�����������

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// �����ַ�������
// �����齫������
const std::string MAHJONG_NAME[(int)M_MAX] = 
{
	"Tong0", "Tong1", "Tong2", "Tong3", "Tong4", "Tong5", "Tong6", "Tong7", "Tong8",
	"Tiao0", "Tiao1", "Tiao2", "Tiao3", "Tiao4", "Tiao5", "Tiao6", "Tiao7", "Tiao8",
	"Wan0", "Wan1", "Wan2", "Wan3", "Wan4", "Wan5", "Wan6", "Wan7", "Wan8",
	"Feng0", "Feng1", "Feng2", "Feng3", "Feng4", "Feng5", "Feng6",
	"Hua0", "Hua1", "Hua2", "Hua3", "Hua4", "Hua5", "Hua6", "Hua7"
};

#endif