#include "ssPlayer.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssTransform.h"
#include "ssInput.h"
#include "ssTime.h"
#include "ssCollider2D.h"
#include "ssAnimator.h"
#include "ssMaterial.h"
#include "ssPlayerScript.h"
#include "ssRigidbody2D.h"
#include "ssCharacterState.h"
#include "ssGameState.h"
#include "ssSceneManager.h"
#include "ssScene.h"

namespace ss
{
	Player::Player()
		: Character(eType::PLAYER)		
		, mAttackCol(nullptr)
		, mEnableRender(true)
	{
	}
	Player::~Player()
	{
	}
	void Player::Initialize()
	{
		


		

		mTransform = GetComponent<Transform>();
		mTransform->SetScale(Vector3(240.f, 50.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�. 

		mCol = AddComponent<Collider2D>();
		mCol->SetName(L"colHit_player");
		mCol->SetType(eColliderType::Rect);
		mCol->SetSize(Vector2(0.3f, 0.8f));
		mCol->SetCenter(Vector2(-6.f, 2.f));


		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"PlayerAnimMtrl"));

	

		// ================
		std::shared_ptr<Texture> Image1= Resources::Find<Texture>(L"D_Spawn");
		std::shared_ptr<Texture> Image2 = Resources::Find<Texture>(L"D_Idle");
		std::shared_ptr<Texture> Image3 = Resources::Find<Texture>(L"D_Run");
		std::shared_ptr<Texture> Image4 = Resources::Find<Texture>(L"D_Jump");
		std::shared_ptr<Texture> Image5 = Resources::Find<Texture>(L"D_Landing");
		std::shared_ptr<Texture> Image19 = Resources::Find<Texture>(L"D_Fall");

		// ======== sword
		std::shared_ptr<Texture> Image6 = Resources::Find<Texture>(L"S_PlayerIdle");
		std::shared_ptr<Texture> Image7 = Resources::Find<Texture>(L"S_PlayerRun");
		std::shared_ptr<Texture> Image8 = Resources::Find<Texture>(L"S_PlayerJump");
		std::shared_ptr<Texture> Image9 = Resources::Find<Texture>(L"S_PlayerFall");
		std::shared_ptr<Texture> Image10 = Resources::Find<Texture>(L"S_PlayerLanding");
		std::shared_ptr<Texture> Image11 = Resources::Find<Texture>(L"S_PlayerHang");
		std::shared_ptr<Texture> Image12 = Resources::Find<Texture>(L"S_PlayerDash");
		std::shared_ptr<Texture> Image13 = Resources::Find<Texture>(L"S_PlayerStun");
		std::shared_ptr<Texture> Image14 = Resources::Find<Texture>(L"S_PlayerHit");
		std::shared_ptr<Texture> Image15 = Resources::Find<Texture>(L"S_PlayerDie");
		std::shared_ptr<Texture> Image16 = Resources::Find<Texture>(L"S_PlayerGuard");
		std::shared_ptr<Texture> Image17 = Resources::Find<Texture>(L"S_PlayerAttack1");
		std::shared_ptr<Texture> Image20 = Resources::Find<Texture>(L"S_PlayerAttack2");
		std::shared_ptr<Texture> Image21 = Resources::Find<Texture>(L"S_PlayerAttack3");
		std::shared_ptr<Texture> Image18 = Resources::Find<Texture>(L"S_PlayerspAttack");
		std::shared_ptr<Texture> Image27 = Resources::Find<Texture>(L"S_PlayerOverloading");

		// ======== ��
		std::shared_ptr<Texture> Image22 = Resources::Find<Texture>(L"P_PlayerIdle");
		std::shared_ptr<Texture> Image23 = Resources::Find<Texture>(L"P_PlayerRun");	
		std::shared_ptr<Texture> Image24 = Resources::Find<Texture>(L"P_PlayerJump");
		std::shared_ptr<Texture> Image25 = Resources::Find<Texture>(L"P_PlayerFall");
		std::shared_ptr<Texture> Image26 = Resources::Find<Texture>(L"P_PlayerDash");
		std::shared_ptr<Texture> Image28 = Resources::Find<Texture>(L"P_PlayerHit");
		std::shared_ptr<Texture> Image29 = Resources::Find<Texture>(L"P_PlayerGuard");
		std::shared_ptr<Texture> Image30 = Resources::Find<Texture>(L"P_PlayerAttack1,2");
		std::shared_ptr<Texture> Image31 = Resources::Find<Texture>(L"P_PlayerAttack3");
		std::shared_ptr<Texture> Image32 = Resources::Find<Texture>(L"P_PlayerspAttack");
		std::shared_ptr<Texture> Image33 = Resources::Find<Texture>(L"P_PlayerOverLoadReady");
		std::shared_ptr<Texture> Image34 = Resources::Find<Texture>(L"P_PlayerOverLoading");
		std::shared_ptr<Texture> Image35 = Resources::Find<Texture>(L"P_PlayerOverLoadEnd");

		// ======== �尩
		std::shared_ptr<Texture> Image36 = Resources::Find<Texture>(L"G_PlayerIdle");
		std::shared_ptr<Texture> Image37 = Resources::Find<Texture>(L"G_PlayerRun");
		std::shared_ptr<Texture> Image38 = Resources::Find<Texture>(L"G_PlayerJump");
		std::shared_ptr<Texture> Image39 = Resources::Find<Texture>(L"G_PlayerFall");
		std::shared_ptr<Texture> Image40 = Resources::Find<Texture>(L"G_PlayerDash");
		std::shared_ptr<Texture> Image41 = Resources::Find<Texture>(L"G_PlayerHit");
		std::shared_ptr<Texture> Image42 = Resources::Find<Texture>(L"G_PlayerGuard");
		std::shared_ptr<Texture> Image43 = Resources::Find<Texture>(L"G_PlayerAttack1");
		std::shared_ptr<Texture> Image44 = Resources::Find<Texture>(L"G_PlayerAttack2");
		std::shared_ptr<Texture> Image45 = Resources::Find<Texture>(L"G_PlayerAttack3");
		std::shared_ptr<Texture> Image46 = Resources::Find<Texture>(L"G_PlayerspAttack");
		std::shared_ptr<Texture> Image47 = Resources::Find<Texture>(L"G_Playeroverload");





		Animator* anim = AddComponent<Animator>();


		// �� �浹ü ũ��� �޶����°� �ִٸ� fsm���� ���� �������ֱ� 
		// === 
			// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
			// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
			//  LT, 1������ �縦 ������, �ڸ� ����, ������, , ����ð� ������
			// === 
			// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
			// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
			//  LT, 1������ �縦 ������, �ڸ� ����, �������, ������
		anim->Create(L"Player_D_Spawn", Image1, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 12, Vector2(240.f, 50.f));

		anim->Create(L"Player_D_IdleR", Image2, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 6, Vector2(240.f, 50.f), Vector2(10.f, 17.f));
		anim->Create(L"Player_D_IdleL", Image2, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 6, Vector2(240.f, 50.f), Vector2(0.f, -17.f), 0.1f, true);

		anim->Create(L"Player_D_RunR", Image3, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 8, Vector2(240.f, 50.f), Vector2(10.f, 17.f));
		anim->Create(L"Player_D_RunL", Image3, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 8, Vector2(240.f, 50.f), Vector2(0.f, -17.f), 0.1f, true);

		anim->Create(L"Player_D_JumpR", Image4, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 14, Vector2(240.f, 50.f), Vector2(10.f, 17.f), 0.07f);
		anim->Create(L"Player_D_JumpL", Image4, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 14, Vector2(240.f, 50.f), Vector2(0.f, -17.f), 0.07f, true);

		

		// fall �ִϸ��̼� 
		anim->Create(L"Player_D_FallR", Image19, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 4, Vector2(240.f, 50.f), Vector2(10.f, 17.f));
		anim->Create(L"Player_D_FallL", Image19, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 4, Vector2(240.f, 50.f), Vector2(0.f, -17.f), 0.1f, true);


		anim->Create(L"Player_D_LandingR", Image5, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 8, Vector2(240.f, 50.f), Vector2(10.f, 17.f));
		anim->Create(L"Player_D_LandingL", Image5, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 8, Vector2(240.f, 50.f), Vector2(0.f, -17.f), 0.1f, true);



		// ===========
		// 2. sword (������ -12�� ���� �� ����) // �Ϲ� ���ݱ����� ������ ����� (sp ���ݽ� �Ƹ� ������ �޶�����) 
		anim->Create(L"Player_S_IdleR", Image6, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 6, Vector2(240.f, 50.f), Vector2(10.f, -7.f), 0.1f);
		anim->Create(L"Player_S_IdleL", Image6, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 6, Vector2(240.f, 50.f), Vector2(0.f, -7.f), 0.1f, true);


		// �޸��� (���� �Ʒ������� �� ������) 
		anim->Create(L"Player_S_RunR", Image7, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 5,Vector2(240.f, 50.f), Vector2(10.f, -8.f), 0.1f);
		anim->Create(L"Player_S_RunL", Image7, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 5,Vector2(240.f, 50.f), Vector2(0.f, -8.f), 0.1f, true);

		// �Ϲ� ����
		anim->Create(L"Player_S_Attack1R", Image17, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(240.f, 50.f), Vector2(10.f, -2.f), 0.09f);
		anim->Create(L"Player_S_Attack1L", Image17, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(240.f, 50.f), Vector2(0.f, -2.f), 0.09f, true);

		anim->Create(L"Player_S_Attack2R", Image20, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(240.f, 50.f), Vector2(10.f, -2.f), 0.09f);
		anim->Create(L"Player_S_Attack2L", Image20, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(240.f, 50.f), Vector2(0.f, -2.f), 0.09f, true);

		anim->Create(L"Player_S_Attack3R", Image21, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(240.f, 50.f), Vector2(10.f, -2.f), 0.09f);
		anim->Create(L"Player_S_Attack3L", Image21, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(240.f, 50.f), Vector2(0.f, -2.f), 0.09f, true);


		// ����
		anim->Create(L"Player_S_JumpR", Image8, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 10, Vector2(240.f, 50.f), Vector2(10.f, -6.f), 0.07f);
		anim->Create(L"Player_S_JumpL", Image8, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 10, Vector2(240.f, 50.f), Vector2(0.f, -6.f), 0.07f, true);


		// �뽬
		anim->Create(L"Player_S_DashR", Image12, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 3,Vector2(240.f, 50.f), Vector2(10.f, -6.f));
		anim->Create(L"Player_S_DashL", Image12, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 3,Vector2(240.f, 50.f), Vector2(0.f, -6.f), 0.1f, true);

		// ������ (falling)
		anim->Create(L"Player_S_FallR", Image9, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(10.f, -6.f));
		anim->Create(L"Player_S_FallL", Image9, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(0.f, -6.f), 0.1f, true);

		// Landind (����) 
		anim->Create(L"Player_S_LandingR", Image10, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(240.f, 50.f), Vector2(10.f, -6.f));
		anim->Create(L"Player_S_LandingL", Image10, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(240.f, 50.f), Vector2(0.f, -6.f), 0.1f, true);

		// �Ŵ޸���
		anim->Create(L"Player_S_HangR", Image11, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(10.f, -6.f));
		anim->Create(L"Player_S_HangL", Image11, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(0.f, -6.f), 0.1f, true);

		// ����
		anim->Create(L"Player_S_StunR", Image13, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(240.f, 50.f), Vector2(10.f, -6.f));
		anim->Create(L"Player_S_StunL", Image13, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(240.f, 50.f), Vector2(0.f, -6.f), 0.1f, true);

		// �¾��� �� 
		anim->Create(L"Player_S_HitR", Image14, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4,Vector2(240.f, 50.f), Vector2(10.f, -6.5f), 0.12f);
		anim->Create(L"Player_S_HitL", Image14, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4,Vector2(240.f, 50.f), Vector2(0.f, -6.5f), 0.12f, true);

		// ���� 
		anim->Create(L"Player_S_GuardR", Image16, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(240.f, 50.f), Vector2(10.f, -6.f), 0.2f);
		anim->Create(L"Player_S_GuardL", Image16, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(240.f, 50.f), Vector2(0.f, -6.f), 0.2f, true);



		// ����
		anim->Create(L"Player_S_DieR", Image15, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 18, Vector2(240.f, 50.f), Vector2(10.f, -6.f));
		anim->Create(L"Player_S_DieL", Image15, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 18, Vector2(240.f, 50.f), Vector2(0.f, -6.f), 0.1f, true);

		// SP Attack
		anim->Create(L"Player_S_spAttackR", Image18, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 7, Vector2(240.f, 50.f), Vector2(0.f, -6.f));
		anim->Create(L"Player_S_spAttackL", Image18, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 7, Vector2(240.f, 50.f), Vector2(0.f, -6.f), 0.1f, true);

		// Overload
		anim->Create(L"Player_S_OverloadingR", Image27, Vector2(0.f, 0.f), Vector2(158.f, 47.f), 10, Vector2(240.f, 50.f), Vector2(39.f, -2.f), 0.06);
		anim->Create(L"Player_S_OverloadingL", Image27, Vector2(0.f, 0.f), Vector2(158.f, 47.f), 10, Vector2(240.f, 50.f), Vector2(26.f, -2.f), 0.06f, true);


		// =========================
		// 3. ��
		anim->Create(L"Player_P_IdleR", Image22, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 6, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_IdleL", Image22, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 6, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_P_RunR", Image23, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_RunL", Image23, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_P_JumpR", Image24, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 11, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_JumpL", Image24, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 11, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);


		anim->Create(L"Player_P_FallR", Image25, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_FallL", Image25, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_P_DashR", Image26, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 3, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_DashL", Image26, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 3, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_P_HitR", Image28, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_HitL", Image28, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_P_GuardR", Image29, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_GuardL", Image29, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);


		anim->Create(L"Player_P_Attack1,2R", Image30, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_Attack1,2L", Image30, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);


		anim->Create(L"Player_P_Attack3R", Image31, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 7, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_Attack3L", Image31, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 7, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_P_spAttackR", Image32, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 15, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_spAttackL", Image32, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 15, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_P_OverLoadReadyR", Image33, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_OverLoadReadyL", Image33, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_P_OverLoadingR", Image34, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 9, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_OverLoadingL", Image34, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 9, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_P_OverLoadEndR", Image35, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_P_OverLoadEndL", Image35, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		// =============================
		// 4. �尩 
		anim->Create(L"Player_G_IdleR", Image36, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 6, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_IdleL", Image36, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 6, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_G_RunR", Image37, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_RunL", Image37, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);


		anim->Create(L"Player_G_JumpR", Image38, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 9, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_JumpL", Image38, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 9, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);


		anim->Create(L"Player_G_FallR", Image39, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_FallL", Image39, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);
											  
		anim->Create(L"Player_G_DashR", Image40, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 3, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_DashL", Image40, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 3, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_G_HitR", Image41, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_HitL", Image41, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_G_GuardR", Image42, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_GuardL", Image42, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_G_Attack1R", Image43, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_Attack1L", Image43, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_G_Attack2R", Image44, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_Attack2L", Image44, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_G_Attack3R", Image45, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 7, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_Attack3L", Image45, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 7, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_G_spAttackR", Image46, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 10, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_spAttackL", Image46, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 10, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_G_overloadR", Image47, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 16, Vector2(240.f, 50.f), Vector2(10.f, 2.f));
		anim->Create(L"Player_G_overloadL", Image47, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 16, Vector2(240.f, 50.f), Vector2(0.f, 2.f), 0.1f, true);



		Rigidbody2D* rigid = AddComponent<Rigidbody2D>();
	

		CharacterState* statue = AddComponent<CharacterState>();


		AddComponent<PlayerScript>();

		


		Character::Initialize();

	}


	void Player::Update()
	{

		Character::Update();
	}
	void Player::LateUpdate()
	{


		Character::LateUpdate();
	}
	void Player::Render()
	{
		Character::Render();
	}
}