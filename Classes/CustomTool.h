#ifndef __CustomTool__
#define __CustomTool__
#include "cocos2d.h"
#include "cocostudio/CCSGUIReader.h"

USING_NS_CC;

class CustomTool : public Ref
{
public:
	static CustomTool* getInstance();
	/*
	create MenueItemImage with SpriteFrameName
	*/
	static MenuItemImage* createMenuItemImage(const char* normal, const char* selected, ccMenuCallback callback);

	static cocos2d::ui::Widget* getCSUI(std::string name, cocos2d::CCNode* parent);

	static SpriteFrame* getSpriteFrameFromLocal(std::string filename);

	static void playMusic(std::string filename, bool bLoop = false);

	static void preloadBackgroundMusic(std::string filename);
	static void preloadEffect(std::string filename);
	static void pauseEffect(unsigned int soundId);
	static void resumeEffect(unsigned int nSoundId);
	static void stopAllSound();
	static unsigned int playSoundEffect(std::string filename, bool bLoop = false,
		float pitch = 1.0f, float pan = 0.0f, float gain = 1.0f);
	static void stopEffect(unsigned int nSoundId);
	static void stopAllEffects();

private:
	static CustomTool* _tool;
};

#endif
