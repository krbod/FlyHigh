
#ifndef __GAME_MODE_H__
#define __GAME_MODE_H__

class CGameMode
{
public:
	enum E_GameMode
	{
		kModeUnLimited,
		kModeLimited,
		kModeCount = 2,
	};

	static CGameMode* GetInstance()
	{
		if( _mInstance == nullptr )
		{
			_mInstance = new CGameMode;
		}
		return _mInstance;
	}

	inline void SetGameMode(E_GameMode mode) { _mGameMode = mode; }
	inline E_GameMode GetGameMode() { return _mGameMode; }

private:
	static CGameMode*	_mInstance;

	E_GameMode			_mGameMode;

};

#endif