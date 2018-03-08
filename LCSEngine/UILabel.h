#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "ElementGameUI.h"

class UILabel : public ElementGameUI
{
public:
	UILabel(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	~UILabel();

};

#endif //__UILABEL_H__