#pragma once


namespace ss::enums
{
	// 컴포넌트 순서는 바꾸지 말 것
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


	// 내가 만들고자 하는 게임에 따라 레이어 종류 증가시키기 
	enum class eLayerType
	{
		Grid,  // 그리드는 맨 뒤에 출력되어야하므로 맨 앞에 둠 
		BG,
		Monster,
		Boss,
		Player, // 몬스터와 보스에게 가려지면 안되서 레이어 순서 이렇게 바꿔둠
		Bullet,
		Collision, // trigger 용도???
		Ground,
		Wall,
		Camera, // 카메라 순서 gpt한테 한번 물어보기 
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
	enum class eTextureType : UINT //hlsl에 활용하기 위해 UINT로 형변환
	{
		PLYAERHP, // 플레이어
		PLAYERSP,
		MONSTER,
		BOSS,
		END,
	};
}