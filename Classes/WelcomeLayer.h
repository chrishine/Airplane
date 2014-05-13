#pragma once

#include "cocos2d.h"

USING_NS_CC;

class WelcomeLayer :
	public Layer
{
public:

	WelcomeLayer(void);

	~WelcomeLayer(void);

	virtual bool init();

	CREATE_FUNC(WelcomeLayer);

	void loadingDone(Node* pNode);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	bool isHaveSaveFile();

	void getHighestHistorySorce();
};
