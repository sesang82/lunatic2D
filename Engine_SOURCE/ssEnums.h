#pragma once


namespace ss::enums
{
	// ������Ʈ ������ �ٲ��� �� ��
	enum class eComponentType
	{
		None,
		Transform,
		Mesh,
		Rigidbody2D,
		Collider2D,
		Particle,
		MeshRenderer,
		Animator,
		Script,
		Camera,
		AudioListener,
		AudioSource,
		End,
	};


	// ���� ������� �ϴ� ���ӿ� ���� ���̾� ���� ������Ű�� 
	enum class eLayerType
	{
		Grid,  // �׸���� �� �ڿ� ��µǾ���ϹǷ� �� �տ� �� 
		BG,
		Collision, // trigger �뵵???
		Monster,
		Boss,
		Player, // ���Ϳ� �������� �������� �ȵǼ� ���̾� ���� �̷��� �ٲ��
		Player_Bullet,
		Mon_Bullet,
		Guard,
		Effect,
		Item,
		Ground,
		Wall,
		Etc, // ���� UI�� �����ϴµ� ��ũ��Ʈ�� ���̴ٺ��� �ȳ����� �͵��� �̰� ����
		Camera, // ī�޶� ���� gpt���� �ѹ� ����� 
		UI,
		End = 16
	};

	enum class eResourceType
	{
		Texture,
		Shader,
		Material,
		Mesh,
		Animation,
		ComputeShader,
		AudioClip,
		End,
	};

	enum class eColliderType
	{
		Rect,
		Circle,
		Sphere,
		Cube,
		End,
	};

	// 
	enum class eTextureType : UINT //hlsl�� Ȱ���ϱ� ���� UINT�� ����ȯ
	{
		PLYAERHP, // �÷��̾�
		PLAYERSP,
		MONSTER,
		BOSS,
		END,
	};



	enum class eWeaponType
	{
		NONE = 1,
		SWORD,
		GAUNTLET,
		PISTOL,
		END,
	};

}