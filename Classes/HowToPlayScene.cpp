
#include "HowToPlayScene.h"
#include "MainGameScene.h"

const char* BTN_DONT_SHOW_AGAIN_SPRITE_PATH = "res/dont_show.png";
const char* BTN_DONT_SHOW_AGAIN_CLICKED_SPRITE_PATH = "res/dont_show_clicked.png";

const char* HOW_TO_PLAY_1_SPRITE_PATH = "res/howtoplay_1.png";
const char* HOW_TO_PLAY_2_SPRITE_PATH = "res/howtoplay_2.png";
const char* HOW_TO_PLAY_3_SPRITE_PATH = "res/howtoplay_3.png";
const char* HOW_TO_PLAY_4_SPRITE_PATH = "res/howtoplay_4.png";

const char* USER_DEFAULT_SHOW_AGAIN = "show_again";

cocos2d::CCScene* CHowToPlayScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	CHowToPlayScene *layer = CHowToPlayScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


bool CHowToPlayScene::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	this->setTouchEnabled( true );

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	_mHelpSprite = CCSprite::create(HOW_TO_PLAY_1_SPRITE_PATH);
	_mHelpSprite->setPosition(ccp( winSize.width/2, winSize.height/2));
	this->addChild(_mHelpSprite);

	// Make the ONLINE button
	CCMenuItemImage* btnRankSprite = CCMenuItemImage::create(
		BTN_DONT_SHOW_AGAIN_SPRITE_PATH,
		BTN_DONT_SHOW_AGAIN_CLICKED_SPRITE_PATH,
		this, 
		menu_selector(CHowToPlayScene::OnClickDontShowButton) );

	btnRankSprite->setPosition(ccp(winSize.width * 9.1 / 10, winSize.height * 9.1 / 10) );

	CCMenu* menu = CCMenu::create(btnRankSprite, NULL);
	menu->setPosition(ccp(0, 0));
	this->addChild(menu, 10);

	_mPath[0] = HOW_TO_PLAY_1_SPRITE_PATH;
	_mPath[1] = HOW_TO_PLAY_2_SPRITE_PATH;
	_mPath[2] = HOW_TO_PLAY_3_SPRITE_PATH;
	_mPath[3] = HOW_TO_PLAY_4_SPRITE_PATH;

	_mCurrentIndex = 0;

	return true;
}

void CHowToPlayScene::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	for( CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		CCTouch* touch = (CCTouch*)(*it);
		CCPoint touchPoint = touch->getLocation();

		_mOldTouch = touchPoint;
	}
}


void CHowToPlayScene::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
	for( CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		CCTouch* touch = (CCTouch*)(*it);
		CCPoint touchPoint = touch->getLocation();

		float diff = touchPoint.x - _mOldTouch.x;
		if( diff < 0 || abs(diff) < 20 )
		{
			// to right
			if(_mCurrentIndex == 3 )
			{
				CCDirector::sharedDirector()->replaceScene(MainGameScene::scene());
			}
			else
			{
				_mHelpSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(_mPath[++_mCurrentIndex]));
			}
		}
		else
		{
			// to left
			if( _mCurrentIndex == 0 )
				return;

			_mHelpSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(_mPath[--_mCurrentIndex]));
		}
	}
}

void CHowToPlayScene::OnClickDontShowButton(CCObject* pSender)
{

}
