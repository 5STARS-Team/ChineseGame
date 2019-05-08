#ifndef __MainScene__
#define __MainScene__
#include "cocos2d.h"
#include "GameScene.h"
#include "CustomTool.h"
#include "cocos/ui/UIButton.h"
#include "ui/UIWidget.h"

USING_NS_CC;
using namespace ui;


class MainScene : public Scene{
public:
	bool init();
	CREATE_FUNC(MainScene);


	void startFx(float t);
	void actionIsDownFunc();
	void showTimebyLabel(float t);
	
protected:
	bool actionIsDone;
	LabelTTF *pLabel;
};

#endif