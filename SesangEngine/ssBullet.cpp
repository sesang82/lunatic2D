#include "ssBullet.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"

namespace ss
{
	Bullet::Bullet()
	{
	}
	Bullet::~Bullet()
	{
	}
	void Bullet::Initialize()
	{
		tr = GetComponent<Transform>();



		GameObject::Initialize();

	}
	void Bullet::Update()
	{
		GameObject::Update();
	}
	void Bullet::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Bullet::Render()
	{
		GameObject::Render();
	}
}