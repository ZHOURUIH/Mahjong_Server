/******************************************************************
++  File Name :   FFMClass.h
++  Description: 共享內存類
---------------------------------------------------------------
++  Author:  Fei ZhaoDong
++  Create time: 2004/3/25 上午 10:00:00
++  Version:     1.1
++  Modifier:
++  Activities:
++  Update List: 2004/3/30 下午 02:59:45
*******************************************************************/
// FFMClass.h: interface for the CFFMClass.
//
//////////////////////////////////////////////////////////////////////
#ifndef FZD_FFMCLASS_H
#define FZD_FFMCLASS_H

#include "ServerDefine.h"
#if RUN_PLATFORM == PLATFORM_WINDOWS
//#include <Winbase.h>
#include <aclapi.h>

#pragma warning(disable:4996)

//////////////////////////////////////////////////////////////////////
// Macro definition
// 以下為共享內存定義
#define DEFAULT_FILENAME NULL    // 默認的文件名
#define DEFAULT_MAPNAME  "Local//_FZD_MAP_"   // 默認的共享內存名
#define DEFAULT_MAPSIZE  (0xFFFF + 1) * 100  // 默認的共享內存大小
const DWORD NETRGUSER_CFM_CODE = 0x1211DBFF; // 校驗碼, 用於命令數據
const DWORD NETRGUSER_CMD_NONE = 0;   // 初始化指令碼, 無指令
// 以下為錯誤碼定義
#define ERROR_LEN    256    // 錯誤描述長度
#define ERROR_INVALID_CMDCODE 0xE00001FF  // 已經存在完全一樣的共享內存
#define ERROR_NO_MAPFILE 0xE00002FF  // 未分配共享內存文件
#define ERROR_INVALID_CFMCODE 0xE00003FF  // 校驗碼不匹配
//////////////////////////////////////////////////////////////////////
// 內存文件格式定義
#pragma pack(1)
// 用於存儲命令數據的內存文件格式
typedef struct _tagDATA_HEADER
{
	DWORD dwConfirmCode; // 校驗碼
	DWORD nCommandCode;  // 指令識別碼
	DWORD dwDataSize;  // 數據的大小
	BYTE  dwReserved[19]; // 保留
	BYTE  bInfo[1];   // 數據起始地址
	_tagDATA_HEADER()
	{
		dwConfirmCode = NETRGUSER_CFM_CODE;
		nCommandCode = NETRGUSER_CMD_NONE;
		dwDataSize = 0;
		ZeroMemory(dwReserved, 19);
		ZeroMemory(bInfo, 1);
	}
}DATA_HEADER, *LPDATA_HEADER;
typedef DWORD(WINAPI *PSetEntriesInAcl)(ULONG, PEXPLICIT_ACCESS, PACL, PACL*);
// 用於存儲應答數據的共享內存文件格式 (暫不用)
typedef struct _tagANS_HEADER
{
}ANS_HEADER, *LPANS_HEADER;

#pragma pack()

//////////////////////////////////////////////////////////////////////
// 類定義，共享內存服務端
class CFFMServer
{
public:
	CFFMServer();
	virtual ~CFFMServer();
	CFFMServer(char *szFileName, char *szMapName, DWORD dwSize);
	BOOL Create(char *szFileName = DEFAULT_FILENAME, char *szMapName = DEFAULT_MAPNAME, DWORD dwSize = DEFAULT_MAPSIZE); // 新建共享內存
	LPVOID GetBuffer();      // 獲取內存文件指針
	DWORD GetSize();      // 獲取內存文件大小
	void Destory();       // 銷毀已有的共享內存
	BOOL WriteCmdData(DWORD nCommandCode, DWORD dwDataSize, const LPVOID pBuf); // 寫入命令數據
protected:
	void _Init();    // 初始化參數
	BOOL _IsWinNTLater();  // 判斷當前操作系統
protected:
	PSetEntriesInAcl m_fnpSetEntriesInAcl;
	HANDLE m_hFile;   // 映射文件句柄
	HANDLE m_hFileMap;   // 內存文件句柄
	LPVOID m_lpFileMapBuffer; // 緩衝區指針
	char *m_pFileName;  // 映射文件名
	char *m_pMapName;  // 內存文件名
	DWORD m_dwSize;   // 緩衝區大小
	BOOL m_bCreateFlag;  // 是否已創建共享內存
	DWORD   m_dwLastError;  // 錯誤代碼
};
#endif
#endif
