#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "ElementGameUI.h"

class UIImage : public ElementGameUI
{
public:
	UIImage(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	~UIImage();
	void drawGUI() override;

};

#endif //__UIIMAGE_H__