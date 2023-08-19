#include "ssCharacterState.h"
#include "ssConstantBuffer.h"
#include "ssRenderer.h"
#include "ssGameState.h"


namespace ss
{
	CharacterState::CharacterState()
	{
		mStateType.Type = 0;
	}
	CharacterState::~CharacterState()
	{
	}
	void CharacterState::Initialize()
	{
	
	}
	void CharacterState::Update()
	{
		// ���⿣ �׳� GameState�� bind�ϴ� �Ÿ� �ֱ�. 
		// map�� �����ߴ� �̸� ���� �� 
		BindConstantBuffer(L"Player");


		Script::Update();
	}
	void CharacterState::OnCollisionEnter(Collider2D* other)
	{

	}
	void CharacterState::OnCollisionStay(Collider2D* other)
	{

	}
	void CharacterState::OnCollisionExit(Collider2D* other)
	{
	}
	void CharacterState::BindConstantBuffer()
	{
		renderer::ProgressbarCB pr {};
		pr.p_currentHP = GetCurrentHP();
		pr.p_CurrentSP = GetCurrentSP();
		pr.p_MaxHP = GetMaxHP();
		pr.p_MaxSP = GetMaxSP();
		pr.p_ProgressType = GetBarType();


		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Progressbar];
		cb->SetData(&pr);
		cb->Bind(eShaderStage::PS);
	}
	void CharacterState::BindConstantBuffer(const std::wstring& _name)
	{
		CharacterState* state = GameState::GetInst().GetState(_name);

		renderer::ProgressbarCB pr {};
		pr.p_currentHP = state->GetCurrentHP();
		pr.p_CurrentSP = state->GetCurrentSP();
		pr.p_MaxHP = state->GetMaxHP();
		pr.p_MaxSP = state->GetMaxSP();
		pr.p_ProgressType = state->GetBarType();



		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Progressbar];
		cb->SetData(&pr);
		cb->Bind(eShaderStage::PS);
	}
}