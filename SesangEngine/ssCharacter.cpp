#include "ssCharacter.h"
#include "ssMeshRenderer.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"

namespace ss
{

	Character::Character(eType _type)
		: mCharacterType(_type)
		
	{
	}


	Character::~Character()
	{
	}
	void Character::Initialize()
	{
		GameObject::Initialize();
	}
	void Character::Update()
	{
		GameObject::Update();
	}
	void Character::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Character::Render()
	{
		GameObject::Render();
	}



}