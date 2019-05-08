#include "MainScene.h"
#include "Logic/GameLogic.h"
#include "LanguageConfig.h"
#include "ui/UIHelper.h"
#include "tool/Cjni.h"
#include "ClientDefine.h"
#include "GameScene.h"

bool MainScene::init(){
	if(!Scene::init()) return false;
	
	

	/*
	Sprite *spbg = Sprite::create("img/home.png");
	Rect s_visibleRect=  Director::getInstance()->getOpenGLView()->getVisibleRect();
	spbg->setPosition(Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2));
	this->addChild(spbg);

	Size size = CCDirector::sharedDirector()->getWinSize(); 
	//����Ч��
	//����
	Sprite *pokerFront = Sprite::create("img/logos.png");
	pokerFront->setVisible(false); 
	pokerFront->setPosition(ccp(size.width/2, size.height/2+50)); 
	this->addChild(pokerFront,1,123); 

	//�˿��Ʒ��� 
    CCSprite* pokerBack = CCSprite::create("img/logos.png"); 
	pokerBack->setPosition(pokerFront->getPosition()); 
	this->addChild(pokerBack,1,321);

	actionIsDone = true;//��ʾ�����Ƿ���� 



	
    ParticleFireworks *_emitter = ParticleFireworks::create();
    _emitter->retain();
    _emitter->setTexture( Director::getInstance()->getTextureCache()->addImage("img/stars.png") );
	_emitter->setStartSize(10);
	_emitter->setRadialAccel(150); 
	_emitter->setRadialAccelVar(200);  
	this->addChild(_emitter, 0);
	startFx(0);


	//����ʱ
	time_t tt;
	time(&tt);
	tm *now = localtime(&tt);

	//time_t end_tt = now->

	/*pLabel = CCLabelTTF::create(CCString::createWithFormat("%d:%d:%d", now->tm_hour, now->tm_min, now->tm_sec)->getCString(), "Thonburi", 34);
	pLabel->setPosition(ccp(size.width / 2, size.height / 2));
	addChild(pLabel,11);
	schedule(schedule_selector(MainScene::showTimebyLabel), 1.0f);*/

	Layer * s = GameScene::create();
	addChild(s,11);
	//this->setScale(-0.5);
	return true;
}

//����ʱ
void MainScene::showTimebyLabel(float t)
{
	time_t tt;
	time(&tt);
	tm *now = localtime(&tt);
	pLabel->setString(CCString::createWithFormat("%d:%d:%d",now->tm_hour,now->tm_min,now->tm_sec )->getCString());
}


void MainScene::startFx(float t)
{
	 //���� 
    CCSprite* pokerFront = (CCSprite*)this->getChildByTag(123); 
    //�˿��Ʒ��� 
    CCSprite* pokerBack = (CCSprite*)this->getChildByTag(321); 

	float orbitTime = 0.3; 
	if (pokerFront->isVisible() == false && actionIsDone == true) 
	{ 
             actionIsDone = false; 
             //��һ����������ת��ʱ�䣬�ڶ�����������ʼ�뾶�������������뾶����ĸ���������ʼZ�ǣ��������������תZ�ǲ��������������ʼX�ǣ����һ��������תX�ǲ 
             CCOrbitCamera* orbitFront = CCOrbitCamera::create(orbitTime, 1, 0, 270, 90, 0, 0); 
             CCSequence* sequenceFront = CCSequence::createWithTwoActions(CCShow::create(), orbitFront); 
             CCTargetedAction* targetFront = CCTargetedAction::create(pokerFront, sequenceFront); 
             
             CCOrbitCamera* orbitBack = CCOrbitCamera::create(orbitTime, 1, 0, 0, 90, 0, 0); 
             pokerBack->runAction(CCSequence::create(orbitBack, CCHide::create(), targetFront, CCCallFunc::create(this, callfunc_selector(MainScene::actionIsDownFunc)),NULL)); 
    }
	else if(pokerFront->isVisible() == true && actionIsDone == true){ 
		actionIsDone = false; 
        CCOrbitCamera* orbitFront = CCOrbitCamera::create(orbitTime, 1, 0, 270, 90, 0, 0); 
        CCSequence* sequenceFront = CCSequence::createWithTwoActions(CCShow::create(), orbitFront); 
        CCTargetedAction* targetFront = CCTargetedAction::create(pokerBack, sequenceFront); 
        CCOrbitCamera* orbitBack = CCOrbitCamera::create(orbitTime, 1, 0, 0, 90, 0, 0); 
        pokerFront->runAction(CCSequence::create(orbitBack, CCHide::create(), targetFront, CCCallFunc::create(this, callfunc_selector(MainScene::actionIsDownFunc)),NULL)); 
    } 
}

void MainScene::actionIsDownFunc()
{

	actionIsDone = true; 
	this->scheduleOnce(schedule_selector(MainScene::startFx), 2);
	//startFx();
}