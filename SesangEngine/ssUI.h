#pragma once
#include "ssGameObject.h"

namespace ss
{
    class UI :
        public GameObject
    {
	public:

	public:
		UI();
		virtual ~UI() override;

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	


	protected:
		class Animator*			mAnimator;
		class Rigidbody2D*		mRigidbody;
		class Transform*		mTransform;
		class Collider2D*		mCollider;
		class MeshRenderer*		mMeshRenderer;
		class CharacterState*	mCharacterState;



	private:
		class Monster* mBoss;

	public:
		void SetBoss(Monster* boss) { mBoss = boss; }

	};
  
}

