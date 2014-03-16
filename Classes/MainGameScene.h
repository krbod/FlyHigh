#ifndef __MAINGAME_SCENE_H__
#define __MAINGAME_SCENE_H__

#include "cocos2d.h"
#include "Box2D.h"
#include "CloudManager.h"
#include "RecordManager.h"
#include "MenuLayer.h"
#include "RecordLayer.h"


#define PTM_RATIO 32

USING_NS_CC;

/**
 @brief Main scene
 In this scene, The main game is played.
 */
class MainGameScene : public cocos2d::CCLayer
{
public:

	// cocos2d functions

	CREATE_FUNC(MainGameScene);
	~MainGameScene();

    virtual bool init();  
    static cocos2d::CCScene* scene();

	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);
	
	/** Create the world that physics is active */
	bool CreateBox2dWorld();

	/** Set the player's body which is used for mouse joint */
	void MakePlayerBody();

	/** main game loop
	1. draw the line between character and touch point
	2. take 1 step in physics space
	3. move the character and cloud
	*/
    void GameLoop(float dt);

	/** check if there is something on touch point */
	b2Body* GetBodyAtTab(CCPoint p);

	/** reset the game loop */
	void Reset();

	/** Animate the character's wing */
	CCSprite* SetWingAction();

	inline void SetMainLoop()
	{
		// Set the schedule for main game loop
		this->schedule(schedule_selector(MainGameScene::GameLoop));
	}

	/** Make the "how to play" sprite move down */
	void MoveHowToPlaySpriteDown(float length);

	inline bool GetIsDragging() { return _mOnDrag; }

private:
	/** Set player to die */
	void _SetPlayerDie();

private:

	CCSize			_mWinSize;

	// Box2D 

	/** Box2D World */
	b2World*		_mWorld;
	
	/** For Mouse Joint */
	bool			_mOnDrag;
	b2Body*			_mDragBody;
	b2Body*			_mJointBody;
	b2MouseJoint*	_mMouseJoint;

	/** Character body */
	b2Body*			_mPlayerBody;

	/** BatchNode for character image and wings */
	CCSpriteBatchNode* _mPlayerBatchNode;

	/** Previous difference between character's body Y position and flying line */
	float			_mOldDiff;

	/** Value accumulated difference between oldDiff and new one */
	float			_mAccumulatedDiff;

	/** Making and Deleting the clouds */
	CCloudManager	_mCloudManager;

	/** Managing the record */
	CRecordManager	_mRecordManager;

	/** Layer for main and try again */
	static CMenuLayer*		_mMenuLayer;

	/** Layer to display the record */
	static CRecordLayer*	_mRecordLayer;

	/** Draw the line between character and touch point */
	CCDrawNode*				_mLine;

	/** Store the touch point when dragging */
	CCPoint					_mDragTouch;

	/** Sprite - How to play */
	CCSprite*				_mSpriteHowToPlay;

	/** If player has been died or not */
	bool					_mPlayerDie;
};

#endif 
