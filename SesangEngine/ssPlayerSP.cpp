#include "ssPlayerSP.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssMaterial.h"
#include "ssMeshRenderer.h"
#include "ssRenderer.h"

namespace ss
{
	PlayerSP::PlayerSP()
	{
	}
	PlayerSP::~PlayerSP()
	{
	}
	void PlayerSP::Initialize()
	{
		/*mMeshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mMeshRenderer->SetMaterial(Resources::Find<Material>(L"PlayerSPBarMtrl"));

		mTransform->SetScale(Vector3(99.f, 8.f, 1.f));
		mTransform->SetPosition(Vector3(103.f, -205.f, 100.f));*/


		Progressbar::Initialize();
	}
	void PlayerSP::Update()
	{
	


		Progressbar::Update();
	}
	void PlayerSP::LateUpdate()
	{
		Progressbar::LateUpdate();
	}
	void PlayerSP::Render()
	{
		Progressbar::Render();
	}
	void PlayerSP::BindConstantBuffer()
	{
		renderer::ProgressbarCB pr {};
	/*	pr.p_currentHP = state->GetCurrentHP();
		pr.p_CurrentSP = state->GetCurrentSP();
		pr.p_MaxHP = state->GetMaxHP();
		pr.p_MaxSP = state->GetMaxSP();
		pr.p_ProgressType = state->GetBarType();*/



		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Progressbar];
		cb->SetData(&pr);
		cb->Bind(eShaderStage::PS);




	}
}