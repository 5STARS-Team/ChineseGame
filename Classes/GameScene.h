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
		int id;                //id 1-8С����  9-16���� 17:goodluck1,18:goodluck2,
		int kind;              //1-9��ƻ�������ӣ��棬���������ϣ�˫�ǣ�˫7��bar��goodluck
		int times;             //����

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
	int step;      //��ǰһ�ֽ��н׶� 0-��ʼѺע 1-Ѻע�겥�Ŷ�����  2-ת��luck���ŵڶ��׶ζ���  3-һ�ֽ�������עѺ��С�򲻶�ֱ����Ǯ 4-Ѻע�Ĵ�С�ȴ����
	int totalGold; //�������
	int ownGold;   //��ȥѺע��Ӯȡ�Ľ���Լ�ӵ�еĽ������
	int winGold;   //��ǰһ��ѹסӮ�Ľ������
	int betGold;   //��ǰһ��Ѻע���
	int betType;    //Ѻע 1-�Ĵ�  2-��С
	int borderPos; //��ǰ���λ��
	bool newRound;
	int PressBet;  //����Ѻע��λ�ã�0-��û��  1-8
	int curWinId;  //�����н�λ��
	int luckId;    //��goodluck֮��õ������˽�� 1-���Ե��� 2-С��Ԫ 3-С��ϲ 4-С���� 5-����Ԫ  6-������  7-����
	int trainId;   //���˿��𳵽�������ͷ������λ�� 0-23;
	int pointNum;  //�Ĵ�С�����ĵ��� 0-û��  1-14
	int winFlag;  //�Ĵ�С��û����
	int betTime;
	Box* box0;
	Box* box1;
	Box* box2;
	Box* box3;
	Widget* widget;
	Sprite* lightLeft;
	Sprite* lightRight;
	TextAtlas* betLabels[8];
	Sprite* sprite; //�������
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
	//Ѻע 1-�ӱ� 2-���� 3�Ĵ� 4-��С
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