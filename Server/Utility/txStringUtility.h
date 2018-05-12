#ifndef _TX_STRING_UTILITY_H_
#define _TX_STRING_UTILITY_H_

#include "ServerDefine.h"

class txStringUtility
{
public:
	static std::string removeSuffix(const std::string& str);
	// ȥ�����һ������
	static void removeLastComma(std::string& stream);
	static std::string getFileName(std::string str);
	static std::string getFileNameNoSuffix(std::string str);
	static std::string getFilePath(std::string dir);
	static std::string getFileSuffix(const std::string& fileName);
	// ����ַ�����������ֵ��±�
	static int getLastNotNumberPos(const std::string& str);
	// ����ַ�����β������
	static int getLastNumber(const std::string& str);
	static void split(std::string str, const std::string& deli, txVector<std::string>& vec);
	static std::string boolToString(const bool& value) { return value ? "True" : "False"; }
	static bool stringToBool(const std::string& str) { return str == "True" || str == "true"; }
	// ��str�е�[begin,end)�滻ΪreStr
	static std::string strReplace(const std::string& str, const int& begin, const int& end, const std::string& reStr);
	// limitLen���ַ�������С����,���������λ��������С����,�����ǰ���0
	static std::string intToString(const int& i, const int& limitLen = 0); 
	static int stringToInt(const std::string& str){return atoi(str.c_str());}
	//precisionΪ����,������С����λ��,removeZeroΪ�Ƿ�ȥ��ĩβ���õ�0
	static std::string floatToString(float f, const int& precision = 4, const bool& removeZero = true);
	static float stringToFloat(const std::string& str){return (float)atof(str.c_str());}
	// �ж�oriString�Ƿ���pattern��β,sensitiveΪ�Ƿ��Сд����
	static bool endWith(const std::string& oriString, const std::string& pattern, const bool& sensitive = true);
	// �ж�oriString�Ƿ���pattern��ͷ,sensitiveΪ�Ƿ��Сд����
	static bool startWith(const std::string& oriString, const std::string& pattern, const bool& sensitive = true);
	static std::wstring ANSIToUnicode(const std::string& str);
	static std::string UnicodeToANSI(const std::wstring& str);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static std::wstring UTF8ToUnicode(const std::string& str);
	static std::string ANSIToUTF8(const std::string& str, const bool& addBOM = false);
	static std::string UTF8ToANSI(const std::string& str, const bool& removeBOM = false);
	// json
	static void jsonStartArray(std::string& str, const int& preTableCount = 0, const bool& returnLine = false);
	static void jsonEndArray(std::string& str, const int& preTableCount = 0, const bool& returnLine = false);
	static void jsonStartStruct(std::string& str, const int& preTableCount = 0, bool returnLine = false);
	static void jsonEndStruct(std::string& str, const int& preTableCount = 0, const bool& returnLine = false);
	static void jsonAddPair(std::string& str, const std::string& name, const std::string& value, const int& preTableCount = 0, const bool& returnLine = false);
	static void strToLower(std::string& str);
	static void strToUpper(std::string& str);
	static void rightToLeft(std::string& str);
	static bool findSubstr(std::string res, std::string dst, const bool& sensitive, int* pos = NULL, const int& startPose = 0, const bool& firstOrLast = true);
	static txVector<std::string> findSubstr(txVector<std::string>& res, const std::string& dst, const bool& sensitive = true);
	template<typename T>
	static txVector<std::string> findSubstr(txMap<std::string, T>& res, const std::string& dst, const bool& sensitive = true)
	{
		txVector<std::string> retList;
		// ѭ������,���ƥ�䵽�����б�
		auto itr = res.begin();
		auto itrEnd = res.end();
		FOR_STL(res, ; itr != itrEnd; ++itr)
		{
			const std::string& name = itr->first;
			if (findSubstr(name, dst, sensitive))
			{
				retList.push_back(name);
			}
		}
		END_FOR_STL(res);
		return retList;
	}
	static std::string checkString(const std::string& str, const std::string& valid);
	static std::string checkFloatString(const std::string& str, const std::string& valid = "");
	static std::string checkIntString(const std::string& str, const std::string& valid = "");
	static std::string charToHexString(const unsigned char& byte, const bool& upper = true);
	std::string charArrayToHexString(unsigned char* data, const int& dataCount, const bool& addSpace = true, const bool& upper = true);
};

#endif