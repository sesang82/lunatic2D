#include "ssProgressbar.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"
#include "ssResources.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"
#include "ssSkeletonArcherScript.h"
#include "ssStoneEyeScript.h"
#include "ssSkeletonLizardScript.h"
#include "ssBigWolfScript.h"
#include "ssGameState.h"

namespace ss
{
	using namespace ss::graphics;

	Progressbar::Progressbar()
		: mWidth(0.f) // �̹��� ���� ũ�� �� 
		, mOffset(0.f)
		, mfHPratio(0.f)
	{
	}
	Progressbar::~Progressbar()
	{
	}
	void Progressbar::Initialize()
	{


		mTransform = GetComponent<Transform>();


		MeshRenderer* mr = AddComponent<MeshRenderer>();


		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

		if (GetName() == L"Boss_Bar")
		{
			mr->SetMaterial(Resources::Find<Material>(L"BossHPBarMtrl"));
			mTransform->SetScale(Vector3(260.f, 6.f, 1.f));
			mWidth = 260.f;
		}


		else if (GetName() == L"overloadBar")
		{
			mr->SetMaterial(Resources::Find<Material>(L"PlayerOverloadBarMtrl"));
			mTransform->SetScale(Vector3(48.f, 4.f, 1.f));
			mWidth = 48.f;
		}

		else // �Ϲ� ���� hp�� 
		{
			mr->SetMaterial(Resources::Find<Material>(L"MonsterHPBarMtrl"));
			mTransform->SetScale(Vector3(18.f, 3.f, 1.f));
			mOwner = GetParent();
			mWidth = 18.f;
		}


		
	
		UI::Initialize();

	}
	void Progressbar::Update()
	{
	

			mState = mOwner->GetComponent<CharacterState>();
			
			if (nullptr != mState) // ���Ͱ� dead ������ �� ������Ʈ�� ���� ���� �ʾƼ� �������� ������ ��ó�� �� 
			{
			  mfHPratio = mState->GetCurrentHP() / mState->GetMaxHP();
			}

		// ü���� ������ ���� ü�¹��� �ʺ� ����
		float CurHPWidth = mWidth * mfHPratio;

		Vector3 pos = mTransform->GetPosition();


		if (mOwner->GetName() == L"B_WolfObj")
		{

			// ü�¹� ������ ���� 
			mTransform->SetScale(Vector3(CurHPWidth, 6, 1.f));

			Vector3 Scale = mTransform->GetScale();

			// ü�¹��� ���� �� ��ġ�� ��� (x ��ġ�� ���� ������ �̵�)
			Vector2 leftTop = mTransform->GetWorldLeftTop();

			leftTop.x = -130.f;
			pos.x = leftTop.x + (Scale.x * 0.5f);

			// ü�¹��� ��ġ ����
			mTransform->SetPosition(pos);

		}


		else if (GetName() == L"overloadBar")
		{
			mState = GameState::GetInst().GetState(L"Player");

			float CurOverloadratio = mState->GetCurrentOverload() / mState->GetMaxOverload();

			// ü���� ������ ���� ü�¹��� �ʺ� ����
			float CurOverloadWidth = mWidth * CurOverloadratio;


			// ü�¹� ������ ���� 
			mTransform->SetScale(Vector3(CurOverloadWidth, 4, 1.f));

			Vector3 Scale = mTransform->GetScale();

			// ü�¹��� ���� �� ��ġ�� ��� (x ��ġ�� ���� ������ �̵�)
			Vector2 leftTop = mTransform->GetWorldLeftTop();

			leftTop.x = -24.f;
			pos.x = leftTop.x + (Scale.x * 0.5f);

			// ü�¹��� ��ġ ����
			mTransform->SetPosition(pos);


		}


		else
		{


			// ü�¹� ������ ���� 
			mTransform->SetScale(Vector3(CurHPWidth, 3, 1));

			Vector3 Scale = mTransform->GetScale();

			// ü�¹��� ���� �� ��ġ�� ��� (x ��ġ�� ���� ������ �̵�)
			Vector2 leftTop = mTransform->GetWorldLeftTop();

			
			// update�� �� ���� ���� �ٲ𶧸� �ϰ� �Ϸ�������, ������ ������ ���� 
			if (mOwner->GetName() == L"StoneEye")
			{
				// �������ó�� �������� �ϳ��θ� ������, �ִϸ��̼ǿ��ٰ� �浹ü�� ����������
				// �浹ü���ٰ� �ִϸ��̼��� �������. 
				leftTop.x = -45.f;

			}

			else if (mOwner->GetName() == L"Archer")
			{
				leftTop.x = -15.f;

			}

			else if (mOwner->GetName() == L"Lizard")
			{
				leftTop.x = -25.f;

			}


			else if (mOwner->GetName() == L"WoodGolemObj")
			{
				leftTop.x = -20.f;

			}

			else if (mOwner->GetName() == L"WolfObj")
			{
				leftTop.x = -3.f;

			}

			pos.x = leftTop.x + (Scale.x * 0.5f);

			// ü�¹��� ��ġ ����
			mTransform->SetPosition(pos);

		}

	
			



		UI::Update();

	}


	void Progressbar::LateUpdate()
	{


		UI::LateUpdate();
	}
	void Progressbar::Render()
	{
		UI::Render();
	}
	void Progressbar::BindConstantBuffer()
	{
	
	}
}
