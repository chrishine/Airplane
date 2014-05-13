#pragma once
#include "cocos2d.h"
USING_NS_CC;

class NoTouchLayer :
	public CCLayer
{
public:
	virtual bool init();   

	CREATE_FUNC(NoTouchLayer); 

	virtual bool onTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent); 
	virtual void onTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent); 
	virtual void onTouchEnded (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent); 
};
