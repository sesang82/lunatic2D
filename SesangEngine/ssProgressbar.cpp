#include "ssProgressbar.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"
#include "ssResources.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"
#include "ssSkeletonArcherScript.h"
#include "ssStoneEyeScript.h"
#include "ssSkeletonLizardScript.h"

namespace ss
{
	using namespace ss::graphics;

	Progressbar::Progressbar()
		: UI(eUIType::Progressbar)
		, mWidth(18) // �̹��� ���� ũ�� �� 
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

		if (GetName() != L"Boss_Bar")
		{
			mr->SetMaterial(Resources::Find<Material>(L"MonsterHPBarMtrl"));
			mTransform->SetScale(Vector3(18.f, 3.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�.
			mOwner = GetParent();
		}

		else
		{
			mr->SetMaterial(Resources::Find<Material>(L"BossHPBarMtrl"));
			mTransform->SetScale(Vector3(260.f, 6.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�.
			mWidth = 260.f;
		}


		
	
		UI::Initialize();

	}
	void Progressbar::Update()
	{
	

			mState = mOwner->GetComponent<CharacterState>();
			
			if (nullptr != mState)
			{
			mfHPratio = mState->GetCurrentHP() / mState->GetMaxHP();
			}

		// ü���� ������ ���� ü�¹��� �ʺ� ����
		float CurWidth = mWidth * mfHPratio;

		Vector3 pos = mTransform->GetPosition();


		if (mOwner->GetName() == L"B_WolfObj")
		{
			// ü�¹� ������ ���� 
			mTransform->SetScale(Vector3(CurWidth, 6, 1.f));

			Vector3 Scale = mTransform->GetScale();

			// ü�¹��� ���� �� ��ġ�� ��� (x ��ġ�� ���� ������ �̵�)
			Vector2 leftTop = mTransform->GetWorldLeftTop();

			leftTop.x = -130.f;
			pos.x = leftTop.x + (Scale.x * 0.5f);

			// ü�¹��� ��ġ ����
			mTransform->SetPosition(pos);

		}

		else
		{
			// update�� �� ���� ���� �ٲ𶧸� �ϰ� �Ϸ�������, ������ ������ ���� 
			if (mOwner->GetName() == L"StoneEye")
			{
				// �������ó�� �������� �ϳ��θ� ������, �ִϸ��̼ǿ��ٰ� �浹ü�� ����������
				// �浹ü���ٰ� �ִϸ��̼��� �������. 
				pos.x = -36;

			}

			if (mOwner->GetName() == L"Archer")
			{
				SkeletonArcherScript* ArcherScript = mOwner->GetComponent<SkeletonArcherScript>();

				if (ArcherScript->GetDir() == Vector3(1.0f, 0.f, 0.f))
				{
					pos.x = -5; // offset ũ�� ���� 
				}
				else if (ArcherScript->GetDir() == Vector3(-1.0f, 0.f, 0.f))
				{
					pos.x = 5; // offset ũ�� ���� 
				}

			}

			if (mOwner->GetName() == L"Lizard")
			{
				SkeletonLizardScript* LizardScript = mOwner->GetComponent<SkeletonLizardScript>();
				pos.x = -5;

			}


			if (mOwner->GetName() == L"WoodGolemObj")
			{
				pos.x = -13;

			}

			if (mOwner->GetName() == L"WolfObj")
			{
				pos.x = 8;

			}

			// ü�¹� �����ʿ��� �������� ���̵��� �ϱ� 
			pos.x -= (1 - mfHPratio) * mWidth * 0.5;

			// ü�¹� ������ ���� 
			mTransform->SetScale(Vector3(CurWidth, 3, 1));

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
