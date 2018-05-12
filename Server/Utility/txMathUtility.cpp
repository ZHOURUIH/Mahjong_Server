#include "txMathUtility.h"
#include "txStringUtility.h"

const float txMath::MATH_PI = 3.1415926f;
const float txMath::MIN_DELTA = 0.00001f;

void txMath::checkInt(float& value, const float& precision)
{
	// 先判断是否为0
	if (isFloatZero(value, precision))
	{
		value = 0.0f;
		return;
	}
	int intValue = (int)value;
	// 大于0
	if (value > 0.0f)
	{
		// 如果原值减去整数值小于0.5f,则表示原值可能接近于整数值
		if (value - (float)intValue < 0.5f)
		{
			if (isFloatZero(value - intValue, precision))
			{
				value = (float)intValue;
			}
		}
		// 如果原值减去整数值大于0.5f, 则表示原值可能接近于整数值+1
		else
		{
			if (isFloatZero(value - (intValue + 1), precision))
			{
				value = (float)(intValue + 1);
			}
		}
	}
	// 小于0
	else if (value < 0.0f)
	{
		// 如果原值减去整数值的结果的绝对值小于0.5f,则表示原值可能接近于整数值
		if (std::abs(value - (float)intValue) < 0.5f)
		{
			if (isFloatZero(value - intValue, precision))
			{
				value = (float)intValue;
			}
		}
		else
		{
			// 如果原值减去整数值的结果的绝对值大于0.5f, 则表示原值可能接近于整数值-1
			if (isFloatZero(value - (intValue - 1), precision))
			{
				value = (float)(intValue - 1);
			}
		}
	}
}

int txMath::getGreaterPowerValue(const int& value, const int& pow)
{
	int powValue = 1;
	for (int i = 0; i < 100; ++i)
	{
		if (powValue >= value)
		{
			break;
		}
		powValue *= pow;
	}
	return powValue;
}

int txMath::getForwardInt(const float& value)
{
	if (value >= 0.0f)
	{
		int intValue = (int)(value);
		if (value - intValue > 0.0f)
		{
			return intValue + 1;
		}
		else
		{
			return (int)value;
		}
	}
	else
	{
		return (int)value;
	}
}

bool txMath::replaceKeywordAndCalculate(std::string& str, const std::string& keyword, const int& replaceValue, const bool& floatOrInt)
{
	// 如果最后的表达式中存在i,则需要把i替换为i具体的值,然后计算出最后的表达式的值
	bool replaced = false;
	int iPos;
	while ((iPos = str.find_first_of(keyword)) != -1)
	{
		replaced = true;
		str = txStringUtility::strReplace(str, iPos, iPos + keyword.length(), txStringUtility::intToString(replaceValue));
	}
	if (floatOrInt)
	{
		str = txStringUtility::floatToString(calculateFloat(str), 4);
	}
	else
	{
		str = txStringUtility::intToString(calculateInt(str));
	}
	return replaced;
}

bool txMath::replaceStringKeyword(std::string& str, const std::string& keyword, const int& keyValue, const bool& floatOrInt)
{
	bool replaced = false;
	int expressionBegin = -1;
	int expressionEnd = -1;
	// 倒序寻找
	while (txStringUtility::findSubstr(str, std::string("\\("), true, &expressionBegin, 0, false))
	{
		replaced = true;
		// 找到匹配的)
		txStringUtility::findSubstr(str, std::string(")"), true, &expressionEnd, 0, false);
		// expressionBegin + 1 去掉 /
		std::string calculateValue = str.substr(expressionBegin + 1, expressionEnd - expressionBegin + 1);
		replaceKeywordAndCalculate(calculateValue, keyword, keyValue, floatOrInt);
		// 替换掉最后一个\\()之间的内容
		str = txStringUtility::strReplace(str, expressionBegin, expressionEnd + 1, calculateValue);
	}
	return replaced;
}

float txMath::powerFloat(const float& f, int p)
{
	float ret = 1.0f;
	while (p)
	{
		ret *= f;
		--p;
	}
	return ret;
}

float txMath::calculateFloat(std::string str)
{
	// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// 判断左右括号数量是否相等
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	int strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// 计算错误,左右括号数量不对应
		return 0;
	}

	// 循环判断传入的字符串有没有括号
	while (true)
	{
		// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			float ret = calculateFloat(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			std::string floatStr = txStringUtility::floatToString(ret, 4);
			str = txStringUtility::strReplace(str, curpos, curpos + strInBracket.length() + 2, floatStr);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = txStringUtility::strReplace(str, i, i + 1, EMPTY_STRING);
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	txVector<float> numbers;
	txVector<char> factors;
	// 表示上一个运算符的下标+1
	int beginpos = 0;
	strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
		if (i == strLen - 1)
		{
			std::string num = str.substr(beginpos, strLen - beginpos);
			float fNum = txStringUtility::stringToFloat(num);
			numbers.push_back(fNum);
			break;
		}
		// 找到第一个运算符
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i - beginpos);
				float fNum = txStringUtility::stringToFloat(num);
				numbers.push_back(fNum);
			}
			// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// 计算错误,运算符与数字数量不符
		return 0;
	}
	// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
	while (true)
	{
		// 表示是否还有乘除表达式
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// 先遍历到哪个就先计算哪个
			if (factors[i] == '*' || factors[i] == '/')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				float num1 = numbers[i];
				float num2 = numbers[i + 1];
				float num3 = 0.0f;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				// 删除第i + 1个数,然后将第i个数替换为计算结果
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[i] = num3;
				// 删除第i个运算符
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// 再计算加减法
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// 第一个运算数的下标与运算符的下标是相同的
			float num1 = numbers[0];
			float num2 = numbers[1];
			float num3 = 0.0f;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// 删除第1个数,然后将第0个数替换为计算结果
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// 计算错误
				return 0;
			}
			numbers[0] = num3;
			// 删除第0个运算符
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// 计算错误
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

int txMath::calculateInt(std::string str)
{
	// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '%'
			&& str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// 判断左右括号数量是否相等
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	int strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// 计算错误,左右括号数量不对应
		return 0;
	}

	// 循环判断传入的字符串有没有括号
	while (true)
	{
		// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			int ret = calculateInt(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			std::string intStr = txStringUtility::intToString(ret, 4);
			str = txStringUtility::strReplace(str, curpos, curpos + strInBracket.length() + 2, intStr);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = txStringUtility::strReplace(str, i, i + 1, EMPTY_STRING);
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	txVector<int> numbers;
	txVector<char> factors;
	// 表示上一个运算符的下标+1
	int beginpos = 0;
	strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
		if (i == strLen - 1)
		{
			std::string num = str.substr(beginpos, strLen - beginpos);
			int iNum = txStringUtility::stringToInt(num);
			numbers.push_back(iNum);
			break;
		}
		// 找到第一个运算符
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i - beginpos);
				int iNum = txStringUtility::stringToInt(num);
				numbers.push_back(iNum);
			}
			// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// 计算错误,运算符与数字数量不符
		return 0;
	}
	// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
	while (true)
	{
		// 表示是否还有乘除表达式
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// 先遍历到哪个就先计算哪个
			if (factors[i] == '*' || factors[i] == '/' || factors[i] == '%')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				int num1 = numbers[i];
				int num2 = numbers[i + 1];
				int num3 = 0;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				else if (factors[i] == '%')
				{
					num3 = num1 % num2;
				}
				// 删除第i + 1个数,然后将第i个数替换为计算结果
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[i] = num3;
				// 删除第i个运算符
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// 再计算加减法
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// 第一个运算数的下标与运算符的下标是相同的
			int num1 = numbers[0];
			int num2 = numbers[1];
			int num3 = 0;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// 删除第1个数,然后将第0个数替换为计算结果
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// 计算错误
				return 0;
			}
			numbers[0] = num3;
			// 删除第0个运算符
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// 计算错误
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

// 秒数转换为分数和秒数
void txMath::secondsToMinutesSeconds(int seconds, int& outMin, int& outSec)
{
	outMin = seconds / 60;
	outSec = seconds - outMin * 60;
}

void txMath::secondsToHoursMinutesSeconds(int seconds, int& outHour, int& outMin, int& outSec)
{
	outHour = seconds / (60 * 60);
	outMin = (seconds - outHour * (60 * 60)) / 60;
	outSec = seconds - outHour * (60 * 60) - outMin * 60;
}

float txMath::HueToRGB(float v1, float v2, float vH)
{
	if (vH < 0.0f)
	{
		vH += 1.0;
	}
	if (vH > 1.0f)
	{
		vH -= 1.0f;
	}
	if (6.0f * vH < 1.0f)
	{
		return v1 + (v2 - v1) * 6.0f * vH;
	}
	else if (2.0f * vH < 1.0f)
	{
		return v2;
	}
	else if (3.0f * vH < 2.0f)
	{
		return v1 + (v2 - v1) * (2.0f / 3.0f - vH) * 6.0f;
	}
	else
	{
		return v1;
	}
}