#include "ssStage1Scene.h"

#include "ssInput.h"
#include "ssSceneManager.h"
#include "ssTransform.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssCamera.h"
#include "ssRenderer.h"
#include "ssBackground.h"
#include "ssObject.h"
#include "ssPlatform.h"
#include "ssGroundScript.h"
#include "ssCamera.h"
#include "ssCameraScript.h"
#include "ssPlayer.h"
#include "ssMonster.h"
#include "ssStoneEyeScript.h"
#include "ssCollisionManager.h"
#include "ssPlayerScript.h"
#include "ssStoneEye.h"
#include "ssStoneEyeScript.h"
#include "ssAttackCollider.h"
#include "ssPlayerAttackColScript.h"
#include "ssMonsterBar.h"
#include "ssSkeletonArcherScript.h"
#include "ssSkeletonLizardScript.h"
#include "ssWoodGolemScript.h"
#include "ssWolfScript.h"
#include "ssZombieScript.h"
#include "ssItem.h"
#include "ssItemScript.h"
#include "ssParalloxScript.h"
#include "ssWallScript.h"
#include "ssUIFrameScript.h"
#include "ssAudioClip.h"
#include "ssAudioSource.h"
#include "ssSoundMgrScript.h"
#include "ssAudioListener.h"

namespace ss
{
	Stage1Scene::Stage1Scene()
	{
	}
	Stage1Scene::~Stage1Scene()
	{
	}
	void Stage1Scene::Initialize()
	{



		//// ====== 배경
		//// 둥근 달

		//Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Moon");
		//bg->Initialize();

		//MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
		//mr->SetMaterial(Resources::Find<Material>(L"Moon2Mtrl"));

		//bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 50.f, 850.f));
		//bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 900.f, 1.0f));

		//ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
		//parallox->SetPlayer(mPlayer);
		//parallox->SetSpeed(Vector2(0.18f, 0.18f));

		//parallox->SetMinX(-31.0f);
		//parallox->SetMaxX(33.0f);

		//parallox->SetMinY(-30.0f);
		//parallox->SetMaxY(170.0f);


		//// 나무1

		//{
		//	Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Tree1");
		//	bg->Initialize();


		//	MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
		//	mr->SetMaterial(Resources::Find<Material>(L"Tree1Mtrl"));

		//	bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -150.0f, 800.f));
		//	bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 618.f, 1.0f));

		//	ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
		//	parallox->SetPlayer(mPlayer);
		//	parallox->SetSpeed(Vector2(0.3f, 0.3f));

		//	parallox->SetMinX(-48.0f);
		//	parallox->SetMaxX(48.0f);

		//	parallox->SetMinY(-150.0f);
		//	parallox->SetMaxY(150.0f);

		//}

		//// 나무 2 
		//{
		//	Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Tree2");
		//	bg->Initialize();

		//	MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
		//	mr->SetMaterial(Resources::Find<Material>(L"Tree2Mtrl"));

		//	bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -250.f, 750.f));
		//	bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 618.f, 1.0f));



		//	ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
		//	parallox->SetPlayer(mPlayer);
		//	parallox->SetSpeed(Vector2(0.4f, 0.4f));

		//	parallox->SetMinX(-30.0f);
		//	parallox->SetMaxX(30.0f);

		//	parallox->SetMinY(-150.0f);
		//	parallox->SetMaxY(90.0f);


		//}



		// 맵 배경	
		//
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonMap");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Stage1Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.f, 650.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1600.f, 900.f, 1.0f));


		}





		 //캐릭터
		mPlayer = object::Instantiate<Player>(eLayerType::Player, L"Player");
		mPlayer->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함

		Transform* Playertr = mPlayer->GetComponent<Transform>();
		Playertr->SetPosition(Vector3(-400.f, -366.0f, 500.f)); // 이건 플레이어의 처음 위치임 ... 

		{
			// 스톤아이
			StoneEye* Stone = object::Instantiate<StoneEye>(eLayerType::Monster, L"StoneEye");
			Stone->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			Transform* eyetr = Stone->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(-200.f, -528.f, 500.f));

			StoneEyeScript* stonescript = Stone->GetComponent<StoneEyeScript>();
			stonescript->SetFirstPos(eyetr->GetPosition());

			// 몬스터 체력바 틀
			MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::UI, L"StoneEyeBar");
			mosnterbar->Initialize();

			Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
			mosnterbar->SetParent(Stone);
			monsterbartr->SetPosition(Vector3(-36.f, 38.f, 500.f));


			// 몬스터 HP
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::UI, L"StoneEyeHP");
			stoneHP->SetParent(Stone);
			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			//stoneHP->SetOffset(-36.0f);
			stonehptr->SetPosition(Vector3(-36.f, 38.f, 500.f));
			stoneHP->Initialize();

		}

		{
			// 해골 Archer (궁수)
			Monster* Stone = object::Instantiate<Monster>(eLayerType::Monster, L"Archer");
			Stone->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			Transform* eyetr = Stone->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(-111.f, -166.f, 500.f));

			SkeletonArcherScript* ArcherScript = Stone->AddComponent<SkeletonArcherScript>();
			ArcherScript->SetFirstPos(eyetr->GetPosition());


			// ===== hp틀 그냥 넣지 말기 
			// 몬스터 HP (UI로 넣지말기. 그럼 UI카메라에 의해 플레이어 따라다니는 것처럼 움직임) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"ArcherHP");
			stoneHP->SetParent(Stone);

		
			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(-5.f, 38.f, 500.f));

			stoneHP->Initialize();

		}


		{
			//// 해골 Lizard (근접 공격만 하는 애)
			//Monster* Liazrd = object::Instantiate<Monster>(eLayerType::Monster, L"Lizard");
			//Liazrd->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			//Transform* eyetr = Liazrd->GetComponent<Transform>();
			//eyetr->SetPosition(Vector3(-50.f, 185.f, 500.f));
		


			//SkeletonLizardScript* ArcherScript = Liazrd->AddComponent<SkeletonLizardScript>();
			//ArcherScript->SetFirstPos(eyetr->GetPosition());


			//// ===== hp틀 그냥 넣지 말기 
			//// 몬스터 HP (UI로 넣지말기. 그럼 UI카메라에 의해 플레이어 따라다니는 것처럼 움직임) 
			//Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"LizardHP");
			//stoneHP->SetParent(Liazrd);


			//Transform* stonehptr = stoneHP->GetComponent<Transform>();
			//stonehptr->SetPosition(Vector3(-15.f, 25.f, 500.f));

			//stoneHP->Initialize();

		}


		{
			// Wood Golem 몬스터 
			Monster* Golem = object::Instantiate<Monster>(eLayerType::Monster, L"WoodGolemObj");
			Golem->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			Transform* eyetr = Golem->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(-240.f, 70.f, 500.f));

			WoodGolemScript* ArcherScript = Golem->AddComponent<WoodGolemScript>();
			ArcherScript->SetFirstPos(eyetr->GetPosition());

			// 몬스터 체력바 틀
		/*	MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::Etc, L"WoodHPBar");
			mosnterbar->Initialize();

			Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
			mosnterbar->SetParent(Golem);
			monsterbartr->SetPosition(Vector3(50.f, 40.f, 500.f));*/


			// 몬스터 HP (UI로 넣지말기. 그럼 UI카메라에 의해 플레이어 따라다니는 것처럼 움직임) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"WoodHP");
			stoneHP->SetParent(Golem);
			stoneHP->SetOffset(100.f);


			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(-10.f, 40.f, 500.f));

			stoneHP->Initialize();

		}

		{
			// 미니 wolf 몬스터 
			Monster* Wolf = object::Instantiate<Monster>(eLayerType::Monster, L"WolfObj");
			Wolf->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			
			
			Transform* eyetr = Wolf->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(10.f, 185.f, 500.f));
			//eyetr->SetPosition(Vector3(-350.f, -340.f, 500.f));

			WolfScript* wolfScript = Wolf->AddComponent<WolfScript>();
			wolfScript->SetFirstPos(eyetr->GetPosition());

			// ===== hp틀 그냥 넣지 말기 
			// 몬스터 HP (UI로 넣지말기. 그럼 UI카메라에 의해 플레이어 따라다니는 것처럼 움직임) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"WolfHP");
			stoneHP->SetParent(Wolf);
		
			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(7.f, 25.f, 500.f));

			stoneHP->Initialize();

		}

		
		//	// Zombi 몬스터 (해당 씬에는 더 이상 둘 곳이 없어서 보류)
		//	Monster* Wolf = object::Instantiate<Monster>(eLayerType::Monster, L"ZombiObj");
		//	Wolf->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


		//	Transform* eyetr = Wolf->GetComponent<Transform>();
		//	//eyetr->SetPosition(Vector3(10.f, 185.f, 500.f));
		//	eyetr->SetPosition(Vector3(-350.f, -340.f, 500.f));

		//	ZombieScript* wolfScript = Wolf->AddComponent<ZombieScript>();
		//	wolfScript->SetFirstPos(eyetr->GetPosition());


		//	// 몬스터 체력바 틀
		///*	MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::Etc, L"WolfHPBar");
		//	mosnterbar->Initialize();

		//	Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
		//	mosnterbar->SetParent(Wolf);
		//	monsterbartr->SetPosition(Vector3(50.f, 25.f, 500.f));*/

		//	// ===== hp틀 그냥 넣지 말기 
		//	// 몬스터 HP (UI로 넣지말기. 그럼 UI카메라에 의해 플레이어 따라다니는 것처럼 움직임) 
		//	Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"ZombiHP");
		//	stoneHP->SetParent(Wolf);

		//	Transform* stonehptr = stoneHP->GetComponent<Transform>();
		//	stonehptr->SetPosition(Vector3(50.f, 25.f, 500.f));

		//	stoneHP->Initialize();

		//}
			// =====
		// 테스트용으로 좋은 위치 			eyetr->SetPosition(Vector3(-350.f, -340.f, 500.f));



		// 아이템 오버로드 
		{
			Item* Item_overload = object::Instantiate<Item>(eLayerType::Item, L"overload_Item");


			ItemScript* script = Item_overload->AddComponent<ItemScript>();
			script->SetOwnerObj(mPlayer);

			Transform* tr = Item_overload->GetComponent<Transform>();
			tr->SetPosition(Vector3(-400.f, -400.f, 500.f));





		}

		////==================
	   // Collision


   // 충돌체 (왼쪽 바닥)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
		

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-290.f, -487.f, 500.f));
			tr->SetScale(Vector3(416.f, 200.f, 1.f));


		}

		// 충돌체 (왼쪽 바닥)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-80.f, -338.f, 500.f));
			tr->SetScale(Vector3(95.f, 95.f, 1.f));


		}

		// 충돌체 (맨 아래 오른쪽 바닥 )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
	

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(43.f, -396.f, 500.f));
			tr->SetScale(Vector3(150.f, 150.f, 1.f));


		}


		// 충돌체 (2층 통나무 바닥  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-111.f, -198.f, 500.f));
			tr->SetScale(Vector3(222.f, 5.f, 1.f));


		}


		// 충돌체 (3층 통나무 바닥  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-240.f, -68.f, 500.f));
			tr->SetScale(Vector3(220.f, 5.f, 1.f));


		}

		// 충돌체 (3층 통나무 옆 바닥  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-384.f, -98.f, 500.f));
			tr->SetScale(Vector3(62.f, 5.f, 1.f));


		}



		// 충돌체 (4층 통나무 바닥  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-240.f, 28.f, 500.f));
			tr->SetScale(Vector3(222.f, 5.f, 1.f));


		}


		// 충돌체 (5층 통나무 바닥  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(70.f, 158.f, 500.f));
			tr->SetScale(Vector3(520.f, 5.f, 1.f));


		}


		// 충돌체 (다음 씬 넘어가는)
		{
			Platform* col_Door = object::Instantiate<Platform>(eLayerType::Collision, L"L_Stage1_col_Door");
			col_Door->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함

			Transform* tr = col_Door->GetComponent<Transform>();
			tr->SetPosition(Vector3(320.f, 200.f, 500.f));
			tr->SetScale(Vector3(40.f, 61.f, 1.f));

		}
	


		// ================================
		// 벽


		// 1층 중간 오른쪽 벽

		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Wall, L"Wall_R_Obj");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-120.f, -350.f, 500.f));
			tr->SetScale(Vector3(20.f, 117.f, 1.f)); //바닥보다는 살짝 작게


			WallScript* wallscript = col_Floor->AddComponent<WallScript>();
			wallscript->SetPlayer(mPlayer);


		}


		// 2층 맨 오른쪽 벽
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Wall, L"Wall_R_Obj");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(105.f, -150.f, 500.f));
			tr->SetScale(Vector3(20.f, 250.f, 1.f)); //바닥보다는 살짝 작게


			WallScript* wallscript = col_Floor->AddComponent<WallScript>();
			wallscript->SetPlayer(mPlayer);


		}

		// 맨 위층 왼쪽 천장 
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Wall, L"Wall_Up_Obj");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-120.f, 265.f, 500.f));
			tr->SetScale(Vector3(270.f, 20.f, 1.f));


			WallScript* wallscript = col_Floor->AddComponent<WallScript>();
			wallscript->SetPlayer(mPlayer);


		}


		// ======================================

		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerFrameMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -195.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(308.f, 57.f, 1.f));
		}

		// 무기 슬롯1
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			bg->SetName(L"Weapon_Slot1");

			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-33.f, -197.f, 90.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(24.f, 24.f, 1.f));


			UIFrameScript* uiframe = bg->AddComponent<UIFrameScript>();
			uiframe->SetPlayer(mPlayer);


		}

		// 무기 슬롯2
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			bg->SetName(L"Weapon_Slot2");

			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -185.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(24.f, 24.f, 1.f));


			UIFrameScript* uiframe = bg->AddComponent<UIFrameScript>();
			uiframe->SetPlayer(mPlayer);
		}

		// 무기 슬롯3
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			bg->SetName(L"Weapon_Slot3");

			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(33.f, -197.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(24.f, 24.f, 1.f));


			UIFrameScript* uiframe = bg->AddComponent<UIFrameScript>();
			uiframe->SetPlayer(mPlayer);
		}


		//플레이어 HP바
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerBarMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-103.f, -205.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(99.f, 8.f, 1.f));
		}

		//플레이어 SP바
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerSPBarMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(103.f, -205.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(99.f, 8.f, 1.f));
		}

		//플레이어 오버로드 바
		{
			Progressbar* overloadBar = object::Instantiate<Progressbar>(eLayerType::UI, L"overloadBar");
			overloadBar->SetOwner(mPlayer);

			Transform* tr = overloadBar->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -218.f, 95.f));

			overloadBar->Initialize();

		}


		//플레이어 오버로드 빈 bar 
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Overload_BarMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -218.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(48.f, 4.f, 1.f));
		}


		// 키보드 자판
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Keyboard_A_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-33.f, -218.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(12.f, 12.f, 1.f));
		}


		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Keyboard_S_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -195.f, 80.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(12.f, 12.f, 1.f));
		}

		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Keyboard_D_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(33.f, -218.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(12.f, 12.f, 1.f));
		}

		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Keyboard_F_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -210.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(12.f, 12.f, 1.f));
		}



		// === 카메라

	 // 메인 카메라 생성 (반드시 씬마다 각기 다른 메인 카메라가 하나씩 있어야함!) 
		{
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Camera, camera);

			mCamera = camera->AddComponent<Camera>();
			mCamera->TurnLayerMask(eLayerType::UI, false);

			CameraScript* camerscript = camera->AddComponent<CameraScript>();

		}

		//UI Camera
		{
			GameObject* UIcamera = new GameObject();
			AddGameObject(eLayerType::Camera, UIcamera);
			UIcamera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 970.0f));
			Camera* cameraComp0 = UIcamera->AddComponent<Camera>();
			cameraComp0->DisableLayerMasks();
			cameraComp0->TurnLayerMask(eLayerType::UI);
			renderer::cameras.push_back(cameraComp0);
		}



		Scene::Initialize();
	}
	void Stage1Scene::Update()
	{

		Scene::Update();
	}
	void Stage1Scene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Stage1Scene::Render()
	{
		Scene::Render();
	}
	void Stage1Scene::OnEnter()
	{
		Scene::OnEnter();

		renderer::mainCamera = mCamera;
		mCamera->SetSize(2.3f); // 2.3

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(mPlayer);
	}
	void Stage1Scene::OnExit()
	{
		Scene::OnExit();



	}
}