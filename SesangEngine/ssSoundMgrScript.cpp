#include "ssSoundMgrScript.h"

namespace ss
{

	SoundMgrScript::SoundMgrScript()
		: mListener(nullptr)
		, mBGM(nullptr)
		, mSFX(nullptr)
	{
		SetName(L"SoundMgrScript");
	}

	SoundMgrScript::~SoundMgrScript()
	{
	}

}