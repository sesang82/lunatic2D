#include "ssProgressbar.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"
#include "ssResources.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"

namespace ss
{
	using namespace ss::graphics;

	Progressbar::Progressbar()
		: UI(eUIType::Progressbar)
		, mWidth(18) // �̹��� ���� ũ�� �� 
		, mOffset(0.f)
	{
	}
	Progressbar::~Progressbar()
	{
	}
	void Progressbar::Initialize()
	{


		mTransform = GetComponent<Transform>();
		mTransform->SetScale(Vector3(18.f, 3.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�. 


		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"MonsterHPBarMtrl")); 


		mOwner = GetParent();
		mState = mOwner->GetComponent<CharacterState>();

		UI::Initialize();

	}
	void Progressbar::Update()
	{
	
		float HPratio = mState->GetCurrentHP() / mState->GetMaxHP();

		float test2 = mState->GetCurrentHP();

		// ü���� ������ ���� ü�¹��� �ʺ� ����
		float CurWidth = mWidth * HPratio;


		//// �ٲ𶧸� ������ �ϵ��� ���� (Update�̹Ƿ� ���ɿ� ���� �� �� �����Ƿ�)
		if (mState->Getchanged())
		{
			float test = mState->GetCurrentHP();


			Vector3 pos = mTransform->GetPosition();
			pos.x = mOffset; // offset ũ�� ���� 

			// ü�¹� �����ʿ��� �������� ���̵��� �ϱ� 
			pos.x -= (1 - HPratio) * mWidth * 0.5;

			// ü�¹��� ��ġ ����
			mTransform->SetPosition(Vector3(pos));

			// ü�¹� ������ ���� 
			mTransform->SetScale(Vector3(CurWidth, 3, 1));



			mState->SetChanged(false);
		

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
