#ifndef _VECTOR2_H_
#define _VECTOR2_H_

class Vector2
{
public:
	float x;
	float y;
public:
	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}
	Vector2(float xx, float yy)
	{
		x = xx;
		y = yy;
	}
	Vector2 operator+(const Vector2& that)
	{
		return Vector2(x + that.x, y + that.y);
	}
	Vector2 operator-(const Vector2& that)
	{
		return Vector2(x - that.x, y - that.y);
	}
	Vector2& operator+=(const Vector2& that)
	{
		x -= that.x;
		y -= that.y;
		return *this;
	}
	Vector2& operator-=(const Vector2& that)
	{
		x -= that.x;
		y -= that.y;
		return *this;
	}
};

#endif