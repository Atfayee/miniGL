#include "DrawState.h"
#include "Graphic.h"

State g_State;

State::State()
{
	DrawMode = dmPixel;
	g_OperationType = otNone;
	GridWidth = GridHeight = 10;
	FillMethod = InternalPointsSeedFill;
	symmetryMethod = symmetryX;
	ClipMethod = CohenSutherland;
	g_3DMode = None;
	g_3DState = NoneState;
}