#include "ssRigidbody2D.h"
#include "ssTime.h"
#include "ssGameObject.h"

namespace ss
{
	Rigidbody2D::Rigidbody2D()
		: Component(eComponentType::Rigidbody2D)
		, mMass(1.0f)
		, mForce(Vector2::Zero)
		, mAccelation(Vector2::Zero)
		, mVelocity(Vector2::Zero)
	{
		mLimitedVelocity.x = 200.0f;
		mLimitedVelocity.y = 800.0f;
		mbGround = false;
		mGravity = Vector2(0.0f, 1300.0f); // 1500
		mFriction = 50.0f; // 83 ������
	}

	Rigidbody2D::~Rigidbody2D()
	{

	}

	void Rigidbody2D::Initialize()
	{

	}

	void Rigidbody2D::Update()
	{
		// F = M * A
		// A = M / F
		mAccelation = mForce / mMass;

		// �ӵ��� ���ӵ��� �����ش�.
		mVelocity += mAccelation * Time::DeltaTime();

		if (mbGround)
		{
			// ������ ������
			Vector2 gravity = mGravity;
			gravity.Normalize();

			float dot = mVelocity.Dot(gravity);
			mVelocity -= gravity * dot;
		}
		else
		{
			mVelocity += mGravity * Time::DeltaTime();
		}

		// �߷°��ӵ� �ִ� �ӵ� ����
		Vector2 gravity = mGravity;
		gravity.Normalize();
		float dot = mVelocity.Dot(gravity);
		gravity = gravity * dot;

		Vector2 sideVelocity = mVelocity - gravity;
		if (mLimitedVelocity.y < gravity.Length())
		{
			gravity.Normalize();
			gravity *= mLimitedVelocity.y;
		}

		if (mLimitedVelocity.x < sideVelocity.Length())
		{
			sideVelocity.Normalize();
			sideVelocity *= mLimitedVelocity.x;
		}

		// ������ ���� ( ����� ���� ����, �ӵ��� 0�� �ƴ�)
		if (!(mVelocity == Vector2::Zero))
		{
			//�ӵ��� �ݴ�������� �������� ����ȴ�.
			Vector2 friction = -mVelocity;
			//friction = friction.Normalize() * mFriction * mMass * Time::DeltaTime();
			friction.Normalize();
			friction = friction * mFriction * mMass * Time::DeltaTime();

			//���������� ���� �ӵ� ���Ҵ� ���� �ӵ����� ū ���

			if (mVelocity.Length() < friction.Length())
			{
				mVelocity = Vector2::Zero;
			}
			else
			{
				mVelocity += friction;
			}
		}

		// �ӵ��� �°Բ� ��ü�� �̵���Ų��.
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		pos.x = pos.x + mVelocity.x * Time::DeltaTime();
		pos.y = pos.y - mVelocity.y * Time::DeltaTime();

		tr->SetPosition(pos);
		mForce = Vector2::Zero;
	}

	void Rigidbody2D::LateUpdate()
	{
	}

	void Rigidbody2D::Render()
	{
	}

	void Rigidbody2D::AddForce(Vector2 force)
	{
		mForce += force;
	}

}

