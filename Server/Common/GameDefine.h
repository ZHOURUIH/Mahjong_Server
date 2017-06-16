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
	HT_NONE,		// û���˺�
	HT_NORMAL,		// ƽ��
	HT_QINGYISE,	// ��һɫ
};

// �齫
enum MAHJONG
{
	// 7����
	M_FENG_DONG,
	M_FENG_NAN,
	M_FENG_XI,
	M_FENG_BEI,
	M_FENG_ZHONG,
	M_FENG_FA,
	M_FENG_BAI,
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// �ṹ�嶨��
struct PengGangInfo
{
	ACTION_TYPE mType;
	MAHJONG mMahjong;
};

class CharacterPlayer;
class MahjongAction;
struct WaitActionInfo
{
	CharacterPlayer* mPlayer;				// �ȴ�ȷ�ϲ��������
	std::vector<MahjongAction*> mActionList;	// ��ҿ�ѡ��Ĳ���
	CharacterPlayer* mDroppedPlayer;			// ��������Ƶ����
	MAHJONG mMahjong;						// ��ǰ�齫
	MahjongAction* mConfirmedAction;			// ���ȷ��ѡ���һ�ֲ���
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// �������ֶ���
const int MAX_DICE = 6;						// ���ӵ����ֵ
const int MAX_PLAYER = 4;					// �����������������
const float ASSIGN_MAHJONG_INTERVAL = 0.1f;
const int MAX_SINGLE_COUNT = 4;
const int MAX_HAND_IN_COUNT = 14;	// ÿ��������������14����,��������,��,��

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// �����ַ�������
// �����齫������
const std::string MAHJONG_NAME[(int)M_MAX] = 
{
	"Feng0", "Feng1", "Feng2", "Feng3", "Feng4", "Feng5", "Feng6",
	"Tong0", "Tong1", "Tong2", "Tong3", "Tong4", "Tong5", "Tong6", "Tong7", "Tong8",
	"Tiao0", "Tiao1", "Tiao2", "Tiao3", "Tiao4", "Tiao5", "Tiao6", "Tiao7", "Tiao8",
	"Wan0", "Wan1", "Wan2", "Wan3", "Wan4", "Wan5", "Wan6", "Wan7", "Wan8"
};

#endif