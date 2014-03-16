
#ifndef __RANKING_SCENE_H__
#define __RANKING_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "GameMode.h"
#include "RankingModeLayer.h"

using namespace cocos2d::extension;

USING_NS_CC;

const int RANK_COUNT = 10;


class CRankingScene : public CCLayer,
	public cocos2d::extension::CCEditBoxDelegate
{
public:
	virtual bool init();  		
	static cocos2d::CCScene* scene();

	CREATE_FUNC(CRankingScene);

	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);

	void GetMyRank(CGameMode::E_GameMode gameMode);

	void GetLimitedTop10();
	void GetUnlimitedTop10();

	void DisplayMyRecord(CGameMode::E_GameMode gameMode);
	
	void OnClickBackButton(CCObject* pSender);

private:
	void _GetUserID();
	void _GetNewUserID();

	void _ChangeUserID(const char* id);

	void _OnHttpRequestCompleted( CCNode* pSender, void *data );

	void _SendRequest(const char* data, const char* tag);

	void _SetResult(const char* data, const char *tag);

	void _SetMyRecord();

	bool _CheckIDEncodedUTF(std::string text);

	void _MakeLoadingSprite();
	void _RotateLoop(float dt);

	void _StopLoading();

	
private:
	cocos2d::extension::CCEditBox* _mUserIDEditBox;
	CCLabelTTF*	_mHelpLabel;

	std::string _mUserID;

	/** Player's rank */
	CCLabelTTF*	_mMyRankLabel;

	/** Player's record */
	CCLabelTTF* _mMyRecordLabel;

	/** Top 10 of limited mode */
	CCLabelTTF*	_mTop10Record[RANK_COUNT];
	CCLabelTTF* _mTop10ID[RANK_COUNT];

	int			_mMyRank[CGameMode::kModeCount];

	/** loading sprite */
	CCSprite*	_mLoadingSprite;

	/** Layer for main and try again */
	static CRankingModeLayer*	_mRankingModeLayer;

	float			_mLoadingTime;
	
};


#endif

