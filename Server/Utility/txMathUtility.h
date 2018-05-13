#ifndef _TX_MATH_UTILITY_H_
#define _TX_MATH_UTILITY_H_

#include "ServerDefine.h"

class txMath
{
public:
	static const float MATH_PI;
	static const float MIN_DELTA;
	static void checkInt(float& value, const float& precision = MIN_DELTA); // 判断传入的参数是否已经接近于整数,如果接近于整数,则设置为整数
	static bool isFloatZero(const float& value, const float& precision = MIN_DELTA){return value >= -precision && value <= precision;}
	static bool isFloatEqual(const float& value1, const float& value2, const float& precision = MIN_DELTA){return isFloatZero(value1 - value2, precision);}
	// 得到比value大的第一个pow的n次方的数
	static int getGreaterPowerValue(const int& value, const int& pow);
	// 得到数轴上浮点数右边的第一个整数,向上取整
	static int getForwardInt(const float& value);
	template<typename T>
	static void clamp(T& value, const T& minValue, const T& maxValue)
	{
		if (value > maxValue)
		{
			value = maxValue;
		}
		else if (value < minValue)
		{
			value = minValue;
		}
	}
	template<typename T>
	static void clampMin(T& value, const T& minValue)
	{
		if (value < minValue)
		{
			value = minValue;
		}
	}
	template<typename T>
	static void clampMax(T& value, const T& maxValue)
	{
		if (value > maxValue)
		{
			value = maxValue;
		}
	}
	static float randomFloat(const float& minFloat, const float& maxFloat)
	{
		float percent = (rand() % (1000 + 1)) / 1000.0f;
		return percent * (maxFloat - minFloat) + minFloat;
	}
	static int randomInt(const int& minInt, const int& maxInt)
	{
		if (minInt >= maxInt)
		{
			return minInt;
		}
		return rand() % (maxInt - minInt + 1) + minInt;
	}
	static float angleToRadian(const float& angle)
	{
		return angle * 3.14159f / 180.0f;
	}
	static float radianToAngle(const float& radian)
	{
		return radian * 180.0f / 3.14159f;
	}
	static void clampValue(float& value, const float& min, const float& max, const float& cycle)
	{
		while (value < min)
		{
			value += cycle;
		}
		while (value > max)
		{
			value -= cycle;
		}
	}
	static void clampAngle(float& angle, const float& min, const float& max, const float& pi)
	{
		clampValue(angle, min, max, pi * 2.0f);
	}
	static void adjustRadian180(float& radianAngle)
	{
		clampAngle(radianAngle, -MATH_PI, MATH_PI, MATH_PI);
	}
	static void adjustAngle180(float& radianAngle)
	{
		clampAngle(radianAngle, -180.0f, 180.0f, 180.0f);
	}
	static void adjustRadian360(float& radianAngle)
	{
		clampAngle(radianAngle, 0.0f, MATH_PI * 2.0f, MATH_PI);
	}
	static void adjustAngle360(float& radianAngle)
	{
		clampAngle(radianAngle, 0.0f, 360.0f, 180.0f);
	}
	static bool isInRange(const int& value, const int& range0, const int& range1)
	{
		return value >= getMin(range0, range1) && value <= getMax(range0, range1);
	}
	static bool isInRange(const float& value, const float& range0, const float& range1)
	{
		return value >= getMin(range0, range1) && value <= getMax(range0, range1);
	}
	template<typename T>
	static T getMin(const T& a, const T& b)
	{
		return a < b ? a : b;
	}
	template<typename T>
	static T getMax(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	template<typename T>
	static float lerp(const T& start, const T& end, const float& t)
	{
		return start + (end - start) * t;
	}
	static float inverseLerp(const float& a, const float& b, const float& value)
	{
		return (value - a) / (b - a);
	}
	// 将表达式str中的keyword替换为replaceValue,然后计算str的值,返回值表示str中是否有被替换的值,str只能是算术表达式
	static bool replaceKeywordAndCalculate(std::string& str, const std::string& keyword, const int& replaceValue, const bool& floatOrInt);
	// 将表达式str中的所有\\()包含的部分中的keyword替换为keyValue,并且计算包含的表达式,返回值表示str中是否有被替换的部分,str可以是任意表达式
	static bool replaceStringKeyword(std::string& str, const std::string& keyword, const int& keyValue, const bool& floatOrInt);
	static float powerFloat(const float& f, int p);
	static float calculateFloat(std::string str);	// 以浮点数的计算法则计算一个表达式,只支持加减乘除和括号
	static int calculateInt(std::string str);		// 以整数的计算法则计算一个表达式,支持取余,加减乘除和括号
	// 秒数转换为分数和秒数
	static void secondsToMinutesSeconds(int seconds, int& outMin, int& outSec);
	static void secondsToHoursMinutesSeconds(int seconds, int& outHour, int& outMin, int& outSec);
	template<typename T>
	static void swap(T& value0, T& value1)
	{
		T temp = value0;
		value0 = value1;
		value1 = temp;
	}
	template<typename T>
	static T getSign(const T& value)
	{
		if (value > (T)0)
		{
			return (T)1;
		}
		else if (value < (T)0)
		{
			return (T)-1;
		}
		else
		{
			return (T)0;
		}
	}
	float HueToRGB(float v1, float v2, float vH);
};

#endif