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
		End,
	};


	// ���� ������� �ϴ� ���ӿ� ���� ���̾� ���� ������Ű�� 
	enum class eLayerType
	{
		Grid,  // �׸���� �� �ڿ� ��µǾ���ϹǷ� �� �տ� �� 
		BG,
		Monster,
		Boss,
		Player, // ���Ϳ� �������� �������� �ȵǼ� ���̾� ���� �̷��� �ٲ��
		Bullet,
		Collision, // trigger �뵵???
		Ground,
		Wall,
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
}