#include "ssMonster.h"
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
#include "ssMonsterScript.h"

namespace ss
{
	Monster::Monster()
		: Character(eType::MONSTER)
	{
		AddComponent<MeshRenderer>();
		AddComponent<Animator>();
		AddComponent<Collider2D>();
		AddComponent<Rigidbody2D>();
		AddComponent<CharacterState>();

	}

	Monster::~Monster()
	{
	}

	void Monster::Initialize()
	{

	
		Character::Initialize();
	}

	void Monster::Update()
	{
		Character::Update();
	}

	void Monster::LateUpdate()
	{
		Character::LateUpdate();
	}

	void Monster::Render()
	{
		Character::Render();
	}
}
