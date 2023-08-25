#pragma once
#include "ssUI.h"

namespace ss
{
	class Progressbar :
		public UI
	{
	public:
		Progressbar();
		virtual ~Progressbar();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void BindConstantBuffer();

	public:
		int mWidth;
		float mOffset;
		class CharacterState* mState;
		class GameObject* mOwner;
		Vector3 mCurScale;
		Vector3 mMaxScale;


	public:
		void SetOwner(GameObject* _owner) { mOwner = _owner; }
		void SetOffset(float _offset) { mOffset = _offset; } // 1ÇÈ¼¿¾¿ Á¶ÀýÇÏ¸é µÊ
	};
}

