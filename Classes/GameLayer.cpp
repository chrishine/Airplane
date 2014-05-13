#include "GameLayer.h"
#include "Enemy.h"


Level GameLayer::level=EASY;

GameLayer::GameLayer(void)
{
	background1=NULL;
	background2=NULL;
	planeLayer=NULL;
	bulletLayer=NULL;
	mutiBulletsLayer=NULL;
	controlLayer=NULL;
	enemyLayer=NULL;

	score=0;
	bigBoomCount=0;
}

GameLayer::~GameLayer(void)
{
}

bool GameLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		level=EASY;

		if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_music.mp3",true);
		}

		background1=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("background.png"));
		background1->setAnchorPoint(ccp(0,0));
		background1->setPosition(ccp(0,0));
		this->addChild(background1);

		background2=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("background.png"));
		background2->setAnchorPoint(ccp(0,0));
		background2->setPosition(ccp(0,background2->getContentSize().height-2));
		this->addChild(background2);

		this->planeLayer=PlaneLayer::create();
		this->addChild(planeLayer);

		this->bulletLayer=BulletLayer::create();
		this->addChild(bulletLayer);
		this->bulletLayer->StartShoot();

		this->mutiBulletsLayer=MutiBulletsLayer::create();
		this->addChild(mutiBulletsLayer);

		this->enemyLayer=EnemyLayer::create();
		this->addChild(enemyLayer);

		this->controlLayer=ControlLayer::create();
		this->addChild(controlLayer);

		this->ufoLayer=UFOLayer::create();
		this->addChild(ufoLayer);

		this->setTouchEnabled(true);

		this->schedule(schedule_selector(GameLayer::backgroundMove),0.01f);

		this->scheduleUpdate();

		this->setKeypadEnabled(true);

		auto dispather = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
		listener->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchCancelled, this);
		listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
		dispather->addEventListenerWithSceneGraphPriority(listener, this);
		bRet=true;
	} while (0);
	return bRet;
}

void GameLayer::backgroundMove(float dt)
{
	background1->setPositionY(background1->getPositionY()-2);
	background2->setPositionY(background1->getPositionY()+background1->getContentSize().height-2);
	if (background2->getPositionY()==0)
	{
		background1->setPositionY(0);
	}
}

bool GameLayer::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void GameLayer::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (this->planeLayer->isAlive)
	{
		CCPoint beginPoint=pTouch->getLocationInView(); 
		beginPoint=CCDirector::sharedDirector()->convertToGL(beginPoint);  
		CCRect planeRect=this->planeLayer->getChildByTag(AIRPLANE)->boundingBox();
		planeRect.origin.x-=15;
		planeRect.origin.y-=15;
		planeRect.size.width+=30;
		planeRect.size.height+=30;
		if(planeRect.containsPoint(this->getParent()->convertTouchToNodeSpace(pTouch))) 
		{ 
			CCPoint endPoint=pTouch->getPreviousLocationInView(); 
			endPoint=CCDirector::sharedDirector()->convertToGL(endPoint); 

			CCPoint offSet =ccpSub(beginPoint,endPoint);
			CCPoint toPoint=ccpAdd(this->planeLayer->getChildByTag(AIRPLANE)->getPosition(),offSet); 
			this->planeLayer->MoveTo(toPoint); 
		} 
	}
}

void GameLayer::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void GameLayer::update(float dt)
{
	if (level==EASY && score>=1000000)
	{
		level=MIDDLE;
	}
	else if (level==MIDDLE && score>=2000000)
	{
		level=HARD;
	}

	CCArray* bulletsToDelete=CCArray::create();
	bulletsToDelete->retain();
	Ref* bt,*et,*ut;

	//enemy1 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;
		
		CCArray* enemy1sToDelete=CCArray::create();
		enemy1sToDelete->retain();
		int a=this->enemyLayer->m_pAllEnemy1->capacity();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et)
		{
			Enemy* enemy1=(Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife()==1)
				{
					enemy1->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy1sToDelete->addObject(enemy1);
					score+=ENEMY1_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy1sToDelete,et)
		{
			Enemy* enemy1=(Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();

	//enemy2 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;

		CCArray* enemy2sToDelete=CCArray::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et)
		{
			Enemy* enemy2=(Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife()>1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				else if(enemy2->getLife()==1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy2sToDelete->addObject(enemy2);
					score+=ENEMY2_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete,et)
		{
			Enemy* enemy2=(Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();

	//enemy3 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;

		CCArray* enemy3sToDelete=CCArray::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et)
		{
			Enemy* enemy3=(Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				if (enemy3->getLife()>1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				else if(enemy3->getLife()==1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy3sToDelete->addObject(enemy3);
					score+=ENEMY3_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete,et)
		{
			Enemy* enemy3=(Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();
	bulletsToDelete->release();


	CCArray* mutiBulletsToDelete=CCArray::create();
	mutiBulletsToDelete->retain();
	Ref* mbt;

	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;

		CCArray* enemy1sToDelete=CCArray::create();
		enemy1sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et)
		{
			Enemy* enemy1=(Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife()==1)
				{
					enemy1->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy1sToDelete->addObject(enemy1);
					score+=ENEMY1_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy1sToDelete,et)
		{
			Enemy* enemy1=(Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();

	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;

		CCArray* enemy2sToDelete=CCArray::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et)
		{
			Enemy* enemy2=(Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife()>1)
				{
					enemy2->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
				}
				else if(enemy2->getLife()==1)
				{
					enemy2->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy2sToDelete->addObject(enemy2);
					score+=ENEMY2_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete,et)
		{
			Enemy* enemy2=(Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();

	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;

		CCArray* enemy3sToDelete=CCArray::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et)
		{
			Enemy* enemy3=(Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				if (enemy3->getLife()>1)
				{
					enemy3->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
				}
				else if(enemy3->getLife()==1)
				{
					enemy3->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy3sToDelete->addObject(enemy3);
					score+=ENEMY3_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete,et)
		{
			Enemy* enemy3=(Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();
	mutiBulletsToDelete->release();


	CCRect airplaneRect=this->planeLayer->getChildByTag(AIRPLANE)->boundingBox();
	airplaneRect.origin.x+=30;
	airplaneRect.size.width-=60;
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et)
	{
		Enemy* enemy1=(Enemy*)et;
		if (enemy1->getLife()>0)
		{
			if (airplaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				this->unscheduleAllSelectors();
				this->bulletLayer->StopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planeLayer->Blowup(score);
				return;
			}
		}
	}

	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et)
	{
		Enemy* enemy2=(Enemy*)et;
		if (enemy2->getLife()>0)
		{
			if (airplaneRect.intersectsRect(enemy2->getBoundingBox()))
			{
				this->unscheduleAllSelectors();
				this->bulletLayer->StopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planeLayer->Blowup(score);
				return;
			}
		}
	}

	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et)
	{
		Enemy* enemy3=(Enemy*)et;
		if (enemy3->getLife()>0)
		{
			if (airplaneRect.intersectsRect(enemy3->getBoundingBox()))
			{
				this->unscheduleAllSelectors();
				this->bulletLayer->StopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planeLayer->Blowup(score);
				return;
			}
		}
	}

	CCARRAY_FOREACH(this->ufoLayer->m_pAllMutiBullets,ut)
	{
		CCSprite* mutiBullets=(CCSprite*)ut;
		if (this->planeLayer->getChildByTag(AIRPLANE)->boundingBox().intersectsRect(mutiBullets->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/get_double_laser.mp3");
			this->ufoLayer->RemoveMutiBullets(mutiBullets);
			this->bulletLayer->StopShoot();
			this->mutiBulletsLayer->StartShoot();
			this->bulletLayer->StartShoot(6.2f);
		}
	}

	CCARRAY_FOREACH(this->ufoLayer->m_pAllBigBoom,ut)
	{
		CCSprite* bigBoom=(CCSprite*)ut;
		if (this->planeLayer->getChildByTag(AIRPLANE)->boundingBox().intersectsRect(bigBoom->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/get_bomb.mp3");
			this->ufoLayer->RemoveBigBoom(bigBoom);
			bigBoomCount++;
			updateBigBoomItem(bigBoomCount);
		}
	}
}

void GameLayer::updateBigBoomItem(int bigBoomCount)
{
	CCSprite* normalBomb=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("bomb.png"));
	CCSprite* pressedBomb=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("bomb.png"));
	if (bigBoomCount<0)
	{
		return;
	}
	else if (bigBoomCount==0)
	{
		if(this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			this->removeChildByTag(TAG_BIGBOOM_MENUITEM,true);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
	}
	else if (bigBoomCount==1)
	{
		if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			MenuItemImage* pBigBoomItem = MenuItemImage::create("bomb.png", "bomb.png", CC_CALLBACK_1(GameLayer::menuBigBoomCallback,this));
			pBigBoomItem->setPosition(ccp(normalBomb->getContentSize().width/2+10,normalBomb->getContentSize().height/2+10));
			menuBigBoom=CCMenu::create(pBigBoomItem,NULL);
			menuBigBoom->setPosition(CCPointZero);
			this->addChild(menuBigBoom,0,TAG_BIGBOOM_MENUITEM);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
	}
	else
	{
		if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			MenuItemImage* pBigBoomItem = MenuItemImage::create("bomb.png", "bomb.png", CC_CALLBACK_1(GameLayer::menuBigBoomCallback, this));
			pBigBoomItem->setPosition(ccp(normalBomb->getContentSize().width/2+10,normalBomb->getContentSize().height/2+10));
			menuBigBoom=CCMenu::create(pBigBoomItem,NULL);
			menuBigBoom->setPosition(CCPointZero);
			this->addChild(menuBigBoom,0,TAG_BIGBOOM_MENUITEM);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
		if (bigBoomCount>=0 && bigBoomCount<=MAX_BIGBOOM_COUNT)
		{
			CCString* strScore=CCString::createWithFormat("X%d",bigBoomCount);
			bigBoomCountItem=LabelBMFont::create(strScore->getCString(),"font/font.fnt");
			bigBoomCountItem->setColor(ccc3(143,146,147));
			bigBoomCountItem->setAnchorPoint(ccp(0,0.5));
			bigBoomCountItem->setPosition(ccp(normalBomb->getContentSize().width+15,normalBomb->getContentSize().height/2+5));
			this->addChild(bigBoomCountItem,0,TAG_BIGBOOMCOUNT_LABEL);
		}
	}
}

void GameLayer::menuBigBoomCallback(Ref* pSender)
{
	if(bigBoomCount>0 && !CCDirector::sharedDirector()->isPaused())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/use_bomb.mp3");
		bigBoomCount--;
		score+=this->enemyLayer->m_pAllEnemy1->count()*ENEMY1_SCORE;
		score+=this->enemyLayer->m_pAllEnemy2->count()*ENEMY2_SCORE;
		score+=this->enemyLayer->m_pAllEnemy3->count()*ENEMY3_SCORE;
		this->enemyLayer->removeAllEnemy();
		updateBigBoomItem(bigBoomCount);
		this->controlLayer->updateScore(score);
	}
}

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	CCDirector::sharedDirector()->end();
}

Level GameLayer::getCurLevel()
{
	return level;
}