#include "ControlLayer.h"

ControlLayer::ControlLayer(void)
{
}

ControlLayer::~ControlLayer(void)
{
}

bool ControlLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();

		
		CCSprite* normalPause=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_pause_nor.png"));
		CCSprite* pressedPause=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_pause_pressed.png"));
		pauseItem = MenuItemImage::create("game_pause_nor.png", "game_pause_pressed.png", CC_CALLBACK_1(ControlLayer::menuPauseCallback, this));
		pauseItem->setPosition(ccp(normalPause->getContentSize().width/2+10,winSize.height-normalPause->getContentSize().height/2-10));
		CCMenu *menuPause=CCMenu::create(pauseItem,NULL);
		menuPause->setPosition(CCPointZero);
		this->addChild(menuPause,101);

		
		scoreItem=LabelBMFont::create("0","font/font.fnt");
		scoreItem->setColor(ccc3(143,146,147));
		scoreItem->setAnchorPoint(ccp(0,0.5));
		scoreItem->setPosition(ccp(pauseItem->getPositionX()+normalPause->getContentSize().width/2+5,pauseItem->getPositionY()));
		this->addChild(scoreItem);

		bRet=true;
	} while (0);

	return bRet;
}

void ControlLayer::menuPauseCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/button.mp3");
	if(!CCDirector::sharedDirector()->isPaused())
	{
		pauseItem->setNormalSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_resume_nor.png"));
		pauseItem->setSelectedSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_resume_pressed.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
		CCDirector::sharedDirector()->pause();
		noTouchLayer=NoTouchLayer::create();
		this->addChild(noTouchLayer);
	}
	else
	{
		pauseItem->setNormalSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_pause_nor.png"));
		pauseItem->setSelectedSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_pause_pressed.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		CCDirector::sharedDirector()->resume();
		this->removeChild(noTouchLayer,true);
	}
}

void ControlLayer::updateScore(int score)
{
	if (score>=0 && score<=MAX_SCORE)
	{
		CCString* strScore=CCString::createWithFormat("%d",score);
		scoreItem->setString(strScore->getCString());
	}
}
