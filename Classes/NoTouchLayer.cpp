#include "NoTouchLayer.h"

bool NoTouchLayer::init(){  
	if (!CCLayer::init() )  
	{  
		return false;  
	}  
	setTouchEnabled(true);  
	CCLayer::registerWithTouchDispatcher();
	return true;  
}  

bool NoTouchLayer::onTouchBegan (CCTouch *pTouch, CCEvent *pEvent)
{     
	return true;  
}  

void NoTouchLayer::onTouchMoved (CCTouch *pTouch, CCEvent *pEvent)
{  
}  

void NoTouchLayer::onTouchEnded (CCTouch *pTouch, CCEvent *pEvent)
{  
}  