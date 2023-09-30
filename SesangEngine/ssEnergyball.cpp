#include "ssEnergyball.h"
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
	int Energyball::miSpawnedBallCount = 0;

	Energyball::Energyball()
		: mAnimator(nullptr)
		, mTransform(nullptr)
		, mbTest(false)
		, mbSpawnComplete(false)
		, mStorePlayerPos(false)
		, miMaxSpawnedBalls(12)
		, mPlayerPos(Vector3::Zero)
		, mfTime(0.0f)
	{

		spawnPatterns =
		{
			// LT
			{0, 35},
			{0, -35}, // �Ʒ�
			{35, 35}, // ������


			// LB
			{-35, -105},  // ������
			{0, 35}, // �� 
			{35, -35}, // ������

			// RT
			{70, 0}, // ������
			{0, 35}, // ��
			{-35, -35}, // �Ʒ� 

			// RB
			{35, 105}, // ������
			{0, -35}, // �Ʒ�
			{-35, 35}, // ��
		};

		Energyball::Initialize();


	}
	Energyball::~Energyball()
	{
	}
	void Energyball::Initialize()
	{
		mTransform = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mAnimator = AddComponent<Animator>();


		mTransform->SetScale(Vector3(58.f, 54.f, 1.0f));
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"EnergyballCol");


		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_Spawn_Effect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_AfterSpawn_Effect");

		mAnimator->Create(L"Energyball_S_Parrying_Spawn", Image1, Vector2(0.f, 0.f), Vector2(58.f, 54.f), 6, Vector2(58.f, 54.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Energyball_S_Parrying_Energying", Image2, Vector2(0.f, 0.f), Vector2(58.f, 54.f), 6, Vector2(58.f, 54.f), Vector2::Zero, 0.08f);


		mAnimator->PlayAnimation(L"Energyball_S_Parrying_Spawn", false); // trigger �ϼ��ϸ� ����� 

		AddComponent<EnergyballScript>();

		
		

		Bullet::Initialize();
	}
	void Energyball::Update()
	{

		int i = miSpawnedBallCount;

			mPlayer = SceneManager::GetPlayer();

			RemoveComponent<Collider2D>();


		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mAnimator->PlayAnimation(L"Energyball_S_Parrying_Energying", true);

			if (!mbTest)
			{
				CreateEnergyball();
				
				//object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y - 35.f, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj2"); // �Ʒ�
				//object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x + 60.f, mTransform->GetPosition().y + 35.f, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj3"); // ������


			//	object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y - 35.f, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj2"); // ������

				mbTest = true;
				mbSpawnComplete = true;
			}	

		}

		mfTime += Time::DeltaTime();

		if (mfTime > 7.f && mbSpawnComplete && miSpawnedBallCount == 12)
		{
			AddComponent<Collider2D>();

			if (!mStorePlayerPos)
			{
				mPlayer = SceneManager::GetPlayer();
				// ������ �밢������ �̵��Ѵ�.
				mPlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
				mStorePlayerPos = true;
			
			}

			Vector3 TargetPos = Vector3(mPlayerPos.x, mPlayerPos.y, mPlayerPos.z);
			Vector3 EnergyballPos = mTransform->GetPosition();

			// ���󿡼� ��ǥ ��ġ���� ���� ���͸� ����մϴ�.
			Vector3 dir = TargetPos - EnergyballPos;
			float distance = dir.Length();  // ����� ��ǥ ��ġ ������ �Ÿ��� ����մϴ�.
			dir.Normalize();  // ���� ���͸� ����ȭ�մϴ�.


			float moveSpeed = 380.0f;  // ���ϴ� �ӵ� ���� �����ϼ���.

			Vector3 moveAmount = dir * moveSpeed * Time::DeltaTime();  // �����Ӵ� ������ ���� ����մϴ�.

			EnergyballPos += moveAmount;  // ���� ��ġ�� ������Ʈ�մϴ�.
			mTransform->SetPosition(EnergyballPos);  // ������Ʈ�� ��ġ�� �����մϴ�.

		}

		Bullet::Update();
	}
	void Energyball::LateUpdate()
	{
		Bullet::LateUpdate();
	}
	void Energyball::Render()
	{
		Bullet::Render();
	}

	void Energyball::CreateEnergyball()
	{

		// 12�������� �����ǰ� �ص� 
		if (miMaxSpawnedBalls <= miSpawnedBallCount)
			return;


		Vector2 offset = spawnPatterns[miSpawnedBallCount]; // stiatc���� �صױ� ������ ��ü�� �����ǵ� ���ϴ� �ε������� ��ġ �� ������ �� ���� 


		object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x + offset.x, mTransform->GetPosition().y + offset.y, 400.f),
			eLayerType::Collision, L"Parrying_S_EnergyballObj"); // �Ʒ�

		++miSpawnedBallCount;


	}






	
}