#include "ssStoneEye.h"
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
#include "ssStoneEyeScript.h"
#include "ssStoneEyeColScript.h"


namespace ss
{
	StoneEye::StoneEye()
	{
	}
	StoneEye::~StoneEye()
	{
	}
	void StoneEye::Initialize()
	{

		Transform* tr = GetComponent<Transform>();
		tr->SetScale(Vector3(240.f, 123.f, 1.0f)); // back������ �״�� ���� 

		MeshRenderer* mr = GetComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial")); // ������ �ִϸ��̼� �� �Ŷ� �ϰų� �־�θ� �� 

		Animator* ani = GetComponent<Animator>();

		std::shared_ptr<graphics::Texture> Image1 = Resources::Find<graphics::Texture>(L"StoneEyeIdle");
		std::shared_ptr<graphics::Texture> Image2 = Resources::Find<graphics::Texture>(L"StoneEyeHit");
		std::shared_ptr<graphics::Texture> Image3 = Resources::Find<graphics::Texture>(L"StoneEyeStun");
		std::shared_ptr<graphics::Texture> Image4 = Resources::Find<graphics::Texture>(L"StoneEyeDie");
		std::shared_ptr<graphics::Texture> Image5 = Resources::Find<graphics::Texture>(L"StoneEyeNearAttack");
		std::shared_ptr<graphics::Texture> Image6 = Resources::Find<graphics::Texture>(L"StoneEyeBorrow");
		std::shared_ptr<graphics::Texture> Image7 = Resources::Find<graphics::Texture>(L"StoneEyeFarAttack");

		// === 
		// 		// �� �浹ü ũ��� �޶����°� �ִٸ� fsm���� ���� �������ֱ� 
		// === 
		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, ������(y���� -���� Ŭ���� �ö�), ����ð�, ������

		// === 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, �������, ������(y���� -���� Ŭ���� �ö�)
		ani->Create(L"StoneEye_IdleR", Image1, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 7, Vector2(240.f, 123.f), Vector2(0.f, -19.f));
		ani->Create(L"StoneEye_IdleL", Image1, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 7, Vector2(240.f, 123.f), Vector2(70.f, -19.f), 0.1f, true);

		ani->Create(L"StoneEye_HitR", Image2, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 1, Vector2(240.f, 123.f), Vector2(0.f, -20.f), 0.3f);
		ani->Create(L"StoneEye_HitL", Image2, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 1, Vector2(240.f, 123.f), Vector2(70.f, -20.f), 0.3f, true);

		ani->Create(L"StoneEye_StunR", Image3, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 5, Vector2(240.f, 123.f), Vector2(0.f, -19.f));
		ani->Create(L"StoneEye_StunL", Image3, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 5, Vector2(240.f, 123.f), Vector2(70.f, -19.f), 0.1f, true);

		ani->Create(L"StoneEye_DieR", Image4, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 10, Vector2(240.f, 123.f), Vector2(0.f, -19.f));
		ani->Create(L"StoneEye_DieL", Image4, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 10, Vector2(240.f, 123.f), Vector2(70.f, -19.f), 0.1f, true);

		ani->Create(L"StoneEye_NearAttackR", Image5, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 12, Vector2(240.f, 123.f), Vector2(0.f, -18.f));
		ani->Create(L"StoneEye_NearAttackL", Image5, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 12, Vector2(240.f, 123.f), Vector2(70.f, -18.f), 0.1f, true);

		ani->Create(L"StoneEye_Borrow", Image6, Vector2(0.f, 0.f), Vector2(25.f, 10.f), 1, Vector2(25, 10.f), Vector2(0.f, -2.f));


		// 9�����ӿ� �Ѿ� ���� 
		ani->Create(L"StoneEye_FarAttackR", Image7, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 11, Vector2(240.f, 123.f), Vector2(0.f, -19.f), 0.1f);
		ani->Create(L"StoneEye_FarAttackL", Image7, Vector2(0.f, 0.f), Vector2(114.f, 86.f), 11, Vector2(240.f, 123.f), Vector2(70.f, -19.f), 0.1f, true);




		Collider2D* col = GetComponent<Collider2D>();
		col->SetName(L"StoneHitCol");
		col->SetType(eColliderType::Rect);

		col->SetSize(Vector2(0.15f, 0.43f));
		col->SetCenter(Vector2(-35.f, 0.2f));



		AddComponent<StoneEyeScript>();
		AddComponent<CharacterState>();







	


		Monster::Initialize();
	}
	void StoneEye::Update()
	{
		Monster::Update();
	}
	void StoneEye::LateUpdate()
	{
		Monster::LateUpdate();
	}
	void StoneEye::Render()
	{
		Monster::Render();
	}
}
