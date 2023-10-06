#include "ssSwordBullet.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssTime.h"
#include "ssSmallEnergyballScript.h"
#include "ssAnimator.h"
#include "ssobject.h"
#include "ssGoddnessScript.h"
#include "ssPlayer.h"
#include "ssSwordBulletScript.h"

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


		mTransform->SetScale(Vector3(89.f, 89.f, 1.0f));
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SwordBulletMtrl"));

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"SwordBulletCol");


		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2_Sword_wallHit_Effect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2_Sword_Left_To_Right_Effect");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Boss2_Sword_Right_To_Left_Effect");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Boss2_Sword_Down_To_Up_Effect");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Boss2_Sword_Up_To_Down_Effect");

		mAnimator->Create(L"Boss2_Sword_wallHit", Image1, Vector2(0.f, 0.f), Vector2(89.f, 38.f), 4, Vector2(89.f, 38.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Boss2_Sword_LToR", Image2, Vector2(0.f, 0.f), Vector2(89.f, 38.f), 1, Vector2(89.f, 38.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Boss2_Sword_RToL", Image3, Vector2(0.f, 0.f), Vector2(89.f, 38.f), 1, Vector2(89.f, 38.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Boss2_Sword_DtoU", Image4, Vector2(0.f, 0.f), Vector2(38.f, 89.f), 1, Vector2(38.f, 89.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Boss2_Sword_UtoD", Image5, Vector2(0.f, 0.f), Vector2(38.f, 89.f), 1, Vector2(38.f, 89.f), Vector2::Zero, 0.08f);


		AddComponent<SwordBulletScript>();

		Bullet::Initialize();
	}
	void SwordBullet::Update()
	{

		if (GetName() == L"Sword_LeftToLight")
		{
			mAnimator->PlayAnimation(L"Boss2_Sword_LToR", false); // 벽에 부딪칠 때 해당 애니메이션 재생
			mTransform->SetScale(Vector3(89.f, 38.f, 1.0f));

	
		}


		else if (GetName() == L"Sword_RightToLeft")
		{
			mAnimator->PlayAnimation(L"Boss2_Sword_RToL", false); // 벽에 부딪칠 때 해당 애니메이션 재생
			mTransform->SetScale(Vector3(89.f, 38.f, 1.0f));

		}




		else if (GetName() == L"Sword_DownToUp")
		{
			mAnimator->PlayAnimation(L"Boss2_Sword_DtoU", false); // 벽에 부딪칠 때 해당 애니메이션 재생
			mTransform->SetScale(Vector3(38.f, 89.f, 1.0f));
		}

		else if (GetName() == L"Sword_UpToDown")
		{
			mAnimator->PlayAnimation(L"Boss2_Sword_UtoD", false); // 벽에 부딪칠 때 해당 애니메이션 재생
			mTransform->SetScale(Vector3(38.f, 89.f, 1.0f));
		}



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
