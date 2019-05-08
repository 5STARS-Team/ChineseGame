#pragma once
#include "cocos2d.h"


using namespace std;
USING_NS_CC;

class TimeLoadingLayer : public  LayerColor
{
public:
	TimeLoadingLayer(void);
	~TimeLoadingLayer(void);
    static TimeLoadingLayer* createLayer();
    bool init();
    void onEnter();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void updateD( float delta );
    static void StartWaiting();
    static void EndWaiting();
    CREATE_FUNC(TimeLoadingLayer);
    
public:
    int flag;
};

