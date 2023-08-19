#include "ssPlayerHP.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssMaterial.h"
#include "ssMeshRenderer.h"
#include "ssRenderer.h"

namespace ss
{
	PlayerHP::PlayerHP()
	{
	}
	PlayerHP::~PlayerHP()
	{
	}
	void PlayerHP::Initialize()
	{
		mMeshRenderer = AddComponent<MeshRenderer>();
		mMeshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mMeshRenderer->SetMaterial(Resources::Find<Material>(L"PlayerHPBarMtrl"));

	
		/*mTransform ->SetScale(Vector3(99.f, 8.f, 1.f));
		mTransform ->SetPosition(Vector3(103.f, -205.f, 100.f));*/




		Progressbar::Initialize();
	}

	void PlayerHP::Update()
	{
		Progressbar::Update();
	}

	void PlayerHP::LateUpdate()
	{
		Progressbar::LateUpdate();
	}

	void PlayerHP::Render()
	{
		Progressbar::Render();
	}


	void PlayerHP::BindConstantBuffer()
	{
		renderer::ProgressbarCB pr {};
		/*pr.p_currentHP = state->GetCurrentHP();
		pr.p_CurrentSP = state->GetCurrentSP();
		pr.p_MaxHP = state->GetMaxHP();
		pr.p_MaxSP = state->GetMaxSP();
		pr.p_ProgressType = state->GetBarType();*/



		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Progressbar];
		cb->SetData(&pr);
		cb->Bind(eShaderStage::PS);




	}
}