#include "WelcomeLayer.h"
#include "GameScene.h"
#include "GameOverLayer.h"

WelcomeLayer::WelcomeLayer(void)
{
}

WelcomeLayer::~WelcomeLayer(void)
{
}

bool WelcomeLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		
		CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/shoot_background.plist");
		CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/shoot.plist");

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();

		
		CCSprite* background=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("background.png"));
		background->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(background);
		
		CCSprite* loading=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_loading1.png"));
		loading->setPosition(ccp(winSize.width/2,winSize.height/2-40));
		this->addChild(loading);

		CCAnimation* animation=CCAnimation::create();
		animation->setDelayPerUnit(0.2f);
		animation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_loading1.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_loading2.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_loading3.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("game_loading4.png"));

		CCAnimate* animate=CCAnimate::create(animation);
		CCRepeat* repeat=CCRepeat::create(animate,2);
		CCCallFuncN* repeatdone=CCCallFuncN::create(this,callfuncN_selector(WelcomeLayer::loadingDone));
		CCSequence* sequence=CCSequence::create(repeat,repeatdone,NULL);
		loading->runAction(sequence);
		
		getHighestHistorySorce();

		this->setKeypadEnabled(true);

		bRet=true;
	} while (0);

	return bRet;
}

void WelcomeLayer::loadingDone(CCNode* pNode)
{
	auto* pScene=GameScene::create();
	CCTransitionMoveInB *animateScene=CCTransitionMoveInB::create(0.5f,pScene);
	CCDirector::getInstance()->replaceScene(animateScene);
}

void WelcomeLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	CCDirector::getInstance()->end();
}

bool WelcomeLayer::isHaveSaveFile()
{
	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isHaveSaveFileXml"))
	{
		CCUserDefault::sharedUserDefault()->setBoolForKey("isHaveSaveFileXml", true);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",0);
		CCUserDefault::sharedUserDefault()->flush();
		return false;
	}
	else
	{
		return true;
	}
}

void WelcomeLayer::getHighestHistorySorce()
{
	if (isHaveSaveFile())
	{
		GameOverLayer::highestHistoryScore=CCUserDefault::sharedUserDefault()->getIntegerForKey("HighestScore",0);
	}
}