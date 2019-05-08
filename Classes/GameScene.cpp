#include "GameScene.h"
#include <math.h>
#include "Logic/GameLogic.h"
#include "ClientDefine.h"
#include "tool/libuitl.h"
#include "tool/Cjni.h"
#include "MainScene.h"
#include "LanguageConfig.h"

using namespace rapidjson;
using namespace JNI;

bool Box::init(int kind){
	if (!Sprite::init()){
		return false;
	}
	string fileName = "img/box" + cocos2d::Value(kind).asString() + ".png";
	this->setTexture(fileName);
	this->setScale(0.85);
	return true;
}

void Box::onEnter(){
	Sprite::onEnter();
	//stop();
}

void Box::setTargetPos(int target){
	targetPos = target;
	while (targetPos - pos < GameScene::MIN_SPACE * 2){
		targetPos += GameScene::MIN_SPACE;
	}
}

void Box::run(function<void()> backFun){
	startPos = pos;
	process = 0;
	speed = 1;
	addSpeed = 0;
	targetPos = -1;
	step = 0;
	this->setVisible(true);
	this->unschedule(schedule_selector(Box::starHandler));
	this->schedule(schedule_selector(Box::runHandler), 0.03);
	this->backFun = backFun;
}

void Box::starFast(){
	starNum = 0;
	this->unschedule(schedule_selector(Box::starHandler));
	this->schedule(schedule_selector(Box::starFastHandler), 0.1);
}

void Box::stop(){
	step = 4;
	starNum = 0;
	running = false;
	this->unschedule(schedule_selector(Box::runHandler));
	this->schedule(schedule_selector(Box::starHandler), 0.5);
}


//旋转
void Box::runHandler(float t){
	if (step == 0){
		process += 0.33;
		if (process >= 5){
			step = 1;
		}
		if (pos != startPos + (int)process && !backFun){
			CustomTool::playSoundEffect("sounds/s" + cocos2d::Value((9 - (int)process) % 10).asString() + ".ogg");
		}
	} else if(step == 1){
		process += speed;
		if (process > GameScene::ROUND_COUNT && targetPos > -1){
			startPos = pos;
			process = 0;
			speed = 1;
			runProcess = targetPos - pos;
			addSpeed = - speed * speed * 0.5 / runProcess;
			step = 2;
		}
	} else if (step == 2){
		float prev_speed = speed;
		speed += addSpeed;
		process += (speed + prev_speed) * 0.5;
		if (process >= runProcess || speed < 0.01){
			speed = 0;
			process = runProcess;
			this->unschedule(schedule_selector(Box::runHandler));
			runStop();
		}
	}
	int p = (startPos + (int)process) % GameScene::ROUND_COUNT;
	if (pos != p && step > 0 && !backFun){
		CustomTool::playSoundEffect("sounds/run.ogg");// +cocos2d::Value(((int)process) % 10).asString() + ".ogg");
	}
	this->setPos(p);
	if (backFun){
		backFun();
	}
}

void Box::runStop(){
	GameScene::gamePtr->getResult();
}

void Box::starHandler(float t){
	if (running){
		this->setVisible(true);
		return;
	}
	starNum++;
	this->setVisible(starNum % 2);
}

void Box::starFastHandler(float t){
	starNum++;
	this->setVisible(starNum % 2);
	if (starNum > 20){
		this->setVisible(true);
		this->unschedule(schedule_selector(Box::starFastHandler));
		GameScene::gamePtr->playResult();
	}
}

void Box::setPos(int pos){
	if (pos < 0){
		pos += GameScene::ROUND_COUNT;
	} else if (pos >= GameScene::ROUND_COUNT){
		pos %= GameScene::ROUND_COUNT;
	}
	this->pos = pos;
	Vec2 v;
	if (pos < 7){
		v.x = 68 + pos * 57;
		v.y = 662;
	} else if (pos < 13){
		v.x = 410;
		v.y = 662 - (pos - 6) * 57;
	} else if(pos < 19){
		v.x = 410 - (pos - 12) * 57;
		v.y = 318;
	} else{
		v.x = 68;
		v.y = 318 + (pos - 18) * 57;
	}
	this->setPosition(v);
}

GameScene* GameScene::gamePtr = nullptr;
bool GameScene::SoundOpen = true;

GameScene::GameScene(){
	GameScene::gamePtr = this;
	getcoinid = 0;
}


GameScene::~GameScene(){
	GameScene::gamePtr = nullptr;
}

Scene* GameScene::createScene(){
	auto scene = Scene::create();
	auto game = GameScene::create();
	scene->addChild(game);
	return scene;
}


bool GameScene::init(){
	if (!Layer::init()){
		return false;
	}

	//初始化游戏逻辑
	GameLogic::GetInstance()->init();

	widget = CustomTool::getCSUI("gameScene", this);
	Button* startBtn = (Button*)widget->getChildByName("startBtn");
	Button* cancelBtn = (Button*)widget->getChildByName("cancelBtn");
	Button* halfBtn = (Button*)widget->getChildByName("halfBtn");
	Button* doubleBtn = (Button*)widget->getChildByName("doubleBtn");
	Button* smallBtn = (Button*)widget->getChildByName("smallBtn");
	Button* bigBtn = (Button*)widget->getChildByName("bigBtn");
	CheckBox* soundBox = (CheckBox*)widget->getChildByName("soundBox");
	soundBox->addEventListenerCheckBox(this, checkboxselectedeventselector(GameScene::onSelected));
	winNumLabel = (TextAtlas*)widget->getChildByName("winNum");
	totalNumLabel = (TextAtlas*)widget->getChildByName("totalNum");

	totalGold = GameLogic::GetInstance()->user.coin;
	totalNumLabel->setStringValue(util::toString(totalGold));
	winNumLabel->setStringValue("0");

	pointNumLabel = (TextAtlas*)widget->getChildByName("pointNumLabel");
	TextAtlas* timeNumLabel = (TextAtlas*)widget->getChildByName("timeNum");
	timeNumLabel->setVisible(false);


	for (int i = 1; i <= 8; i++){
		cocos2d::ui::Button* betBtn = (cocos2d::ui::Button*)widget->getChildByName("betBtn" + cocos2d::Value(i).asString());
		betBtn->addTouchEventListener(this, toucheventselector(GameScene::onTouch));
		TextAtlas* label = (TextAtlas*)widget->getChildByName("betNum" + cocos2d::Value(i).asString());
		label->setStringValue("00");
		betLabels[i - 1] = label;
	}
	for (int i = 0; i < GameScene::ROUND_COUNT; i++){
		boxArr[i] = Box::create(0);
		//boxArr[i]->retain();
		boxArr[i]->setPos(i);
		boxArr[i]->setVisible(false);
		widget->addChild(boxArr[i]);
	}
	lightLeft = nullptr;
	lightRight = nullptr;
	sprite = nullptr; 
	startBtn->addTouchEventListener(this, toucheventselector(GameScene::onTouch));
	cancelBtn->addTouchEventListener(this, toucheventselector(GameScene::onTouch));
	halfBtn->addTouchEventListener(this, toucheventselector(GameScene::onTouch));
	doubleBtn->addTouchEventListener(this, toucheventselector(GameScene::onTouch));
	smallBtn->addTouchEventListener(this, toucheventselector(GameScene::onTouch));
	bigBtn->addTouchEventListener(this, toucheventselector(GameScene::onTouch));
	box0 = Box::create();
	widget->addChild(box0);
	box1 = nullptr;
	box2 = nullptr;
	box3 = nullptr;
	imgMap.insert(map<int, string>::value_type(1, "img/eat"));
	imgMap.insert(map<int, string>::value_type(2, "img/xsy"));
	imgMap.insert(map<int, string>::value_type(3, "img/xsx"));
	imgMap.insert(map<int, string>::value_type(4, "img/xmg"));
	imgMap.insert(map<int, string>::value_type(5, "img/dsy"));
	imgMap.insert(map<int, string>::value_type(6, "img/dmg"));
	imgMap.insert(map<int, string>::value_type(7, "img/khc"));

	mm_imgs.insert(map<int, string>::value_type(0, "img/MM/MM_0000.png"));
	mm_imgs.insert(map<int, string>::value_type(1, "img/MM/MM_0001.png"));
	mm_imgs.insert(map<int, string>::value_type(2, "img/MM/MM_0002.png"));
	mm_imgs.insert(map<int, string>::value_type(3, "img/MM/MM_0003.png"));
	mm_imgs.insert(map<int, string>::value_type(4, "img/MM/MM_0004.png"));
	mm_imgs.insert(map<int, string>::value_type(5, "img/MM/MM_0005.png"));
	mm_imgs.insert(map<int, string>::value_type(6, "img/MM/MM_0006.png"));
	step = 0;
	/*CustomTool::preloadBackgroundMusic("sounds/bg.ogg");
	CustomTool::preloadEffect("sounds/run.ogg");
	CustomTool::preloadEffect("sounds/bs.ogg");
	CustomTool::preloadEffect("sounds/bswin.ogg");
	CustomTool::preloadEffect("sounds/chidiao.ogg");
	CustomTool::preloadEffect("sounds/train.ogg");
	for (int i = 0; i < 8; i++){
		CustomTool::preloadEffect("sounds/s" + cocos2d::Value(i + 1).asString() + ".ogg");
		CustomTool::preloadEffect("sounds/w" + cocos2d::Value(i + 1).asString() + ".ogg");
	}
	CustomTool::preloadEffect("sounds/w9.ogg");*/
	cur_mm = 0;

	//请求逻辑用户数据
	//requestData();

	initGame();


	

	return true;
}


void GameScene::requestData(){
	LanguageConfig::GetInstance()->init("lang_s");
	GameLogic::GetInstance()->init();
	if (!GameLogic::GetInstance()->parseLoginCode()){
		JNI::cppInfo("login error");
	}
	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);
	root.AddMember("c", CODE_GAME_GET_COIN_RE, allocator);
	rapidjson::Value data(rapidjson::kObjectType);
	data.AddMember("coin", 0, allocator);
	root.AddMember("data", data, allocator);
	GameLogic::GetInstance()->SendRequest(root);
}

void GameScene::addMethod(FP fp){
	for (vector<FP>::iterator it = methods.begin(); it != methods.end(); it++){
		if (*it == fp){
			return;
		}
	}
	methods.push_back(fp);
	if (!this->isScheduled(schedule_selector(GameScene::frameHandler))){
		this->schedule(schedule_selector(GameScene::frameHandler),0.05);
	}
}

void GameScene::removeMethod(FP fp){
	for (vector<FP>::iterator it = methods.begin(); it != methods.end(); it++){
		if (*it == fp){
			methods.erase(it);
			break;
		}
	}
	if (methods.empty()){
		this->unschedule(schedule_selector(GameScene::frameHandler));
	}
}

void GameScene::frameHandler(float t){
	/*for (vector<FP>::iterator it = methods.begin(); it != methods.end(); it++){
	(this->*(*it))();
	}*/
	for (int i = methods.size() - 1; i >= 0; i--){
		(this->*methods[i])();
	}
}

void GameScene::onSelected(Ref* obj, CheckBoxEventType type){
	CheckBox* checkBox = (CheckBox*)obj;
	bool flag = checkBox->getSelectedState();
	GameScene::SoundOpen = !flag;
	if (GameScene::SoundOpen){
		CustomTool::playMusic("sounds/bg.ogg", true);
	} else{
		CustomTool::stopAllSound();
	}
}

void GameScene::onEnter(){
	Layer::onEnter();
	//initGame();
	auto listener2 = EventListenerKeyboard::create();
	//�ͻص�������
	listener2->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	//��ӵ��¼��ַ�����
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
	this->schedule(schedule_selector(GameScene::changeMMImg), 30);
}

void GameScene::changeMMImg(float t){
	cur_mm++;
	cur_mm %= mm_imgs.size();
	ImageView* img = (ImageView*)widget->getChildByName("mm_img");
	img->loadTexture(mm_imgs.at(cur_mm));
}

void GameScene::initGame(){
	totalGold = GameLogic::GetInstance()->user.coin;
	ownGold = totalGold;
	winGold = 0;

	for (int i = 0; i < 8; i++){
		prevBet[i] = 0;
	}
	box0->setPos(0);
	box0->stop();
	clearBet();
	nextRound();
	CustomTool::playMusic("sounds/bg.ogg",true);
}

void GameScene::onTouch(Ref* obj, TouchEventType type){
	Button* btn = (Button*)obj;
	string name = btn->getName();
	if (type != TouchEventType::TOUCH_EVENT_ENDED && name.find("betBtn") == -1) return;
	if (name == "startBtn"){
		toStart();
	} else if (name == "cancelBtn"){
		betCancel();
	} else if (name == "halfBtn"){  //充值金币
		betSize(2);
	} else if (name == "doubleBtn"){ //全部加+1
		//betSize(1);
		toBet(1);
		toBet(2);
		toBet(3);
		toBet(4);
		toBet(5);
		toBet(6);
		toBet(7);
		toBet(8);
	} else if (name == "smallBtn"){  //选择赌小代码
		dundaxiao(4);
	} else if (name == "bigBtn"){   //选择赌大代码
		dundaxiao(3);
	} else if (name.find("betBtn") != -1){
		int index = atoi(&(name.at(name.size() - 1)));
		if (type == TouchEventType::TOUCH_EVENT_BEGAN){
			PressBet = index;
			addMethod(&GameScene::addBet);
		} else if (type == TouchEventType::TOUCH_EVENT_ENDED || type == TouchEventType::TOUCH_EVENT_CANCELED){
			PressBet = 0;
			toBet(index);
		}
	}
}

void GameScene::addBet(){
	if (PressBet == 0){
		removeMethod(&GameScene::addBet);
		return;
	}
	toBet(PressBet);
}

//开始旋转
void GameScene::toStart(){
	//CustomTool::playSoundEffect("sounds/s1.ogg");
	if (step == 3){
		collectGold();
	} else if (step == 0){
		clearResult();
		if (betGold <= 0){
			betAgain();
		}
		if (betGold > 0){
			step = 1;
			bengin();
			//getWinId();
			CustomTool::playSoundEffect("sounds/TURN_START.ogg");
		}
	}
}

void GameScene::betCancel(){
	if (step != 0) return;
	CustomTool::playSoundEffect("sounds/s1.ogg");
	clearBet();
	for (int i = 0; i < 8; i++){
		prevBet[i] = 0;
	}
	showGoldNum();
}

void GameScene::runBox(){
	step = 1;
	box0->run();
}

void GameScene::getResult(){
	if (step == 1){
		if (curWinId == 9 || curWinId == 21){
			step = 2;
			box0->starFast();
			playSoundEffect(curWinId);
		} else{
			if (winGold > 0){
				playSoundEffect(curWinId);
			} else{
				CustomTool::playSoundEffect("sounds/chidiao.ogg");
			}
			this->scheduleOnce(schedule_selector(GameScene::waitLater), 2);
			box0->stop();
			showGoldNum();
		}
	} else if (step == 2){
		this->scheduleOnce(schedule_selector(GameScene::waitLater), 3);
		showGoldNum();
	}
}

void GameScene::playSoundEffect(int id){
	string soundName;
	switch (id){
		case 0:
		case 11:
		case 12:
			soundName = "w1.mp3";
			break;
		case 1:
		case 13:
		case 23:
			soundName = "w2.mp3";
			break;
		case 2:
		case 3:
			soundName = "w3.mp3";
			break;
		case 4:
		case 5:
		case 10:
		case 16:
		case 22:
			soundName = "w4.mp3";
			break;
		case 6:
		case 17:
		case 18:
			soundName = "w5.mp3";
			break;
		case 7:
		case 8:
			soundName = "w6.mp3";
			break;
		case 14:
		case 15:
			soundName = "w7.mp3";
			break;
		case 19:
		case 20:
			soundName = "w8.mp3";
			break;
		case 9:
		case 21:
			soundName = "bswin.ogg";
			break;
		default:
			break;
	}
	if (id == 9 || id == 21){
		CustomTool::playSoundEffect("sounds/" + soundName);
	} else{
		CustomTool::stopAllSound();
		CustomTool::playMusic("sounds/" + soundName, true);
	}

	//撒金币
	startGold();
	this->scheduleOnce(schedule_selector(GameScene::closelizi), 3);
}

void GameScene::waitLater(float t){
	if (winGold > 0){
		step = 3;
	} else{
		nextRound();
	}
}

void GameScene::playResult(){
	if (luckId == 1){
		sprite = Sprite::create(imgMap[luckId] + ".png");
		widget->addChild(sprite);
		CustomTool::playSoundEffect("sounds/chidiao.ogg");
	}else{
		box0->setVisible(false);
		sprite = Sprite::create();
		Vector<SpriteFrame*> allframe;//��������֡

		string s1 = imgMap[luckId] + "0.png";
		string s2 = imgMap[luckId] + "1.png";

		SpriteFrame* sf1 = CustomTool::getSpriteFrameFromLocal(s1);
		SpriteFrame* sf2 = CustomTool::getSpriteFrameFromLocal(s2);


		allframe.pushBack(sf1);
		allframe.pushBack(sf2);
		Animation * ani = Animation::createWithSpriteFrames(allframe, 0.5);
		sprite->runAction(RepeatForever::create(Animate::create(ani)));
		widget->addChild(sprite);
		switch (luckId){
			case 2:
				boxArr[11]->setVisible(true);
				boxArr[17]->setVisible(true);
				boxArr[23]->setVisible(true);
				break;
			case 3:
				boxArr[4]->setVisible(true);
				boxArr[10]->setVisible(true);
				boxArr[16]->setVisible(true);
				boxArr[22]->setVisible(true);
				break;
			case 4:
				boxArr[2]->setVisible(true);
				boxArr[5]->setVisible(true);
				boxArr[8]->setVisible(true);
				boxArr[11]->setVisible(true);
				boxArr[14]->setVisible(true);
				boxArr[17]->setVisible(true);
				boxArr[20]->setVisible(true);
				boxArr[23]->setVisible(true);
				break;
			case 5:
				boxArr[7]->setVisible(true);
				boxArr[15]->setVisible(true);
				boxArr[19]->setVisible(true);
				break;
			case 6:
				for (int i = 0; i < GameScene::ROUND_COUNT; i++){
					boxArr[i]->setVisible(true);
				}
				lightUp();
				break;
			case 7:
				startTrain();
				break;
			default:
				break;
		}
	}
	if (luckId != 7){
		getResult();
		if (luckId != 1){
			CustomTool::playMusic("sounds/w9.mp3",true);
		}
	}
	sprite->setPosition(Vec2(widget->getContentSize().width * .5, 525));
}

void GameScene::startTrain(){
	int pos = box0->getPos();
	box1 = Box::create(1);
	box2 = Box::create(2);
	box3 = Box::create(3);
	box1->setPos(pos);
	box2->setPos(pos);
	box3->setPos(pos);
	widget->addChild(box1);
	widget->addChild(box2);
	widget->addChild(box3);
	box0->run(CC_CALLBACK_0(GameScene::followFirstTrain, this));
	box0->setTargetPos(trainId);
	CustomTool::stopAllSound();
	CustomTool::playMusic("sounds/train.ogg");
}

void GameScene::followFirstTrain(){
	if (box1->getPos() != box0->getPos()){
		box1->setPos(box0->getPos() - 1);
	}
	if (box2->getPos() != box1->getPos()){
		box2->setPos(box1->getPos() - 1);
	}
	if (box3->getPos() != box2->getPos()){
		box3->setPos(box2->getPos() - 1);
	}
}


void GameScene::lightUp(){
	lightLeft = Sprite::create();
	lightRight = Sprite::create();
	Vector<SpriteFrame*> aframes;//��������֡
	SpriteFrame* sf1 = CustomTool::getSpriteFrameFromLocal("img/light0.png");
	SpriteFrame* sf2 = CustomTool::getSpriteFrameFromLocal("img/light1.png");
	aframes.pushBack(sf1);
	aframes.pushBack(sf2);
	Animation * ani = Animation::createWithSpriteFrames(aframes, 0.5);
	lightLeft->runAction(RepeatForever::create(Animate::create(ani)));
	lightRight->runAction(RepeatForever::create(Animate::create(ani)));
	widget->addChild(lightLeft);
	widget->addChild(lightRight);
	lightLeft->setPosition(widget->getChildByName("lightLeft")->getPosition());
	lightRight->setPosition(widget->getChildByName("lightRight")->getPosition());
}

void GameScene::collectGold(){
	winNumLabel->setStringValue("00000000");
	totalGold =  totalGold + winGold;

	winGold = 0;
	nextRound();
	showGoldNum();
	clearResult();
}

//清除结果
void GameScene::clearResult(){
	if (sprite != nullptr){
		widget->removeChild(sprite);
		sprite = nullptr;
	}
	if (lightLeft != nullptr){
		widget->removeChild(lightLeft);
		widget->removeChild(lightRight);
		lightLeft = nullptr;
		lightRight = nullptr;
	}
	if (box1 != nullptr){
		widget->removeChild(box1);
		widget->removeChild(box2);
		widget->removeChild(box3);
		box1 = nullptr;
		box2 = nullptr;
		box3 = nullptr;
	}
	CustomTool::stopAllEffects();
	CustomTool::playMusic("sounds/bg.ogg", true);
	for (int i = 0; i < GameScene::ROUND_COUNT; i++){
		boxArr[i]->setVisible(false);
	}
}

void GameScene::getWinId(){
	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);
	rapidjson::Value data(rapidjson::kObjectType);
	rapidjson::Value bet(rapidjson::kArrayType);
	for (int i = 0; i < 8; i++){
		int betNum = betArr[i];
		bet.PushBack(betNum, allocator);
	}
	data.AddMember("bet", bet, allocator);
	data.AddMember("gameid", GameLogic::GetInstance()->gameid, allocator);
	root.AddMember("c", MSG_BET_SAMPLE_REQ, allocator);
	root.AddMember("data", data, allocator);
	GameLogic::GetInstance()->SendRequest(root);
}


//消息返回 
void GameScene::getResponse(int code, string ret, string info /*= ""*/, const rapidjson::Value &data /*= NULL*/){
	if (ret != "0"){
		if (ret == "900"){
			info = "NET_ERROR";
		}
		string loginAgain = LanguageConfig::GetInstance()->FindLang(info);
		JNI::cppInfo(loginAgain);
		//MainScene::Quit();
		return;
	}
	if (code == MSG_BET_SAMPLE_RSP){
		curWinId = util::toInt(data["wintag"].GetString());
		luckId = util::toInt(data["wintagtype"].GetString());
		totalGold = util::toInt(data["coin"].GetString());
		winGold = util::toInt(data["wincoin"].GetString());
		if (luckId != 7){
			trainId = 0;
		} else{
			trainId = util::toInt(data["wintagvalue"][(unsigned)3].GetString());
		}
		for (int i = 0; i < 8; i++){
			prevBet[i] = betArr[i];
			betArr[i] = 0;
		}
		runBox();
		box0->setTargetPos(curWinId);
	} else if (code == MSG_FRUIYS_DA_XIA_RSP){
		pointNum = util::toInt(data["num"].GetString());
		totalGold = util::toInt(data["coin"].GetString());
		winFlag = util::toInt(data["type"].GetString());
		this->schedule(schedule_selector(GameScene::runPoint), 0.2);
	} else if (code == CODE_GAME_GET_COIN_RS){
		//GameLogic::GetInstance()->user.coin = util::toInt(data["coin"].GetString());
		this->initGame();
	}
}

void GameScene::betSize(int type){
	if (step != 3) return;
	clearResult();
	CustomTool::playSoundEffect("sounds/s1.ogg");
	switch (type){
		case 1:
			if (ownGold > winGold){
				//ownGold -= winGold;
				winGold *= 2;
			}
			break;
		case 2:
			if (winGold > 1){
				int gold = (int)winGold * 0.5;
				//ownGold += gold;
				winGold -= gold;
			}
			break;
		case 3:
		case 4:
			betType = type - 2;
			betSizeResult();
			break;
	}
	showGoldNum();
}

void GameScene::betSizeResult(){
	step = 4;
	betTime = 0;
	pointNum = 0;
	getPointNum();
}

//发送消息赌大小
void GameScene::getPointNum(){
	//srand((int)time(0));
	//pointNum = rand() % 14 + 1;
	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);
	rapidjson::Value data(rapidjson::kObjectType);
	data.AddMember("coin", winGold, allocator);
	data.AddMember("num", betType == 2 ? 0 : 1, allocator);
	data.AddMember("gameid", GameLogic::GetInstance()->gameid, allocator);
	root.AddMember("c", MSG_FRUIYS_DA_XIA_REQ, allocator);
	root.AddMember("data", data, allocator);
	GameLogic::GetInstance()->SendRequest(root);
}

void GameScene::randomPoint(){
	srand((int)time(0));
	int prev_point = atoi(pointNumLabel->getStringValue().c_str());
	int point = rand() % 14 + 1;
	while (prev_point == point){
		point = rand() % 14 + 1;
	}
	CustomTool::playSoundEffect("sounds/run.ogg");
	string ps = cocos2d::Value(point).asString();
	pointNumLabel->setStringValue(ps);
	betTime++;
}

void GameScene::runPoint(float t){
	if (betTime > 12 && pointNum > 0){
		this->unschedule(schedule_selector(GameScene::runPoint));
		betTime = 0;
		this->schedule(schedule_selector(GameScene::runPoint2), 0.5);
	} else{
		randomPoint();
	}
}

void GameScene::runPoint2(float t){
	if (betTime > 3){
		string ps = cocos2d::Value(pointNum).asString();
		pointNumLabel->setStringValue(ps);
		this->unschedule(schedule_selector(GameScene::runPoint2));
		getPointResult();
	} else{
		randomPoint();
	}
}

void GameScene::getPointResult(){
	if (winFlag){
		//totalGold += winGold;
		winGold *= 2;
		step = 3;
		CustomTool::playSoundEffect("sounds/D_STAR.ogg");
	} else{
		//totalGold -= winGold;
		winGold = 0;
		nextRound();
		CustomTool::playSoundEffect("sounds/chidiao.ogg");
	}
	showGoldNum();
}

void GameScene::toBet(int pos){
	if (step != 0) return;
	if (betArr[pos - 1] >= BET_MAX) return;
	if (ownGold == 0) return;
	if (newRound){
		clearBet();
		clearResult();
		newRound = false;
	}
	if (PressBet == 0){
		CustomTool::playSoundEffect("sounds/s" + cocos2d::Value(pos).asString() + ".ogg");
	}
	betArr[pos - 1] ++;
	showGoldNum(1);
}

void GameScene::clearBet(){
	for (int i = 0; i < 8; i++){
		betArr[i] = 0;
	}
	winGold = 0;
	showGoldNum();
}


//开始旋转判断
void GameScene::betAgain(){
	int total = 0;
	for (int i = 0; i < 8; i++){
		total += prevBet[i];
	}
	if (totalGold < total){
		//弹出---金币不足
		string coin_require = LanguageConfig::GetInstance()->FindLang("coin_require");
		JNI::cppInfo(coin_require);
		return;
	}
	for (int i = 0; i < 8; i++){
		betArr[i] = prevBet[i];
	}
	//totalGold = totalGold - total;
	showGoldNum();
}

//显示赢来的钱  和 显示 总金额
void GameScene::showGoldNum(int types){
	bool flag = false;
	for (int i = 0; i < 8; i++){
		if (betArr[i] != 0){
			flag = true;
			break;
		}
	}
	betGold = 0;
	for (int i = 0; i < 8; i++){
		int betNum = flag ? betArr[i] : prevBet[i];
		betGold += betArr[i];
		TextAtlas* label = betLabels[i];
		string str = betNum >= 10 ? cocos2d::Value(betNum).asString() : "0" + cocos2d::Value(betNum).asString();
		label->setStringValue(str);
	}
	ownGold = totalGold  - betGold;
	string goldString = cocos2d::Value(ownGold).asString();
	/*while (goldString.size() < 8){
		goldString = "0" + goldString;
	}*/
	//总金额
	totalNumLabel->setStringValue(goldString);
	goldString = cocos2d::Value(winGold).asString();
	/*while (goldString.size() < 8){
		goldString = "0" + goldString;
	}*/
	//赢来金额
	winNumLabel->setStringValue(goldString);
	
	CCUserDefault::sharedUserDefault()->setIntegerForKey("integer", totalGold);
	CCUserDefault::sharedUserDefault()->flush();
}

void GameScene::returnMainScene(){
	//auto director = Director::getInstance();
	//auto scene = MainScene::CreateScene();
	//director->runWithScene(scene);
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *pEvent)
{
	if (keyCode != EventKeyboard::KeyCode::KEY_BACK && keyCode != EventKeyboard::KeyCode::KEY_BACKSPACE){
		return;
	}
	JNI::cppLoginOut("");
}

void GameScene::nextRound(){
	step = 0;
	newRound = true;
	PressBet = 0;
}


//开始赌大小结果
//type 4 = 小  3= 大
void GameScene::dundaxiao(int type)
{
	step = 4;     //4等待结果状态
	betTime = 0;


	pointNum = GameLogic::GetInstance()->getluckDx(type,totalGold);   //大小开出来的结果
	if(pointNum > 7 && type == 3)
	{
		winFlag = 1;
		//totalGold = totalGold + winGold*2;
		this->schedule(schedule_selector(GameScene::runPoint), 0.2);
		return;
	}
	if(pointNum <= 7 && type == 4)
	{
		winFlag = 1;
		//totalGold = totalGold + winGold*2;
		this->schedule(schedule_selector(GameScene::runPoint), 0.2);
		return;
	}
	winFlag = 0;
	this->schedule(schedule_selector(GameScene::runPoint), 0.2);

}



/*
1- 苹果：5
2- 橘子：10
3- 芒果：10
4- 铃铛：15
5- 西瓜：20
6- 星星：30
7- 77 ： 40
8- BAR ： 120 倍
*/



/*
老虎机格子编号
1-大铃铛 - 15倍
2-HAH    - 50倍
3-BAR   - 120倍
4-苹果   - 5倍
5-小苹果 -3倍
6-芒果  -10倍
7-大西瓜 -20倍
8-小西瓜 -3倍
9-GOODLUCK -运气
10-苹果 - 5倍
11-橘子 - 3倍
12-大橘子 - 10倍
13-铃铛 - 15倍
14-双7 - 3倍数
15-大双7 - 40倍
16-大苹果 - 5倍
17-小芒果 - 3倍
18-大芒果 - 10倍数
19-双星 - 30倍
20-小双星 -3倍
21-GOODLUCK -
22-苹果- 5倍
23- 铃铛 - 3倍
24-橘子 - 10倍
*/


//开始设置旋转结果
void GameScene::bengin()
{
	//totalGold = totalGold + winGold;

	int rd = GameLogic::GetInstance()->getRandPro(betArr,totalGold);
	map<int,Gezis *>::iterator itr  = GameLogic::GetInstance()->v_gezi.find(rd);
	Gezis *gzsinfo = itr->second;
	int pro = gzsinfo->fenshu;
	int zid = gzsinfo->zhuid;
	//CCLOG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %d",i);
	//设定结果
	curWinId = rd;
	//获得幸运结果
	luckId = GameLogic::GetInstance()->getluckPro(totalGold);
	if (luckId == 0) luckId = 1;
	trainId = 0;
	//总金币
	//totalGold = 99999;
	totalGold = totalGold - betGold;

	int sumgold = 0;
	if(zid > 0 && pro>0)
	{
		/* 计算赢得金币 */
		for (int i = 0; i < 8; i++)
		{
			int ya_gold = betArr[i];
			if(ya_gold > 0 && (i+1) == zid) //命中
			{
				sumgold = ya_gold * pro;
			}
		}
	}else
	{
		//大满贯;全中
		if(luckId == 6 )
		{
			for (int i = 0; i < 8; i++)
			{
				int ya_gold = betArr[i];
				if(ya_gold > 0)
				{
					map<int,Gezis *>::iterator itr_b =  GameLogic::GetInstance()->v_gezi.begin();
					while (itr_b != GameLogic::GetInstance()->v_gezi.end() )
					{
						Gezis *gzsinfo = (itr_b->second);
						if (gzsinfo->zhuid == i)
						{
							sumgold = sumgold + (gzsinfo->fenshu * ya_gold);
						}
					}
					
				}
			}
		}
		else if(luckId == 7) //开火车
		{
			srand((int)time(0));
			trainId = rand()%23;
			if(trainId ==9 || trainId == 21)
				trainId++;
			map<int,Gezis *>::iterator itrs  = GameLogic::GetInstance()->v_gezi.find(rd);
			Gezis *gzsinfos = itrs->second;
			/* 计算赢得金币 */
			for (int i = 0; i < 8; i++)
			{
				int ya_golds = betArr[i];
				if(ya_golds > 0 && (i+1) == gzsinfos->zhuid) //命中
				{
					sumgold = ya_golds * gzsinfos->fenshu;
				}
			}

		}
		else if(luckId == 3)
		{
			//小四喜；4个苹果 *5倍 （4,10，16，22）
			if(betArr[0] >0 )
			{
				sumgold = (betArr[0] * 5) * 4;
			}
		}
		else if(luckId == 2)
		{
			//小三元
			//铃铛 芒果 橙子 * 3
			sumgold = sumgold + betArr[1] * 3;
			sumgold = sumgold + betArr[2] * 3;
			sumgold = sumgold + betArr[3] * 3;
			
		}
		else if(luckId == 4)
		{
			//小满贯 2,5,8,11,14,17,20,23
			sumgold = sumgold + betArr[7] * 50;
			sumgold = sumgold + betArr[0] * 3;
			sumgold = sumgold + betArr[4] * 3;
			sumgold = sumgold + betArr[1] * 3;
			sumgold = sumgold + betArr[6] * 3;
			sumgold = sumgold + betArr[2] * 3;
			sumgold = sumgold + betArr[5] * 3;
			sumgold = sumgold + betArr[3] * 3;
		}
		else if(luckId == 5)
		{
			//大三元 7,15,19,
			sumgold = sumgold + betArr[4] * 20;
			sumgold = sumgold + betArr[7] * 40;
			sumgold = sumgold + betArr[5] * 30;
		}
	}
	
	if(winGold >0 )
	{
		//this->scheduleOnce(schedule_selector(GameScene::startGold), 2);
	}

	//赢得金币
	winGold = sumgold;

	if (luckId != 7){
		trainId = 0;
	} else{
		trainId = 5;
	}
	for (int i = 0; i < 8; i++){
		prevBet[i] = betArr[i];
		betArr[i] = 0;
	}
	runBox();
	box0->setTargetPos(curWinId);
}
 


void GameScene::closelizi(float t)
{
	_emitter->removeFromParent();
	CustomTool::stopEffect(getcoinid);
}

//开始撒金币
void GameScene::startGold()
{
	_emitter =ParticleFire::create();
	_emitter->retain();
	
	_emitter->setTexture( Director::getInstance()->getTextureCache()->addImage("img/getPoints.png") );//.pvr");
	
	auto p = _emitter->getPosition();
	_emitter->setPosition( Vec2(p.x,p.y+800) );
	_emitter->setGravity(CCPoint(0,-940));
	_emitter->setStartSize(15.0); //粒子大小
	_emitter->setRadialAccel(400); 
	_emitter->setRadialAccelVar(0);  
	
	_emitter->setEmissionRate(60);  
	_emitter->autorelease();
	this->addChild(_emitter, 10);

	getcoinid = CustomTool::playSoundEffect("sounds/coinGet.ogg",true);
	//this->scheduleOnce(schedule_selector(GameScene::startGold), 2);
}