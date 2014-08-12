#include "GameOverLayer.h"
#include "GameScene.h"

int GameOverLayer::highestHistoryScore=0;

GameOverLayer::GameOverLayer(void)
{
	score=0;
	highestScore=NULL;
}

GameOverLayer::~GameOverLayer(void)
{
}

GameOverLayer* GameOverLayer::create(int passScore)
{
	GameOverLayer *pRet = new GameOverLayer();
	pRet->score=passScore;
	if (pRet && pRet->init())
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}

bool GameOverLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		}
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/game_over.mp3");

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();

		//����gameover_background
	    CCSprite* background=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("gameover.png"));
		background->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(background);

		//����BackMenu
		CCSprite* normalBackToGame=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("btn_finish.png"));
		CCSprite* pressedBackToGame=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("btn_finish.png"));
		MenuItemImage* pBackItem=MenuItemImage::create();
		pBackItem->create("btn_finish.png", "btn_finish.png",CC_CALLBACK_1(GameOverLayer::menuBackCallback,this));
		pBackItem->setPosition(ccp(winSize.width-normalBackToGame->getContentSize().width/2-10,normalBackToGame->getContentSize().height/2+10));
		CCMenu *menuBack=CCMenu::create(pBackItem,NULL);
		menuBack->setPosition(CCPointZero);
		this->addChild(menuBack);

		//����score
		CCString* strScore=CCString::createWithFormat("%d",score);
		LabelBMFont* finalScore=LabelBMFont::create(strScore->getCString(),"font/font.fnt");
		finalScore->setColor(ccc3(143,146,147));
		finalScore->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(finalScore);

		CCDelayTime* delay=CCDelayTime::create(1.0f);
		CCScaleTo* scalebig=CCScaleTo::create(1.0f,3.0f);
		CCScaleTo* scalelittle=CCScaleTo::create(0.3f,2.0f);
		CCCallFunc* showAD=CCCallFunc::create(this,callfunc_selector(GameOverLayer::showAD));
		CCFiniteTimeAction* sequence=CCSequence::create(delay,scalebig,scalelittle,showAD,NULL);
		finalScore->runAction(sequence);

		CCString* strHighestScore=CCString::createWithFormat("%d",highestHistoryScore);
		highestScore=LabelBMFont::create(strHighestScore->getCString(),"font/font.fnt");
		highestScore->setColor(ccc3(143,146,147));
		highestScore->setAnchorPoint(ccp(0,0.5));
		highestScore->setPosition(ccp(140,winSize.height-30));
		this->addChild(highestScore);

		if (score>highestHistoryScore)
		{
			CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",score);
			highestHistoryScore=score;
			CCDelayTime* delayChange=CCDelayTime::create(1.3f);
			CCMoveBy* moveOut=CCMoveBy::create(0.1f,ccp(0,100));
			CCCallFuncN* beginChange=CCCallFuncN::create(this,callfuncN_selector(GameOverLayer::beginChangeHighestScore));
			CCMoveBy* moveIn=CCMoveBy::create(0.1f,ccp(0,-100));
			CCFiniteTimeAction* sequence=CCSequence::create(delayChange,moveOut,beginChange,moveIn,NULL);
			highestScore->runAction(sequence);
		}

		this->setKeypadEnabled(true);

		bRet=true;
	} while (0);
	return bRet;
}

void GameOverLayer::menuBackCallback(Ref* pSender)
{
	CCScene* pScene=GameScene::create();
	CCTransitionSlideInL* animateScene=CCTransitionSlideInL::create(1.0f,pScene);
	CCDirector::sharedDirector()->replaceScene(animateScene);
}

void GameOverLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE){
		CCDirector::sharedDirector()->end();
	}
}

void GameOverLayer::beginChangeHighestScore(CCNode* pNode)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/achievement.mp3");
	CCString* changeScore=CCString::createWithFormat("%d",score);
	highestScore->setString(changeScore->getCString());
}

void GameOverLayer::showAD()
{
	
}
