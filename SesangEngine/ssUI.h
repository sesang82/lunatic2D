#pragma once
#include "ssGameObject.h"

namespace ss
{
    class UI :
        public GameObject
    {
	public:
		enum class eUIType
		{
			Button,
			Progressbar,
			END,
		};


	public:
		UI(eUIType type);
		virtual ~UI() override;

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	


	protected:
		eUIType mUIType;
		class Animator*			mAnimator;
		class Rigidbody2D*		mRigidbody;
		class Transform*		mTransform;
		class Collider2D*		mCollider;
		class MeshRenderer*		mMeshRenderer;
		class CharacterState*	mCharacterState;

	};
  
}

