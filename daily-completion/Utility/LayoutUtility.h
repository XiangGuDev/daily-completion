////////////////////////////////////////////////////////////////////////////////
// Code for HPP file
#pragma once

class LayoutUtility 
{
public:
	enum LAYOUT_ANCHOR
	{
		ANCHOR_CENTER,
		ANCHOR_LEFTTOP,
		ANCHOR_RIGHTTOP,
		ANCHOR_LEFTBOTTOM,
		ANCHOR_RIGHTBOTTOM,
	};
	static const unsigned ANCHOR_LEFT = 0x01;
	static const unsigned ANCHOR_TOP = 0x02;
	static const unsigned ANCHOR_RIGHT = 0x04;
	static const unsigned ANCHOR_BOTTOM = 0x08;

	LayoutUtility();
	~LayoutUtility();
	static void SetControl(CWnd *wnd, LAYOUT_ANCHOR flag);
};

