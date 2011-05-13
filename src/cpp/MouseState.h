#pragma once

#include "Defines.h"
#include "ButtonState.h"

enum MouseButtons
{
	LeftButton = 0,
	RightButton = 1,
	CenterButton = 2,
	Mouse4 = 3,
	Mouse5 = 4,
};

class MouseState
{
private:
	int _x, _y, _dx, _dy;
	ButtonState _buttonStates[5];

	static MouseState _prevState;

public:
	MouseState();
	~MouseState();

	static void SetCursorPosition(int x, int y);
	static void SetCursorVisible(bool visible);

	int GetX();
	int GetY();
	int GetDX();
	int GetDY();

	ButtonState GetButtonState(MouseButtons button);
    bool IsButtonDown(MouseButtons button);
	bool IsButtonUp(MouseButtons button);
    bool IsButtonJustPressed(MouseButtons button);
	bool IsButtonJustReleased(MouseButtons button);
	
	static MouseState GetState();
};