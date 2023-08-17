#pragma once
#include "ssComponent.h"


using namespace ss::math;

namespace ss
{
	class Rigidbody2D : public Component
	{
	public:
		Rigidbody2D();
		~Rigidbody2D();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
		float mMass;
		Vector2 mForce;
		Vector2 mAccelation;
		Vector2 mVelocity;
		Vector2 mLimitedVelocity;

		Vector2 mGravity;
		float mFriction;
		bool mbGround;

	public:
		void SetMass(float mass) { mMass = mass; }
		void AddForce(Vector2 force);
		void SetGround(bool ground) { mbGround = ground; }
		void SetVelocity(Vector2 velocity) { mVelocity = velocity; }

		Vector2 GetVelocity() { return mVelocity; }
		Vector2 GetGravity() { return mGravity; }
		bool	IsGround() { return mbGround; }



    };
}

