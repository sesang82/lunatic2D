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
		tr->SetScale(Vector3(240.f, 123.f, 1.0f)); // back사이즈 그대로 쓰기 

		MeshRenderer* mr = GetComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial")); // 어차피 애니메이션 쓸 거라 암거나 넣어두면 됨 

		Animator* ani = GetComponent<Animator>();

		std::shared_ptr<graphics::Texture> Image1 = Resources::Find<graphics::Texture>(L"StoneEyeIdle");
		std::shared_ptr<graphics::Texture> Image2 = Resources::Find<graphics::Texture>(L"StoneEyeHit");
		std::shared_ptr<graphics::Texture> Image3 = Resources::Find<graphics::Texture>(L"StoneEyeStun");
		std::shared_ptr<graphics::Texture> Image4 = Resources::Find<graphics::Texture>(L"StoneEyeDie");
		std::shared_ptr<graphics::Texture> Image5 = Resources::Find<graphics::Texture>(L"StoneEyeNearAttack");
		std::shared_ptr<graphics::Texture> Image6 = Resources::Find<graphics::Texture>(L"StoneEyeBorrow");
		std::shared_ptr<graphics::Texture> Image7 = Resources::Find<graphics::Texture>(L"StoneEyeFarAttack");

		// === 
		// 		// 각 충돌체 크기는 달라지는게 있다면 fsm에서 각각 조정해주기 
		// === 
		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 오프셋(y값은 -값이 클수록 올라감), 재생시간, 뒤집기

		// === 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 백사이즈, 오프셋(y값은 -값이 클수록 올라감)
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


		// 9프레임에 총알 나감 
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
