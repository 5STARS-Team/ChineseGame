#include "TimeLoading.h"
#include "ui\UIImageView.h"
using namespace cocos2d::ui;




static TimeLoadingLayer* _TimeLoadingLayer = NULL;

TimeLoadingLayer::TimeLoadingLayer( void )
{
    flag = 0;
}

TimeLoadingLayer::~TimeLoadingLayer( void )
{
    this->unschedule(schedule_selector(TimeLoadingLayer::updateD));
    _TimeLoadingLayer=NULL;
}

TimeLoadingLayer* TimeLoadingLayer::createLayer()
{
    if(_TimeLoadingLayer == NULL){
       _TimeLoadingLayer = TimeLoadingLayer::create();
    }
    return _TimeLoadingLayer;
}

bool TimeLoadingLayer::init()
{
	LayerColor::init();
    Director::getInstance()->getRunningScene()->addChild(this,999999);
    this->schedule(schedule_selector(TimeLoadingLayer::updateD),0.05f);
    return true;
}

void TimeLoadingLayer::onEnter()
{
	LayerColor::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(TimeLoadingLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TimeLoadingLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TimeLoadingLayer::onTouchEnded, this);

    this->setColor(Color3B::BLACK);
    this->setOpacity(100);

    ImageView* img = ImageView::create();
    img->loadTexture("img/waiting.png");    
    this->addChild(img);

    Size size  = Director::getInstance()->getWinSize();     
    img->setPosition(Vec2(size.width/2,size.height/2));

    img->runAction(RepeatForever::create(RotateBy::create(0.5f,360)));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->initWithColor(Color4B(0,0,0,128));
    this->setOpacity(0);
    this->runAction(CCFadeTo::create(0.3f,180));

}

bool TimeLoadingLayer::onTouchBegan( Touch *pTouch, Event *pEvent )
{
    return true;
}

void TimeLoadingLayer::onTouchMoved( Touch *pTouch, Event *pEvent )
{

}

void TimeLoadingLayer::onTouchEnded( Touch *pTouch, Event *pEvent )
{

}

void TimeLoadingLayer::StartWaiting()
{
    TimeLoadingLayer::createLayer();
}

void TimeLoadingLayer::EndWaiting()
{
    if(_TimeLoadingLayer){
        _TimeLoadingLayer->flag = 1;        
    }    
}

void TimeLoadingLayer::updateD( float delta )
{
    if(flag == 1){
        flag = 0;
        this->removeFromParent();
    }
}
