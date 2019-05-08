#ifndef __GAME_SCENE__
#define __GAME_SCENE__

#include "cocos2d.h"
#include "CustomTool.h"
#include "ui/UIButton.h"
#include "ui/UIWidget.h"
#include "ui/UIImageView.h"
#include "ui/UICheckBox.h"
#include "ui/UITextAtlas.h"
#include <array>
#include "base/ccMacros.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace ui;
using namespace std; 
using namespace cocostudio;
using namespace cocos2d;


class Box : public Sprite{
public:
	bool init(int kind = 0);
	static Box* create(int kind = 0){
		Box *pRet = new Box();
		if (pRet && pRet->init(kind)){
			pRet->autorelease();
				return pRet;
		}else{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
	void onEnter();
	void run(function<void()> backFun = nullptr);
	void stop();
	void setTargetPos(int target);
	void setPos(int pos);
	int getPos(){
		return pos;
	}
	void run(float power);
	void starHandler(float t);
	void runHandler(float t);
	void starFastHandler(float t);
	void runStop();
	void starFast();
	int step;
	int startPos;
	int targetPos;
	float runProcess;
	float process;
	float speed;
	float addSpeed;
private:
	function<void()> backFun;
	int starNum;
	bool running;
	int pos;
};

class GameScene : public Layer{
public:

	static GameScene* gamePtr;
	static bool SoundOpen;

	struct betItem{
		int id;                //id 1-8小倍率  9-16大倍率 17:goodluck1,18:goodluck2,
		int kind;              //1-9，苹果，橘子，梨，铃铛，西瓜，双星，双7，bar，goodluck
		int times;             //倍数

	};
	map <int, betItem> betList;
	static const int BET_MAX = 99;
	static const int MIN_SPACE = 24;
	static const int ROUND_COUNT = 24;
	typedef void(GameScene::*FP)();
	vector<FP> methods;
	void addMethod(FP fp);
	void removeMethod(FP fp);
	bool init();
	void onEnter();
	void requestData();
	void initGame();
	void getResult();
	void playResult();
	void clearResult();
	void getResponse(int code, string ret, string info /*= ""*/, const rapidjson::Value &data /*= NULL*/);
	static Scene* createScene();
	void followFirstTrain();
	GameScene();
	virtual ~GameScene();
	CREATE_FUNC(GameScene);
	void clearBet();
private:
	int step;      //当前一轮进行阶段 0-开始押注 1-押注完播放动画中  2-转到luck播放第二阶段动画  3-一轮结束可下注押大小或不赌直接收钱 4-押注赌大小等待结果
	int totalGold; //金币总数
	int ownGold;   //除去押注和赢取的金币自己拥有的金币数量
	int winGold;   //当前一轮压住赢的金币数量
	int betGold;   //当前一轮押注金币
	int betType;    //押注 1-赌大  2-赌小
	int borderPos; //当前光标位置
	bool newRound;
	int PressBet;  //连续押注的位置，0-是没有  1-8
	int curWinId;  //本局中奖位置
	int luckId;    //中goodluck之后得到的幸运结果 1-被吃掉了 2-小三元 3-小四喜 4-小满贯 5-大三元  6-大满贯  7-开火车
	int trainId;   //中了开火车奖励，火车头开到的位置 0-23;
	int pointNum;  //赌大小开出的点数 0-没有  1-14
	int winFlag;  //赌大小有没有中
	int betTime;
	Box* box0;
	Box* box1;
	Box* box2;
	Box* box3;
	Widget* widget;
	Sprite* lightLeft;
	Sprite* lightRight;
	TextAtlas* betLabels[8];
	Sprite* sprite; //结果动画
	TextAtlas* totalNumLabel;
	TextAtlas* winNumLabel;
	TextAtlas* pointNumLabel;
	map <int, string> imgMap;
	map <int, string> mm_imgs;
	int cur_mm;
	Box* boxArr[24];
	int prevBet[8];
	int betArr[8];
	void showGoldNum(int types = 0);
	void nextRound();
	void toStart();
	void runBox();
	void collectGold();
	void getWinId();
	void betAgain();
	//押注 1-加倍 2-减半 3赌大 4-赌小
	void betSize(int type);
	void betSizeResult();
	void toBet(int pos);
	void waitLater(float t);
	void lightUp();
	void startTrain();
	void randomPoint();
	void runPoint(float t);
	void runPoint2(float t);
	void getPointNum();
	void playSoundEffect(int id);
	void getPointResult();
	void onTouch(Ref* obj, TouchEventType type);
	void onSelected(Ref* obj, CheckBoxEventType type);
	void returnMainScene();
	void frameHandler(float t);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *pEvent);
	void addBet();
	void changeMMImg(float t);
	void betCancel();
	void dundaxiao(int type);
	void bengin();
	ParticleSystemQuad* _emitter; 
	void startGold();

	int getcoinid ;
	void closelizi(float t);
};

#endif