#ifndef _TX_SHARE_MEMORY_SERVER_H_
#define _TX_SHARE_MEMORY_SERVER_H_

#ifdef WINDOWS
#include <aclapi.h>

#pragma warning(disable:4996)

#define DEFAULT_FILENAME NULL    // Ĭ�J���ļ���
#define DEFAULT_MAPNAME  "Local//_FZD_MAP_"   // Ĭ�J�Ĺ���ȴ���
#define DEFAULT_MAPSIZE  (0xFFFF + 1) * 100  // Ĭ�J�Ĺ���ȴ��С
const DWORD NETRGUSER_CFM_CODE = 0x1211DBFF; // У�a, ��������
const DWORD NETRGUSER_CMD_NONE = 0;   // ��ʼ��ָ��a, �oָ��

#define ERROR_LEN    256    // �e�`�����L��
#define ERROR_INVALID_CMDCODE 0xE00001FF  // �ѽ�������ȫһ�ӵĹ���ȴ�
#define ERROR_NO_MAPFILE 0xE00002FF  // δ���乲��ȴ��ļ�
#define ERROR_INVALID_CFMCODE 0xE00003FF  // У�a��ƥ��

#pragma pack(1)
// ��춴惦������ăȴ��ļ���ʽ
typedef struct _tagDATA_HEADER
{
	DWORD dwConfirmCode; // У�a
	DWORD nCommandCode;  // ָ���R�e�a
	DWORD dwDataSize;  // �����Ĵ�С
	BYTE  dwReserved[19]; // ����
	BYTE  bInfo[1];   // ������ʼ��ַ
	_tagDATA_HEADER()
	{
		dwConfirmCode = NETRGUSER_CFM_CODE;
		nCommandCode = NETRGUSER_CMD_NONE;
		dwDataSize = 0;
		ZeroMemory(dwReserved, 19);
		ZeroMemory(bInfo, 1);
	}
}DATA_HEADER, *LPDATA_HEADER;
typedef DWORD(WINAPI *PSetEntriesInAcl)(unsigned long, PEXPLICIT_ACCESS, PACL, PACL*);

#pragma pack()

//////////////////////////////////////////////////////////////////////
// ��x������ȴ���ն�
class txShareMemoryServer
{
public:
	txShareMemoryServer();
	virtual ~txShareMemoryServer();
	txShareMemoryServer(char *szFileName, char *szMapName, DWORD dwSize);
	bool Create(char *szFileName = DEFAULT_FILENAME, char *szMapName = DEFAULT_MAPNAME, DWORD dwSize = DEFAULT_MAPSIZE); // �½�����ȴ�
	LPVOID GetBuffer();      // �@ȡ�ȴ��ļ�ָ�
	DWORD GetSize(){ return m_dwSize; }      // �@ȡ�ȴ��ļ���С
	void Destory();       // �N�����еĹ���ȴ�
	bool WriteCmdData(DWORD nCommandCode, DWORD dwDataSize, const LPVOID pBuf); // ���������
protected:
	void _Init();    // ��ʼ������
	bool _IsWinNTLater();  // �Дஔǰ����ϵ�y
protected:
	PSetEntriesInAcl m_fnpSetEntriesInAcl;
	HANDLE m_hFile;   // ӳ���ļ����
	HANDLE m_hFileMap;   // �ȴ��ļ����
	LPVOID m_lpFileMapBuffer; // ���n�^ָ�
	char *m_pFileName;  // ӳ���ļ���
	char *m_pMapName;  // �ȴ��ļ���
	DWORD m_dwSize;   // ���n�^��С
	bool m_bCreateFlag;  // �Ƿ��ф�������ȴ�
	DWORD   m_dwLastError;  // �e�`���a
};
#endif
#endif
