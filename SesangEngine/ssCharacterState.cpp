#include "ssCharacterState.h"
#include "ssConstantBuffer.h"
#include "ssRenderer.h"
#include "ssGameState.h"


namespace ss
{
	CharacterState::CharacterState()
		: mChanged(false)
	{
		mStateType.mBarState = eBarState::Damaged;
	}
	CharacterState::~CharacterState()
	{
	}
	void CharacterState::Initialize()
	{
	
	}
	void CharacterState::Update()
	{

		// 여기엔 그냥 GameState로 bind하는 거만 넣기. 
		// map에 저장했던 이름 쓰면 됨 
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


	void CharacterState::BindConstantBuffer(const std::wstring& _name)
	{
		CharacterState* state = GameState::GetInst().GetState(_name);

		renderer::ProgressbarCB pr {};
		pr.p_currentHP = state->GetCurrentHP();
		pr.p_CurrentSP = state->GetCurrentSP();
		pr.p_MaxHP = state->GetMaxHP();
		pr.p_MaxSP = state->GetMaxSP();
		pr.p_ProgressType = (UINT)state->GetBarType();



		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Progressbar];
		cb->SetData(&pr);
		cb->Bind(eShaderStage::PS);
	}
}