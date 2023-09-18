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
		AudioListener,
		AudioSource,
		End,
	};


	// 내가 만들고자 하는 게임에 따라 레이어 종류 증가시키기 
	enum class eLayerType
	{
		Grid,  // 그리드는 맨 뒤에 출력되어야하므로 맨 앞에 둠 
		BG,
		Collision, // trigger 용도???
		Monster,
		Boss,
		Player, // 몬스터와 보스에게 가려지면 안되서 레이어 순서 이렇게 바꿔둠
		Player_Bullet,
		Mon_Bullet,
		Guard,
		Effect,
		Item,
		Ground,
		Wall,
		Etc, // 원래 UI로 빼야하는데 스크립트에 붙이다보니 안나오는 것들은 이거 쓰기
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
	enum class eTextureType : UINT //hlsl에 활용하기 위해 UINT로 형변환
	{
		PLYAERHP, // 플레이어
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