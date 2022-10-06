#pragma once
struct RectCollider
{
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	int ID;
};
struct Cursor
{
	float x;
	float y;
	struct RectCollider RC;
	int state;
	int onHover;
	int rcID;
} cursor;
int IsRectCollide(struct RectCollider _rc1, struct RectCollider _rc2);