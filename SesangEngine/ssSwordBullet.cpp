#include "ssSwordBullet.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssTime.h"
#include "ssEnergyballScript.h"
#include "ssAnimator.h"
#include "ssobject.h"
#include "ssGoddnessScript.h"
#include "ssPlayer.h"

namespace ss
{
	SwordBullet::SwordBullet()
		: mAnimator(nullptr)
		, mTransform(nullptr)
	{
		SwordBullet::Initialize();
	}
	SwordBullet::~SwordBullet()
	{
	}
	void SwordBullet::Initialize()
	{
		mTransform = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mAnimator = AddComponent<Animator>();


		mTransform->SetScale(Vector3(89.f, 38.f, 1.0f));
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"Boss2_Sword_base_Effect_Mtrl"));

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"SwordBulletCol");


		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2_Sword_base_Effect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2_Sword_wallHit_Effect");

		mAnimator->Create(L"Boss2_Sword", Image1, Vector2(0.f, 0.f), Vector2(89.f, 38.f), 1, Vector2(89.f, 38.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Boss2_Sword_wallHit", Image2, Vector2(0.f, 0.f), Vector2(89.f, 38.f), 4, Vector2(89.f, 38.f), Vector2::Zero, 0.08f);

		mAnimator->PlayAnimation(L"Boss2_Sword", false); // 벽에 부딪칠 때 해당 애니메이션 재생



		Bullet::Initialize();
	}
	void SwordBullet::Update()
	{
		Bullet::Update();
	}
	void SwordBullet::LateUpdate()
	{
		Bullet::LateUpdate();
	}
	void SwordBullet::Render()
	{
		Bullet::Render();
	}
}
