#include "collision.h"
#include "gamestate_rps.h"
//enum CickStates { IDLE = 0, CLICKED } CursorStateEnum;

int IsRectCollide(struct RectCollider _rc1, struct RectCollider _rc2)
{
	if (_rc1.xMin >= _rc2.xMin && _rc1.xMin <= _rc2.xMax //_rc1 top left
		&& _rc1.yMin >= _rc2.yMin && _rc1.yMin <= _rc2.yMax //^^^^^^^^^^^^
		|| _rc1.xMax >= _rc2.xMin && _rc1.xMax <= _rc2.xMax //_rc1 bottom right
		&& _rc1.yMax >= _rc2.yMin && _rc1.yMax <= _rc2.yMax //^^^^^^^^^^^^^^^^^
		|| _rc1.xMax >= _rc2.xMin && _rc1.xMax <= _rc2.xMax //_rc1 top right
		&& _rc1.yMin >= _rc2.yMin && _rc1.yMin <= _rc2.yMax //^^^^^^^^^^^^^^
		|| _rc1.xMin >= _rc2.xMin && _rc1.xMin <= _rc2.xMax //_rc1 bottom left
		&& _rc1.yMax >= _rc2.yMin && _rc1.yMax <= _rc2.yMax) //^^^^^^^^^^^^^^^
	{
		cursor.rcID = _rc2.ID;
		if (cursor.onHover != 1 && cursor.rcID == _rc2.ID)
		{
			cursor.onHover = 1;
			CP_Sound_Play(onHover);
		}
		return 1; //Collision has occured
	}
	if (cursor.rcID == _rc2.ID && cursor.onHover == 1 )
	{
		cursor.onHover = 0;
	}
	return 0; //No collision
}