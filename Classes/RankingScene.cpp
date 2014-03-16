#include "RankingScene.h"
#include "AppMacros.h"

const char* PLAYER_ID = "user_id";
//const char*	HTTP_URL = "210.118.75.182/seaft/flyhigh/ranking.php";
const char*	HTTP_URL = "http://lotusbiztel.com/seaft/flyhigh/ranking.php";

const int OP_TAG_GET_RANK = 2;
const int OP_TAG_GET_LIMITED_TOP_10 = 3;
const int OP_TAG_GET_UNLIMITED_TOP_10 = 4;
const int OP_TAG_SET_RECORD = 5;

const char* REQUEST_TAG_GET_NEW_ID = "TAG_GET_NEW_ID";
const char* REQUEST_TAG_CHANGE_ID = "TAG_CHANGE_ID";
const char* REQUEST_TAG_GET_RANK = "TAG_GET_RANK";
const char* REQUEST_TAG_GET_TOP_10_LIMITED = "TAG_GET_TOP_10_LIMITED";
const char* REQUEST_TAG_GET_TOP_10_UNLIMITED = "TAG_GET_TOP_10_UNLIMITED";
const char* REQUEST_TAG_SET_RECORD = "TAG_SET_RECORD";

const char* BTN_BACK_SPRITE_PATH = "res/back.png";
const char* BTN_BACK_CLICKED_SPRITE_PATH = "res/back_clicked.png";

void SplitString(const std::string& text, std::vector<std::string>& words);

CRankingModeLayer* CRankingScene::_mRankingModeLayer = 0;

cocos2d::CCScene* CRankingScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	CRankingScene *layer = CRankingScene::create();

	// add layer as a child to scene
	scene->addChild(layer);
	
	// Initialize the menu, record layer.
	_mRankingModeLayer = CRankingModeLayer::create();
	layer->addChild(_mRankingModeLayer);

	// return the scene
	return scene;
}


bool CRankingScene::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}


	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* bkgSprite = CCSprite::create("res/rankboard.png");
	bkgSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
	this->addChild(bkgSprite);

	_mLoadingSprite = CCSprite::create("res/star_loading.png");
	_mLoadingSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
	this->addChild(_mLoadingSprite, 10);
	_mLoadingSprite->setVisible(false);

	// Editbox setting
	_mUserIDEditBox = CCEditBox::create(CCSizeMake(winSize.width*6.3/10, winSize.height * 0.5 / 10), CCScale9Sprite::create("res/rank_ID_box.png"));
	_mUserIDEditBox->setAnchorPoint(ccp(0.5f, 0.5f));
	_mUserIDEditBox->setPosition(ccp(winSize.width*4.2/10, winSize.height * 9.5 / 10));

	_mUserIDEditBox->setFont("fonts/ostrich-regular.ttf", 42);
	_mUserIDEditBox->setFontColor(ccc3(0.0f, 0.0f, 0.0f));

	_mUserIDEditBox->setMaxLength(14);
	_mUserIDEditBox->setReturnType(kKeyboardReturnTypeDone);
	_mUserIDEditBox->setDelegate(this);
	this->addChild(_mUserIDEditBox);

	_mHelpLabel = CCLabelTTF::create("IF YOU WANT TO CHANGE ID, CLICK THE BOX", "fonts/ostrich-regular.ttf", 60);
	_mHelpLabel->setPosition(ccp(winSize.width*1.1/10, winSize.height * 8.8 / 10));
	_mHelpLabel->setAnchorPoint(ccp(0.0f, 0.0f));
	_mHelpLabel->setColor(ccc3(255,0,0));
	this->addChild(_mHelpLabel);

	_mMyRankLabel = CCLabelTTF::create("", "fonts/ostrich-regular.ttf", 45);
	_mMyRankLabel->setPosition(ccp(winSize.width * 4.2 / 10, winSize.height * 7.55 / 10));
	_mMyRankLabel->setAnchorPoint(ccp(0.5f, 0.0f));
	this->addChild(_mMyRankLabel);

	_mMyRecordLabel = CCLabelTTF::create("", "fonts/ostrich-regular.ttf", 45);
	_mMyRecordLabel->setPosition(ccp(winSize.width * 7.2 / 10, winSize.height * 7.55 / 10));
	_mMyRecordLabel->setAnchorPoint(ccp(0.5f, 0.0f));
	this->addChild(_mMyRecordLabel);

	for(int i=0; i<RANK_COUNT; ++i)
	{
		_mTop10Record[i] = CCLabelTTF::create("", "fonts/ostrich-regular.ttf", 45);
		_mTop10Record[i]->setPosition(ccp(winSize.width * 7.65 / 10, winSize.height * 1.45 / 10 + i * 100));
		_mTop10Record[i]->setAnchorPoint(ccp(1.0f, 1.0f));
		this->addChild(_mTop10Record[i]);
	}

	for(int i=0; i<RANK_COUNT; ++i)
	{
		_mTop10ID[i] = CCLabelTTF::create("", "fonts/ostrich-regular.ttf", 45);
		_mTop10ID[i]->setPosition(ccp(winSize.width * 4.2 / 10, winSize.height * 1.45 / 10 + i * 100));
		_mTop10ID[i]->setAnchorPoint(ccp(0.5f, 1.0f));
		this->addChild(_mTop10ID[i]);
	}


	// Make the ONLINE button
	CCMenuItemImage* btnBackSprite = CCMenuItemImage::create(
		BTN_BACK_SPRITE_PATH,
		BTN_BACK_CLICKED_SPRITE_PATH,
		this, 
		menu_selector(CRankingScene::OnClickBackButton) );
	btnBackSprite->setPosition(ccp(winSize.width * 8.7 / 10, winSize.height * 9.5 / 10) );

	CCMenu*	_mMenu = CCMenu::create(btnBackSprite, NULL);
	_mMenu->setPosition(ccp(0, 0));
	this->addChild(_mMenu, 10);
	
	// Initialize the my rank with 0.
	for(int i=0; i<CGameMode::kModeCount; ++i)
	{
		_mMyRank[i] = 0;
	}
	
	// Get Player's ID from User Default
	_GetUserID();

	// Display my record
	DisplayMyRecord(CGameMode::GetInstance()->GetGameMode());
	
	_MakeLoadingSprite();
	_mLoadingTime = 0;

	return true;
}

void CRankingScene::_GetUserID()
{
	_mUserID = CCUserDefault::sharedUserDefault()->getStringForKey(PLAYER_ID);

	// If user don't have user id, then make the user id after check the DB
	if(_mUserID == "")
	{
		// Get the user count from DB
		_GetNewUserID();
	}
	else
	{
		_mUserIDEditBox->setText(_mUserID.c_str());
		_SetMyRecord();
	}

}


void CRankingScene::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
	if( text.length() > 11 || text.length() < 4 )
	{
		_mHelpLabel->setString("ID must be within 5 and 10 characters.");
		_mUserIDEditBox->setText(_mUserID.c_str());
		return;
	}
	else if( _CheckIDEncodedUTF(text) )
	{
		_mHelpLabel->setString("ID must consist of a-z, A-Z or 0-9.");
		_mUserIDEditBox->setText(_mUserID.c_str());
		return;
	}

	_mHelpLabel->setString("ID is available.");
	_mUserIDEditBox->setText(_mUserID.c_str());
	_ChangeUserID(text.c_str());
}

void CRankingScene::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
	CCLog("EditBox Did return");
}

void CRankingScene::_GetNewUserID()
{
	char postData[64] = {};
	sprintf(postData, "tag=0&id=0\0");
	_SendRequest(postData, REQUEST_TAG_GET_NEW_ID);
}

void CRankingScene::_ChangeUserID(const char* id)
{
	char postData[64] = {};
	sprintf(postData, "tag=1&id=%s&origin=%s\0", id, _mUserID.c_str());
	_SendRequest(postData, REQUEST_TAG_CHANGE_ID);
}

void CRankingScene::GetMyRank(CGameMode::E_GameMode gameMode)
{
	// If rank data have already been exist, then dont make the request.
	if( _mMyRank[gameMode] != 0 )
	{
		char tmp[64] = "";
		sprintf(tmp, "%d", _mMyRank[gameMode]);
		_mMyRankLabel->setString(tmp);

		return;
	}
	char postData[64] = {};
	sprintf(postData, "tag=%d&id=%s\0", OP_TAG_GET_RANK, _mUserID.c_str());
	_SendRequest(postData, REQUEST_TAG_GET_RANK);
}

void CRankingScene::GetLimitedTop10()
{
	char postData[64] = {};
	sprintf(postData, "tag=%d&id=%s\0", OP_TAG_GET_LIMITED_TOP_10, _mUserID.c_str());
	_SendRequest(postData, REQUEST_TAG_GET_TOP_10_LIMITED);
}

void CRankingScene::GetUnlimitedTop10()
{
	char postData[64] = {};
	sprintf(postData, "tag=%d&id=%s\0", OP_TAG_GET_UNLIMITED_TOP_10, _mUserID.c_str());
	_SendRequest(postData, REQUEST_TAG_GET_TOP_10_UNLIMITED);
}


void CRankingScene::_SetMyRecord()
{
	char postData[64] = {};

	sprintf(postData, "tag=%d&id=%s&unlimit=%d&limit=%d", 
		OP_TAG_SET_RECORD, 
		_mUserID.c_str(),
		CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DEFAULT_UNLIMITED_BEST_RECORD),
		CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DEFAULT_LIMITED_BEST_RECORD)
		);
	_SendRequest(postData, REQUEST_TAG_SET_RECORD);
}


void CRankingScene::_OnHttpRequestCompleted(CCNode* pSender, void *data)
{
	CCHttpResponse* pResponse = (CCHttpResponse*)data;
	if( !pResponse )
		return;

	if( strlen(pResponse->getHttpRequest()->getTag() ) != 0 )
		CCLog("%s completed", pResponse->getHttpRequest()->getTag());

	int statusCode = pResponse->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, pResponse->getHttpRequest()->getTag());
	CCLog("response code:%d", statusCode);

	if( !pResponse->isSucceed() ){
		CCLog("response failed");
		CCLog("error buffer:%s", pResponse->getErrorBuffer());
		return;
	}

	std::vector<char>* pBuffer = pResponse->getResponseData();

	char receiveData[1024] = {};	
	for( unsigned int i=0; i<pBuffer->size(); i++){
		receiveData[i] = (*pBuffer)[i];
	}
	receiveData[ pBuffer->size()] = '\0';
	CCLog("Receive Data : %s", receiveData);

	//memmove(receiveData, receiveData + sizeof("\r\n"), sizeof(receiveData)+sizeof("\r\n"));

	_SetResult(receiveData, pResponse->getHttpRequest()->getTag());

}

void CRankingScene::_SendRequest(const char* data, const char* tag)
{
	CCHttpRequest*	pRequest = new CCHttpRequest();
	pRequest->setUrl(HTTP_URL);
	pRequest->setRequestType(CCHttpRequest::kHttpPost);
	pRequest->setResponseCallback(this, callfuncND_selector(CRankingScene::_OnHttpRequestCompleted));

	pRequest->setRequestData(data, strlen( data ));
	pRequest->setTag( tag );
	CCHttpClient::getInstance()->send(pRequest);
	pRequest->release();
}

void CRankingScene::_SetResult(const char* data, const char *tag)
{
	if( strcmp(tag, REQUEST_TAG_GET_NEW_ID) == 0 )
	{
		std::string id(data);	
		_mUserID = id;
		_mUserIDEditBox->setText(_mUserID.c_str());

		CCUserDefault::sharedUserDefault()->setStringForKey(PLAYER_ID, _mUserID);
		
		_SetMyRecord();
	}
	else if( strcmp(tag, REQUEST_TAG_CHANGE_ID) == 0 )
	{
		if( strcmp(data, "EXIST") == 0 )
		{
			_mHelpLabel->setString("already exists");
		}
		else
		{
			std::string id(data);
			_mUserID = id;

			CCUserDefault::sharedUserDefault()->setStringForKey(PLAYER_ID, _mUserID);

			switch(CGameMode::GetInstance()->GetGameMode())
			{
			case CGameMode::kModeLimited:
				GetLimitedTop10();
				break;

			case CGameMode::kModeUnLimited:
				GetUnlimitedTop10();
				break;
			}
		}

		_mUserIDEditBox->setText(_mUserID.c_str());
	}
	// Get my ranks.
	else if( strcmp(tag, REQUEST_TAG_GET_RANK) == 0 )
	{
		std::string result(data);
		
		std::vector<std::string> v;
		SplitString(result, v);

		for(int i=0; i<CGameMode::kModeCount; ++i)
		{
			_mMyRank[i] = atoi(v[i].c_str()) + 1;
		}

		char tmp[64] = "";
		sprintf(tmp, "%d", _mMyRank[CGameMode::GetInstance()->GetGameMode()]);
		_mMyRankLabel->setString(tmp);
	}
	else if( strcmp(tag, REQUEST_TAG_GET_TOP_10_LIMITED) == 0 )
	{
		std::string result(data);
		std::vector<std::string> v;
		SplitString(result, v);

		char resTmp[640] = "";
		for(int i=RANK_COUNT-1, j=0; i>=0; --i, j+=2)
		{
			char tmp[64] = "";
			sprintf(tmp,"%s", v[j].c_str());
			_mTop10ID[i]->setString(tmp);

			sprintf(tmp,"%dM %02dCM", atoi(v[j+1].c_str())/100, atoi(v[j+1].c_str())%100);
			_mTop10Record[i]->setString(tmp);
		}

		_StopLoading();
	}
	else if( strcmp(tag, REQUEST_TAG_GET_TOP_10_UNLIMITED) == 0 )
	{
		std::string result(data);
		std::vector<std::string> v;
		SplitString(result, v);

		char resTmp[640] = "";
		for(int i=RANK_COUNT-1, j=0; i>=0; --i, j+=2)
		{
			char tmp[64] = "";
			sprintf(tmp,"%s", v[j].c_str());
			_mTop10ID[i]->setString(tmp);

			sprintf(tmp,"%dM %02dCM", atoi(v[j+1].c_str())/100, atoi(v[j+1].c_str())%100);
			_mTop10Record[i]->setString(tmp);
		}

		_StopLoading();
	}
	// Set my record and get my ranking
	else if( strcmp(tag, REQUEST_TAG_SET_RECORD) == 0 )
	{
		std::string result(data);

		std::vector<std::string> v;
		SplitString(result, v);

		for(int i=0; i<CGameMode::kModeCount; ++i)
		{
			_mMyRank[i] = atoi(v[i].c_str()) + 1;
		}

		char tmp[64] = "";
		sprintf(tmp, "%d", _mMyRank[CGameMode::GetInstance()->GetGameMode()]);
		_mMyRankLabel->setString(tmp);


		switch(CGameMode::GetInstance()->GetGameMode())
		{
		case CGameMode::kModeLimited:
			GetLimitedTop10();
			break;

		case CGameMode::kModeUnLimited:
			GetUnlimitedTop10();
			break;
		}
	}
}

void CRankingScene::DisplayMyRecord(CGameMode::E_GameMode gameMode)
{
	int record;
	switch( gameMode )
	{
	case CGameMode::kModeLimited:
		record = CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DEFAULT_LIMITED_BEST_RECORD);
		break;

	case CGameMode::kModeUnLimited:
		record = CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DEFAULT_UNLIMITED_BEST_RECORD);
		break;
	}

	char tmp[64] = "";
	sprintf(tmp,"%dM %02dCM", record/100,record%100);
	_mMyRecordLabel->setString(tmp);
}

bool CRankingScene::_CheckIDEncodedUTF(std::string text)
{
	for(int i=0; i<text.length(); i++)
	{
		if( text[i] >= 48 && text[i] <= 57 )
			continue;
		else if( text[i] >= 65 && text[i] <= 90 )
			continue;
		else if( text[i] >= 97 && text[i] <= 122 )
			continue;
		else 
			return true;
	}
	return false;
}

void CRankingScene::OnClickBackButton(CCObject* pSender)
{
	_StopLoading();

	CCDirector::sharedDirector()->popScene();
}

void CRankingScene::_MakeLoadingSprite()
{
	_mLoadingSprite->setVisible(true);
	
	CCActionInterval* rotateAction = CCRotateBy::create(1.0f, 180.0f);
	CCFiniteTimeAction* finiteAction = CCSequence::create(rotateAction, NULL);
	CCActionInterval* repeat = (CCActionInterval*)finiteAction;
	CCAction* action =  CCRepeatForever::create(repeat);

	_mLoadingSprite->runAction(action);

	this->schedule(schedule_selector(CRankingScene::_RotateLoop));
}

void CRankingScene::_RotateLoop(float dt)
{
	_mLoadingTime += dt;
	if( _mLoadingTime > 5.0f )
	{
		_StopLoading();
		_mHelpLabel->setString("Network status is not good, Please try again later");
	}

	CCLog("%f\n", _mLoadingTime);
}

void CRankingScene::_StopLoading()
{
	_mLoadingSprite->pauseSchedulerAndActions();
	this->unschedule(schedule_selector(CRankingScene::_RotateLoop));
	_mLoadingSprite->setVisible(false);
}


void SplitString(const std::string& text, std::vector<std::string>& words) 
{
	int n = text.length();
	int start, stop;
	
	std::string separators("+");
	start = text.find_first_not_of(separators);
	while ((start >= 0) && (start < n)) {
		stop = text.find_first_of(separators, start);
		if ((stop < 0) || (stop > n))
			stop = n;
		words.push_back(text.substr(start, stop - start));
		start = text.find_first_not_of(separators, stop+1);
	}
}