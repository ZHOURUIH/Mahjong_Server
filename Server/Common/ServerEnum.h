#ifndef _SERVER_ENUM_H_
#define _SERVER_ENUM_H_

// �����������ļ���������������
enum SERVER_DEFINE_FLOAT
{
	SDF_HEART_BEAT_TIME_OUT,	// ������ʱʱ��
	SDF_SOCKET_PORT,			// socket�˿ں�
	SDF_BACK_LOG,				// ����������е���󳤶�
	SDF_SHOW_COMMAND_DEBUG_INFO,// �Ƿ���ʾ���������Ϣ
	SDF_OUTPUT_NET_LOG,			// �Ƿ���ʾ������־��Ϣ
	SDF_MAX,
};
// �����������ļ��ַ�����������
enum SERVER_DEFINE_STRING
{
	SDS_DOMAIN_NAME,			// ���ӵķ���������
	SDS_MAX,
};

#endif