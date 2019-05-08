#include "CustomTool.h"
#include "cocos/audio/include/SimpleAudioEngine.h"
#include "GameScene.h";

CustomTool* CustomTool::_tool;

using namespace CocosDenshion;

CustomTool* CustomTool::getInstance()
{
	if(!_tool)
	{
		_tool = new CustomTool();
	}

	return _tool;
}

MenuItemImage* CustomTool::createMenuItemImage(const char* normal, const char* selected, ccMenuCallback callback)
{
	auto item = MenuItemImage::create();
	auto nsf = SpriteFrameCache::getInstance()->getSpriteFrameByName(normal);
	auto ssf = SpriteFrameCache::getInstance()->getSpriteFrameByName(selected);
	item->setNormalSpriteFrame(nsf);
	item->setSelectedSpriteFrame(ssf);
	item->setCallback(callback);
	return item;
}

SpriteFrame* CustomTool::getSpriteFrameFromLocal(std::string filename)
{
	SpriteFrame* s = NULL;
	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filename);
	if (texture)
	{
		Rect rect = Rect::ZERO;
		rect.size = texture->getContentSize();
		s = SpriteFrame::createWithTexture(texture, rect);
	}
	return s;
}

cocos2d::ui::Widget* CustomTool::getCSUI(std::string name, cocos2d::CCNode* parent)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::ui::Widget* widget = (cocos2d::ui::Widget*)cocostudio::GUIReader::shareReader()->widgetFromJsonFile((name + ".json").c_str());

	if (parent != NULL)
		parent->addChild(widget);

	widget->setAnchorPoint(Point(0.5, 0.5));
	widget->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	return widget;
}

void CustomTool::preloadBackgroundMusic(std::string filename){
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(filename.c_str());
}

void CustomTool::preloadEffect(std::string filename){
	SimpleAudioEngine::getInstance()->preloadEffect(filename.c_str());
}

void CustomTool::playMusic(std::string filename, bool bLoop){
	if (!GameScene::SoundOpen) return;
	SimpleAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), bLoop);
}

void CustomTool::pauseEffect(unsigned int soundId){
	SimpleAudioEngine::getInstance()->pauseEffect(soundId);
}

void CustomTool::stopAllSound(){
	SimpleAudioEngine::getInstance()->stopAllEffects();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void CustomTool::resumeEffect(unsigned int soundId){
	SimpleAudioEngine::getInstance()->resumeEffect(soundId);
}

unsigned int CustomTool::playSoundEffect(std::string filename, bool bLoop,float pitch, float pan, float gain){
	if (!GameScene::SoundOpen) return 0;
	return SimpleAudioEngine::getInstance()->playEffect(filename.c_str(),bLoop,pitch,pan,gain);
}

void CustomTool::stopAllEffects(){
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

void CustomTool::stopEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::getInstance()->stopEffect(nSoundId);
}