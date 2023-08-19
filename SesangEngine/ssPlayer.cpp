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
	{
	}
	Player::~Player()
	{
	}
	void Player::Initialize()
	{
		


		

		mTransform = GetComponent<Transform>();
		mTransform->SetScale(Vector3(107.f, 58.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�. 

		mCol = AddComponent<Collider2D>();
		mCol->SetName(L"colHit_player");
		mCol->SetType(eColliderType::Rect);
		mCol->SetSize(Vector2(0.2f, 0.8f));
		mCol->SetCenter(Vector2(-3.5f, 2.f));


		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

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

		// ======== bow
		std::shared_ptr<Texture> Image22 = Resources::Find<Texture>(L"B_PlayerIdle");
		std::shared_ptr<Texture> Image23 = Resources::Find<Texture>(L"B_PlayerAttack");	
		std::shared_ptr<Texture> Image24 = Resources::Find<Texture>(L"B_PlayerDash");
		std::shared_ptr<Texture> Image25 = Resources::Find<Texture>(L"B_PlayerDie");
		std::shared_ptr<Texture> Image26 = Resources::Find<Texture>(L"B_PlayerRun");



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
		anim->Create(L"Player_D_Spawn", Image1, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 12, Vector2(107.f, 46.f));

		anim->Create(L"Player_D_IdleR", Image2, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 6, Vector2(107.f, 46.f), Vector2(0.f, -15.f));
		anim->Create(L"Player_D_IdleL", Image2, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 6, Vector2(107.f, 46.f), Vector2(0.f, -15.f), 0.1f, true);

		anim->Create(L"Player_D_RunR", Image3, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 8, Vector2(107.f, 46.f), Vector2(0.f, -15.f));
		anim->Create(L"Player_D_RunL", Image3, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 8, Vector2(107.f, 46.f), Vector2(0.f, -15.f), 0.1f, true);

		anim->Create(L"Player_D_JumpR", Image4, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 14, Vector2(107.f, 46.f), Vector2(0.f, -15.f), 0.07f);
		anim->Create(L"Player_D_JumpL", Image4, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 14, Vector2(107.f, 46.f), Vector2(0.f, -15.f), 0.07f, true);


		// fall �ִϸ��̼� 
		anim->Create(L"Player_D_FallR", Image19, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 4, Vector2(107.f, 46.f), Vector2(0.f, -15.f));
		anim->Create(L"Player_D_FallL", Image19, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 4, Vector2(107.f, 46.f), Vector2(0.f, -15.f), 0.1f, true);


		anim->Create(L"Player_D_LandingR", Image5, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 8, Vector2(107.f, 46.f), Vector2(0.f, -15.f));
		anim->Create(L"Player_D_LandingL", Image5, Vector2(0.f, 0.f), Vector2(88.f, 75.f), 8, Vector2(107.f, 46.f), Vector2(0.f, -15.f), 0.1f, true);



		// ===========
		// 2. sword (������ -12�� ���� �� ����) // �Ϲ� ���ݱ����� ������ ����� (sp ���ݽ� �Ƹ� ������ �޶�����) 
		anim->Create(L"Player_S_IdleR", Image6, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 6, Vector2(107.f, 58.f), Vector2(0.f, -7.f), 0.1f);
		anim->Create(L"Player_S_IdleL", Image6, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 6, Vector2(107.f, 58.f), Vector2(0.f, -7.f), 0.1f, true);


		// �޸��� (���� �Ʒ������� �� ������) 
		anim->Create(L"Player_S_RunR", Image7, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 5, Vector2(107.f, 58.f), Vector2(0.f, -8.f), 0.1f);
		anim->Create(L"Player_S_RunL", Image7, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 5, Vector2(107.f, 58.f), Vector2(0.f, -8.f), 0.1f, true);

		// �Ϲ� ����
		anim->Create(L"Player_S_Attack1R", Image17, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(107.f, 58.f), Vector2(0.f, -2.f), 0.09f);
		anim->Create(L"Player_S_Attack1L", Image17, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(107.f, 58.f), Vector2(0.f, -2.f), 0.09f, true);

		anim->Create(L"Player_S_Attack2R", Image20, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(107.f, 58.f), Vector2(0.f, -2.f), 0.09f);
		anim->Create(L"Player_S_Attack2L", Image20, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(107.f, 58.f), Vector2(0.f, -2.f), 0.09f, true);

		anim->Create(L"Player_S_Attack3R", Image21, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(107.f, 58.f), Vector2(0.f, -2.f), 0.09f);
		anim->Create(L"Player_S_Attack3L", Image21, Vector2(0.f, 0.f), Vector2(107.f, 46.f), 5, Vector2(107.f, 58.f), Vector2(0.f, -2.f), 0.09f, true);



		// sp ����
		anim->Create(L"Player_S_spAttackR", Image18, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 7, Vector2(107.f, 58.f), Vector2(0.f, -6.f));
		anim->Create(L"Player_S_spAttackL", Image18, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 7, Vector2(107.f, 58.f), Vector2(0.f, -6.f), 0.1f, true);

		// ����
		anim->Create(L"Player_S_JumpR", Image8, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 10, Vector2(107.f, 58.f), Vector2(0.f, -6.f), 0.07f);
		anim->Create(L"Player_S_JumpL", Image8, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 10, Vector2(107.f, 58.f), Vector2(0.f, -6.f), 0.07f, true);


		// �뽬
		anim->Create(L"Player_S_DashR", Image12, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 3, Vector2(107.f, 58.f), Vector2(0.f, -6.f));
		anim->Create(L"Player_S_DashL", Image12, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 3, Vector2(107.f, 58.f), Vector2(0.f, -6.f), 0.1f, true);

		// ������ (falling)
		anim->Create(L"Player_S_FallR", Image9, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 6, Vector2(107.f, 46.f), Vector2(0.f, -6.f));
		anim->Create(L"Player_S_FallL", Image9, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 6, Vector2(107.f, 46.f), Vector2(0.f, -6.f), 0.1f, true);

		// Landind (����) 
		anim->Create(L"Player_S_LandingR", Image10, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(107.f, 46.f), Vector2(0.f, -6.f));
		anim->Create(L"Player_S_LandingL", Image10, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(107.f, 46.f), Vector2(0.f, -6.f), 0.1f, true);

		// �Ŵ޸���
		anim->Create(L"Player_S_HangR", Image11, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(107.f, 46.f), Vector2(0.f, -6.f));
		anim->Create(L"Player_S_HangL", Image11, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 4, Vector2(107.f, 46.f), Vector2(0.f, -6.f), 0.1f, true);

		// ����
		anim->Create(L"Player_S_StunR", Image13, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(107.f, 46.f), Vector2(0.f, -6.f));
		anim->Create(L"Player_S_StunL", Image13, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 8, Vector2(107.f, 46.f), Vector2(0.f, -6.f), 0.1f, true);

		// �¾��� �� 
		anim->Create(L"Player_S_HitR", Image14, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(107.f, 46.f), Vector2(0.f, -6.f));
		anim->Create(L"Player_S_HitL", Image14, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(107.f, 46.f), Vector2(0.f, -6.f), 0.1f, true);

		// ���� 
		anim->Create(L"Player_S_GuardR", Image16, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(107.f, 46.f), Vector2(0.f, -6.f));
		anim->Create(L"Player_S_GuardL", Image16, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 1, Vector2(107.f, 46.f), Vector2(0.f, -6.f), 0.1f, true);



		// ����
		anim->Create(L"Player_S_DieR", Image15, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 18, Vector2(107.f, 46.f), Vector2(0.f, -6.f));
		anim->Create(L"Player_S_DieL", Image15, Vector2(0.f, 0.f), Vector2(105.f, 57.f), 18, Vector2(107.f, 46.f), Vector2(0.f, -6.f), 0.1f, true);



		// =========================
		// 3. Bow
		anim->Create(L"Player_B_IdleR", Image22, Vector2(0.f, 0.f), Vector2(43.f, 41.f), 6, Vector2(107.f, 46.f), Vector2(0.f, 2.f));
		anim->Create(L"Player_B_IdleL", Image22, Vector2(0.f, 0.f), Vector2(43.f, 41.f), 6, Vector2(107.f, 46.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_B_AttackR", Image23, Vector2(0.f, 0.f), Vector2(45.f, 46.f), 15, Vector2(107.f, 46.f), Vector2(0.f, 2.f));
		anim->Create(L"Player_B_AttackL", Image23, Vector2(0.f, 0.f), Vector2(45.f, 46.f), 15, Vector2(107.f, 46.f), Vector2(0.f, 2.f), 0.1f, true);


		anim->Create(L"Player_B_DashR", Image24, Vector2(0.f, 0.f), Vector2(49.f, 39.f), 3, Vector2(107.f, 46.f), Vector2(0.f, 2.f));
		anim->Create(L"Player_B_DashL", Image24, Vector2(0.f, 0.f), Vector2(49.f, 39.f), 3, Vector2(107.f, 46.f), Vector2(0.f, 2.f), 0.1f, true);

		anim->Create(L"Player_B_DieR", Image25, Vector2(0.f, 0.f), Vector2(45.f, 47.f), 18, Vector2(107.f, 46.f), Vector2(0.f, 2.f));
		anim->Create(L"Player_B_DieL", Image25, Vector2(0.f, 0.f), Vector2(45.f, 47.f), 18, Vector2(107.f, 46.f), Vector2(0.f, 2.f), 0.6f, true);

		anim->Create(L"Player_B_RunR", Image26, Vector2(0.f, 0.f), Vector2(43.f, 40.f), 8, Vector2(107.f, 46.f), Vector2(0.f, 2.f));
		anim->Create(L"Player_B_RunL", Image26, Vector2(0.f, 0.f), Vector2(43.f, 40.f), 8, Vector2(107.f, 46.f), Vector2(0.f, 2.f), 0.1f, true);


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