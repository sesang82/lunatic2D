#include "ssRenderer.h"
#include "ssResources.h"
#include "ssTexture.h"
#include "ssMaterial.h"
#include "ssResources.h"
#include "ssAudioSource.h"



namespace renderer
{
	using namespace ss;
	using namespace ss::graphics;
	ss::graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[(UINT)eSamplerType::End] = {};

	//
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)eRSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)eDSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)eBSType::End] = {};
	

	//
	ss::Camera* mainCamera = nullptr;
	std::vector<ss::Camera*> cameras = {};
	std::vector<DebugMesh> debugMeshs = {};

	// 정점 내부 정보에 uv좌표 정보까지 넣어준다. 
	void Initialize()
	{
		LoadMesh();
		LoadBuffer();
		LoadShader();
		SetupState();
		LoadMaterial();
		LoadAudio();
	}


	void Render()
	{
		// 렌더링은 카메라를 통해 이뤄지므로, 여러 대의 카메라들을 순회하며
		// 카메라 클랫의 렌더 함수를 호출한다.
		for (Camera* cam : cameras)
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		// 중간에 카메라가 추가될수있는 경우도 있을 것이다.특별한 효과가 잠깐 나왔다가 사라지는 경우가 있기 때문
		// 그래서 카메라에 의한 물체를 렌더링 한 후에 카메라를 clear해서 지워주고,
		// 매 프레임마다 호출되는 카메라의 lateupdate에선 다시 카메라를 등록해주고, 또 Render함수가 호출되면서
		// 등록된 카메라로 렌더링을 하고, 등록했던 카메라를 다시 지워주고.. 이런식으로 반복되게끔 해준다.
		// (알아서 동적할당 되게끔) 
		cameras.clear();
	}

	void LoadMesh()
	{
		std::vector<Vertex> vertexes = {};
		std::vector<UINT> indexes = {};

		//RECT
		vertexes.resize(4);
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		// ====== 버텍스 버퍼 만들기
		// 리소스 류는 shared_ptr로 관리되기 때문에 new 연산자 대신 std::make_shared<>를 사용한다
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert(L"RectMesh", mesh);

		mesh->CreateVertexBuffer(vertexes.data(), vertexes.size());

		// ====== 인덱스 버퍼 만들기
				// 1. 인덱스 버퍼로 그려낼 정점의 순서를 정수형 배열에 넣어 지정한다.
		// 단, 넣는 순서는 반드시 '시계 방향'으로 넣어야한다.
		// 반시계 방향으로 순서를 지정하면 뒷면으로 인식하여 그려내지 않게 된다.
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);
		mesh->CreateIndexBuffer(indexes.data(), indexes.size());



		// Rect Debug Mesh
		std::shared_ptr<Mesh> rectDebug = std::make_shared<Mesh>();
		Resources::Insert(L"DebugRect", rectDebug);
		rectDebug->CreateVertexBuffer(vertexes.data(), vertexes.size());
		rectDebug->CreateIndexBuffer(indexes.data(), indexes.size());

		// Circle Debug Mesh
		vertexes.clear();
		indexes.clear();

		Vertex center = {};
		center.pos = Vector3(0.0f, 0.0f, 0.0f);
		center.color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes.push_back(center);

		int iSlice = 40;
		float fRadius = 0.5f;
		float fTheta = XM_2PI / (float)iSlice;

		for (int i = 0; i < iSlice; ++i)
		{
			center.pos = Vector3(fRadius * cosf(fTheta * (float)i)
				, fRadius * sinf(fTheta * (float)i)
				, 0.0f);
			center.color = Vector4(0.0f, 1.0f, 0.0f, 1.f);
			vertexes.push_back(center);
		}

		for (int i = 0; i < vertexes.size() - 2; ++i)
		{
			indexes.push_back(i + 1);
		}
		indexes.push_back(1);

		std::shared_ptr<Mesh> circleDebug = std::make_shared<Mesh>();
		Resources::Insert(L"DebugCircle", circleDebug);
		circleDebug->CreateVertexBuffer(vertexes.data(), vertexes.size());
		circleDebug->CreateIndexBuffer(indexes.data(), indexes.size());
	}

	void SetupState()
	{
#pragma region InputLayout
		// Input layout 정점 구조 정보를 넘겨줘야한다.
		D3D11_INPUT_ELEMENT_DESC arrLayout[3] = {};

		arrLayout[0].AlignedByteOffset = 0;
		arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[0].InputSlot = 0;
		arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[0].SemanticName = "POSITION";
		arrLayout[0].SemanticIndex = 0;

		arrLayout[1].AlignedByteOffset = 12;
		arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayout[1].InputSlot = 0;
		arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[1].SemanticName = "COLOR";
		arrLayout[1].SemanticIndex = 0;

		arrLayout[2].AlignedByteOffset = 28;
		arrLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayout[2].InputSlot = 0;
		arrLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[2].SemanticName = "TEXCOORD";
		arrLayout[2].SemanticIndex = 0;


		std::shared_ptr<Shader> shader = ss::Resources::Find<Shader>(L"SpriteShader");
		ss::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());


		shader = ss::Resources::Find<Shader>(L"NoCullingSpriteShader");
		ss::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());


		shader = ss::Resources::Find<Shader>(L"GridShader");
		ss::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = ss::Resources::Find<Shader>(L"DebugShader");
		ss::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = ss::Resources::Find<Shader>(L"SpriteAnimationShader");
		ss::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = ss::Resources::Find<Shader>(L"ProgressHPbarShader");
		ss::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = ss::Resources::Find<Shader>(L"ProgressSPbarShader");
		ss::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

#pragma endregion



#pragma region Sampler State
		//Sampler State
		D3D11_SAMPLER_DESC samplerDesc = {};

		// UV값이 0~1을 넘었을 때 반복되도록 해두는 모드
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[(UINT)eSamplerType::Point].GetAddressOf());
		GetDevice()->BindSampler(eShaderStage::PS, 0, samplerState[(UINT)eSamplerType::Point].GetAddressOf());

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSampler(eShaderStage::PS, 1, samplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
#pragma endregion



#pragma region Rasterizer State
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::WireframeNone].GetAddressOf());
#pragma endregion
#pragma region Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

		//less
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::Less].GetAddressOf());

		//Greater
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::Greater].GetAddressOf());

		//no write
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::NoWrite].GetAddressOf());


		//None
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::None].GetAddressOf());
#pragma endregion
#pragma region Blend State
		D3D11_BLEND_DESC blendDesc = {};
		
		//default
		blendStates[(UINT)eBSType::Default] = nullptr;
		
		// Alpha Blend
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&blendDesc
			, blendStates[(UINT)eBSType::AlphaBlend].GetAddressOf());

		// one one
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&blendDesc
			, blendStates[(UINT)eBSType::OneOne].GetAddressOf());

#pragma endregion


	}

	

	// 상수 버퍼 만들기 
	void LoadBuffer()
	{
		// Constant Buffer
		constantBuffer[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffer[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		// Grid Buffer
		constantBuffer[(UINT)eCBType::Grid] = new ConstantBuffer(eCBType::Grid);
		constantBuffer[(UINT)eCBType::Grid]->Create(sizeof(TransformCB));

		// Collision Buffer
		constantBuffer[(UINT)eCBType::Collision] = new ConstantBuffer(eCBType::Collision);
		constantBuffer[(UINT)eCBType::Collision]->Create(sizeof(ColCB));

		// Animator Buffer
		constantBuffer[(UINT)eCBType::Animator] = new ConstantBuffer(eCBType::Animator);
		constantBuffer[(UINT)eCBType::Animator]->Create(sizeof(AnimatorCB));

		// Progressbar Buffer
		constantBuffer[(UINT)eCBType::Progressbar] = new ConstantBuffer(eCBType::Progressbar);
		constantBuffer[(UINT)eCBType::Progressbar]->Create(sizeof(ProgressbarCB));

		// Hit Buffer (나중에 어느정도 완성되면 이거 컴퓨트 쉐이더로 써보기) 
		constantBuffer[(UINT)eCBType::Hit] = new ConstantBuffer(eCBType::Hit);
		constantBuffer[(UINT)eCBType::Hit]->Create(sizeof(HitCB));


		// Hit Buffer (나중에 어느정도 완성되면 이거 컴퓨트 쉐이더로 써보기) 
		constantBuffer[(UINT)eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffer[(UINT)eCBType::Material]->Create(sizeof(MaterialCB));
	}

	void LoadShader()
	{

		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		spriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		spriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		ss::Resources::Insert(L"SpriteShader", spriteShader);

		// guard로 튕겨내는 물체들은 이거 쉐이더 적용하기 
		std::shared_ptr<Shader> NoCullingSpriteShader = std::make_shared<Shader>();
		NoCullingSpriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		NoCullingSpriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		NoCullingSpriteShader->SetRSState(eRSType::SolidNone); // 뒷면으로 뒤집어도 culling 되지 않도록 함 
		ss::Resources::Insert(L"NoCullingSpriteShader", NoCullingSpriteShader);

		std::shared_ptr<Shader> spriteAnimShader = std::make_shared<Shader>();
		spriteAnimShader->Create(eShaderStage::VS, L"SpriteAnimationVS.hlsl", "main");
		spriteAnimShader->Create(eShaderStage::PS, L"SpriteAnimationPS.hlsl", "main");
		ss::Resources::Insert(L"SpriteAnimationShader", spriteAnimShader);



		std::shared_ptr<Shader> girdShader = std::make_shared<Shader>();
		girdShader->Create(eShaderStage::VS, L"GridVS.hlsl", "main");
		girdShader->Create(eShaderStage::PS, L"GridPS.hlsl", "main");
		ss::Resources::Insert(L"GridShader", girdShader);

		std::shared_ptr<Shader> debugShader = std::make_shared<Shader>();
		debugShader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
		debugShader->Create(eShaderStage::PS, L"DebugPS.hlsl", "main");
		debugShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		debugShader->SetRSState(eRSType::WireframeNone);
		//debugShader->SetDSState(eDSType::NoWrite);
		ss::Resources::Insert(L"DebugShader", debugShader);

		std::shared_ptr<Shader> progressShader = std::make_shared<Shader>();
		progressShader->Create(eShaderStage::VS, L"spProgressVS.hlsl", "main");
		progressShader->Create(eShaderStage::PS, L"hpProgressPS.hlsl", "main");
		ss::Resources::Insert(L"ProgressHPbarShader", progressShader);


		std::shared_ptr<Shader> progressShader2 = std::make_shared<Shader>();
		progressShader2->Create(eShaderStage::VS, L"spProgressVS.hlsl", "main");
		progressShader2->Create(eShaderStage::PS, L"spProgressPS.hlsl", "main");
		ss::Resources::Insert(L"ProgressSPbarShader", progressShader2);
	



	}



	// +.+.+.+. 참고
	// 1. 기본적으로 전부 다 불투명으로 한다. (UI 제외)
	// 2. 출력이 안될 시 반투명으로 바꾼다.
	// 3. 반투명으로 바꾼 게 안나온다면 둘 다 반투명일시 하나를 Grid 레이어로 넣는다
	// 4. UI는 항상 투명으로 해두기(안보인다면 안보이는 녀석을 반투명으로 보낸다)
	void LoadMaterial()
	{
		std::shared_ptr<Shader> spriteShader
			= Resources::Find<Shader>(L"SpriteShader");

		std::shared_ptr<Shader> progressShader
			= Resources::Find<Shader>(L"ProgressHPbarShader");

		std::shared_ptr<Shader> progressShader2
			= Resources::Find<Shader>(L"ProgressSPbarShader");

		
		std::shared_ptr<Shader> noCullingSpriteShader
			= Resources::Find<Shader>(L"NoCullingSpriteShader");


		std::shared_ptr<Texture> texture
			= Resources::Load<Texture>(L"Link", L"..\\Resources\\Texture\\Link.png");

		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(spriteShader);
		material->SetTexture(texture);
		Resources::Insert(L"SpriteMaterial", material);

		texture = Resources::Load<Texture>(L"Smile", L"..\\Resources\\Texture\\Smile.png");
		material = std::make_shared<Material>();
		material->SetShader(spriteShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SpriteMaterial02", material);


		// ==================================
		std::shared_ptr<Shader> AnimShader
			= Resources::Find<Shader>(L"SpriteAnimationShader");

		material = std::make_shared<Material>();
		material->SetShader(AnimShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SpriteAnimationMaterial", material);


		// 
		std::shared_ptr<Shader> BossShader
			= Resources::Find<Shader>(L"SpriteAnimationShader");

		material = std::make_shared<Material>();
		material->SetShader(BossShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"BossAnimMtrl", material);




		material = std::make_shared<Material>();
		material->SetShader(BossShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"PlayerAnimMtrl", material);


		material = std::make_shared<Material>();
		material->SetShader(BossShader);
		material->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"torchAnimMtrl", material);

		material = std::make_shared<Material>();
		material->SetShader(BossShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SwordBulletMtrl", material);

		material = std::make_shared<Material>();
		material->SetShader(BossShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SmallEnergyBulletMtrl", material);

		material = std::make_shared<Material>();
		material->SetShader(BossShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"BigEnergyBulletMtrl", material);




		std::shared_ptr<Shader> gridShader
			= Resources::Find<Shader>(L"GridShader");

		material = std::make_shared<Material>();
		material->SetShader(gridShader);
		Resources::Insert(L"GridMaterial", material);

		std::shared_ptr<Shader> debugShader
			= Resources::Find<Shader>(L"DebugShader");

		material = std::make_shared<Material>();
		material->SetShader(debugShader);
	//	material->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"DebugMaterial", material);


		// ============================================================
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"temp", L"..\\Resources\\Texture\\temp.png");

			// === Material 생성
			std::shared_ptr<Material> testMateiral = std::make_shared<Material>();
			testMateiral->SetShader(spriteShader);
			testMateiral->SetTexture(texture);
			testMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"tempMtrl", testMateiral);

		}
#pragma region Player [Default]
		//// ==========================
		//// +.+. 루나틱용 +.+.
		//// ====== 캐릭터들
		//// 1. 플레이어
		// [Default]
		
		// << Spawn
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Spawn", L"..\\Resources\\Texture\\Player\\Spawn\\merge_from_ofoct (4).png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_SpawnMtrl", spriteMateiral);

		}


		// << Idle
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Idle", L"..\\Resources\\Texture\\Player\\Default\\Idle\\merge_from_ofoct.png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);


			Resources::Insert(L"D_IdleMtrl", spriteMateiral);

		}

		// << Run
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Run", L"..\\Resources\\Texture\\Player\\Default\\Run\\merge_from_ofoct (3).png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_RunMtrl", spriteMateiral);

		}


		// << Jump (안합친거)
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Jump22222", L"..\\Resources\\Texture\\Player\\Default\\Jump\\merge_from_ofoct (1).png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_JumpMtrl", spriteMateiral);

		}

		// 합친거 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Jump", L"..\\Resources\\Texture\\Player\\Default\\Jump\\jump.png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_JumpMtrl", spriteMateiral);

		}


		// << Landing
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Landing", L"..\\Resources\\Texture\\Player\\Default\\Landing\\merge_from_ofoct (2).png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_LandingMtrl", spriteMateiral);

		}

		// Fall
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Fall", L"..\\Resources\\Texture\\Player\\Default\\Fall\\Fall.png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_FallMtrl", spriteMateiral);
		}




#pragma endregion 


#pragma region Player [Sword]
		// >> Idle
		
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"S_PlayerIdle", L"..\\Resources\\Texture\\Player\\Sword\\Idle\\Idle.png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"S_PlayerIdleMtrl", spriteMateiral);

		}

		// >> Run
		// 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"S_PlayerRun", L"..\\Resources\\Texture\\Player\\Sword\\Run\\Run.png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"S_PlayerRunMtrl", spriteMateiral);

		}
		// >> Jump (fall 안합친거)
		 {
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"S_PlayerJump", L"..\\Resources\\Texture\\Player\\Sword\\Jump\\Jump.png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"S_PlayerJumpMtrl", spriteMateiral);

		}
		// >> Fall
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerFall", L"..\\Resources\\Texture\\Player\\Sword\\Fall\\Fall.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerFallMtrl", spriteMateiral);

		 }
		// >> Landing
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerLanding", L"..\\Resources\\Texture\\Player\\Sword\\Landing\\Landing.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerLandingMtrl", spriteMateiral);

		 }
		// >> Hang
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerHang", L"..\\Resources\\Texture\\Player\\Sword\\Hang\\Hang.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerHangMtrl", spriteMateiral);

		 }
		 // >> Dash
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerDash", L"..\\Resources\\Texture\\Player\\Sword\\Dash\\Dash.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerDashMtrl", spriteMateiral);

		 }
		// >> Stun
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerStun", L"..\\Resources\\Texture\\Player\\Sword\\Stun\\Stun.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerStunMtrl", spriteMateiral);

		 }


		 // >> Hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerHit", L"..\\Resources\\Texture\\Player\\Sword\\Hit\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_HitMtrl", spriteMateiral);

		 }





		// >> Die
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerDie", L"..\\Resources\\Texture\\Player\\Sword\\Die\\Die.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerDieMtrl", spriteMateiral);

		 }
		// >> Guard
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerGuard", L"..\\Resources\\Texture\\Player\\Sword\\Guard\\Guard.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerGuardMtrl", spriteMateiral);

		 }
		// >> Attack 1
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerAttack1", L"..\\Resources\\Texture\\Player\\Sword\\Attack\\Attack1.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerAttack1Mtrl", spriteMateiral);

		 }

		 // >> Attack 2
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerAttack2", L"..\\Resources\\Texture\\Player\\Sword\\Attack\\Attack2.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerAttack2Mtrl", spriteMateiral);

		 }

		 // >> Attack 3
		{
		 std::shared_ptr<Texture> texture
			 = Resources::Load<Texture>(L"S_PlayerAttack3", L"..\\Resources\\Texture\\Player\\Sword\\Attack\\Attack3.png");

		 // === Material 생성
		 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
		 spriteMateiral->SetShader(AnimShader);
		 spriteMateiral->SetTexture(texture);
		 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

		 Resources::Insert(L"S_PlayerAttack3Mtrl", spriteMateiral);

		 }


		// >> SP Attack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerspAttack", L"..\\Resources\\Texture\\Player\\Sword\\spAttack\\spAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerspAttackMtrl", spriteMateiral);

		 }

		 // >> Overloading
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"S_PlayerOverloading", L"..\\Resources\\Texture\\Player\\Sword\\Overload\\Overload.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerOverloadingMtrl", spriteMateiral);

		 }



		 // 아이템 오버로드 설명
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"overload_explain", L"..\\Resources\\Texture\\UI\\Etc\\overload_explain.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"overload_explaingMtrl", spriteMateiral);

		 }


#pragma endregion 

#pragma region Player [Bow]
		// >> Idle
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"B_PlayerIdle", L"..\\Resources\\Texture\\Player\\Bow\\Idle\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"B_PlayerIdleMtrl", spriteMateiral);

		 }



		// >> Run
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"B_PlayerRun", L"..\\Resources\\Texture\\Player\\Bow\\Run\\Run.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"B_PlayerRunMtrl", spriteMateiral);
		 }


		// >> Dash
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"B_PlayerDash", L"..\\Resources\\Texture\\Player\\Bow\\Dash\\Dash.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"B_PlayerDashMtrl", spriteMateiral);

		 }

		// >> Jump
		// >> Fall
		// >> Landing
		// >> Hang
		// >> Hit
		// >> Stun
		// >> Die
		 // 덜그럭 거리는거 이상 없음
		{
		 std::shared_ptr<Texture> texture
			 = Resources::Load<Texture>(L"B_PlayerDie", L"..\\Resources\\Texture\\Player\\Bow\\Die\\Die.png");

		 // === Material 생성
		 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
		 spriteMateiral->SetShader(AnimShader);
		 spriteMateiral->SetTexture(texture);
		 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

		 Resources::Insert(L"B_PlayerDieMtrl", spriteMateiral);

		 }
		// >> Guard
		
		// >> Attack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"B_PlayerAttack", L"..\\Resources\\Texture\\Player\\Bow\\Attack\\Attack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"B_PlayerAttackMtrl", spriteMateiral);

		 }
		// >> SP Attack




#pragma endregion 

#pragma region Player [Pistol]
		// >> Idle
		// >> Run
		// >> Dash
		// >> Jump
		// >> Fall
		// >> Landing
		// >> Hang
		// >> Hit
		// >> Stun
		// >> Die
		// >> Guard
		// >> Attack
		// >> SP Attack

		// >> Idle

		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerIdle", L"..\\Resources\\Texture\\Player\\Pistol\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"P_PlayerIdleMtrl", spriteMateiral);

		 }

		 // >> Run
		 // 
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerRun", L"..\\Resources\\Texture\\Player\\Pistol\\Run.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerRunMtrl", spriteMateiral);

		 }
		 // >> Jump (fall 안합친거)
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerJump", L"..\\Resources\\Texture\\Player\\Pistol\\Jump.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerJumpMtrl", spriteMateiral);

		 }
		 // >> Fall
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerFall", L"..\\Resources\\Texture\\Player\\Pistol\\Fall.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerFallMtrl", spriteMateiral);

		 }
		 //// >> Landing
		 //{
			// std::shared_ptr<Texture> texture
			//	 = Resources::Load<Texture>(L"P_PlayerLanding", L"..\\Resources\\Texture\\Player\\Pistol\\Landing.png");

			// // === Material 생성
			// std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			// spriteMateiral->SetShader(AnimShader);
			// spriteMateiral->SetTexture(texture);
			// spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			// Resources::Insert(L"P_PlayerLandingMtrl", spriteMateiral);

		 //}
		 //// >> Hang
		 //{
			// std::shared_ptr<Texture> texture
			//	 = Resources::Load<Texture>(L"S_PlayerHang", L"..\\Resources\\Texture\\Player\\Pistol\\Hang.png");

			// // === Material 생성
			// std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			// spriteMateiral->SetShader(AnimShader);
			// spriteMateiral->SetTexture(texture);
			// spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			// Resources::Insert(L"S_PlayerHangMtrl", spriteMateiral);

		 //}
		 // >> Dash
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerDash", L"..\\Resources\\Texture\\Player\\Pistol\\Dash.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerDashMtrl", spriteMateiral);

		 }
		 //// >> Stun
		 //{
			// std::shared_ptr<Texture> texture
			//	 = Resources::Load<Texture>(L"S_PlayerStun", L"..\\Resources\\Texture\\Player\\Pistol\\Stun.png");

			// // === Material 생성
			// std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			// spriteMateiral->SetShader(AnimShader);
			// spriteMateiral->SetTexture(texture);
			// spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			// Resources::Insert(L"S_PlayerStunMtrl", spriteMateiral);

		 //}


		 // >> Hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerHit", L"..\\Resources\\Texture\\Player\\Pistol\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_HitMtrl", spriteMateiral);

		 }





		 //// >> Die
		 //{
			// std::shared_ptr<Texture> texture
			//	 = Resources::Load<Texture>(L"S_PlayerDie", L"..\\Resources\\Texture\\Player\\Pistol\\Die.png");

			// // === Material 생성
			// std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			// spriteMateiral->SetShader(AnimShader);
			// spriteMateiral->SetTexture(texture);
			// spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			// Resources::Insert(L"S_PlayerDieMtrl", spriteMateiral);

		 //}
		 // >> Guard
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerGuard", L"..\\Resources\\Texture\\Player\\Pistol\\Guard.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerGuardMtrl", spriteMateiral);

		 }
		 // >> Attack 1,2
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerAttack1,2", L"..\\Resources\\Texture\\Player\\Pistol\\ATTACK1.2 105 57 4.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerAttack1Mtrl", spriteMateiral);

		 }

		 // >> Attack 3
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerAttack3", L"..\\Resources\\Texture\\Player\\Pistol\\ATTACK3.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerAttack3Mtrl", spriteMateiral);

		 }


		 // >> SP Attack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerspAttack", L"..\\Resources\\Texture\\Player\\Pistol\\spAttack 105 57 15.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerspAttackMtrl", spriteMateiral);

		 }

		 // >> OverLoad Ready
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerOverLoadReady", L"..\\Resources\\Texture\\Player\\Pistol\\OverloadReady.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerOverLoadReadyMtrl", spriteMateiral);

		 }

		 // >> OverLoading
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerOverLoading", L"..\\Resources\\Texture\\Player\\Pistol\\Overloading.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerOverLoadingMtrl", spriteMateiral);

		 }

		 // >> OverLoad End
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerOverLoadEnd", L"..\\Resources\\Texture\\Player\\Pistol\\OverloadEnd.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerOverLoadEndMtrl", spriteMateiral);

		 }


		 // >> overloading 합본 
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"P_PlayerOverLoadingAdd", L"..\\Resources\\Texture\\Player\\Pistol\\Overloading(169 134 17).png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"P_PlayerOverLoadingAddMtrl", spriteMateiral);

		 }


		 // Bullet 종류 큰거
		 {

			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Pistol_BulletBig", L"..\\Resources\\Texture\\Player\\Pistol\\Weapon_Pistol_Object_BigBullet.png");

			 // == Material 생성(위에서 만든 shader와 texture를 이용)
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Pistol_BulletBigMtrl", spriteMateiral);

		 }


		 {

			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Pistol_BulletSmall", L"..\\Resources\\Texture\\Player\\Pistol\\Weapon_Pistol_Object_Bullet.png");

			 // == Material 생성(위에서 만든 shader와 texture를 이용)
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Pistol_BulletSmallMtrl", spriteMateiral);

		 }


#pragma endregion 



#pragma region Player [Gauntlet]
		 // >> Idle
		 // >> Run
		 // >> Dash
		 // >> Jump
		 // >> Fall
		 // >> Landing
		 // >> Hang
		 // >> Hit
		 // >> Stun
		 // >> Die
		 // >> Guard
		 // >> Attack
		 // >> SP Attack

		 // >> Idle

		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerIdle", L"..\\Resources\\Texture\\Player\\Gauntlet\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"G_PlayerIdleMtrl", spriteMateiral);

		 }

		 // >> Run
		 // 
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerRun", L"..\\Resources\\Texture\\Player\\Gauntlet\\Run.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_PlayerRunMtrl", spriteMateiral);

		 }
		 // >> Jump (fall 안합친거)
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerJump", L"..\\Resources\\Texture\\Player\\Gauntlet\\Jump.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_PlayerJumpMtrl", spriteMateiral);

		 }
		 // >> Fall
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerFall", L"..\\Resources\\Texture\\Player\\Gauntlet\\Fall.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_PlayerFallMtrl", spriteMateiral);

		 }
		 //// >> Landing
		 //{
			// std::shared_ptr<Texture> texture
			//	 = Resources::Load<Texture>(L"G_PlayerLanding", L"..\\Resources\\Texture\\Player\\Gauntlet\\Landing.png");

			// // === Material 생성
			// std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			// spriteMateiral->SetShader(AnimShader);
			// spriteMateiral->SetTexture(texture);
			// spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			// Resources::Insert(L"G_PlayerLandingMtrl", spriteMateiral);

		 //}
		 //// >> Hang
		 //{
			// std::shared_ptr<Texture> texture
			//	 = Resources::Load<Texture>(L"G_PlayerHang", L"..\\Resources\\Texture\\Player\\Gauntlet\\Hang.png");

			// // === Material 생성
			// std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			// spriteMateiral->SetShader(AnimShader);
			// spriteMateiral->SetTexture(texture);
			// spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			// Resources::Insert(L"G_PlayerHangMtrl", spriteMateiral);

		 //}
		 // >> Dash
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerDash", L"..\\Resources\\Texture\\Player\\Gauntlet\\Dash.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_PlayerDashMtrl", spriteMateiral);

		 }
		 //// >> Stun
		 //{
			// std::shared_ptr<Texture> texture
			//	 = Resources::Load<Texture>(L"G_PlayerStun", L"..\\Resources\\Texture\\Player\\Gauntlet\\Stun.png");

			// // === Material 생성
			// std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			// spriteMateiral->SetShader(AnimShader);
			// spriteMateiral->SetTexture(texture);
			// spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			// Resources::Insert(L"G_PlayerStunMtrl", spriteMateiral);

		 //}


		 // >> Hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerHit", L"..\\Resources\\Texture\\Player\\Gauntlet\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_HitMtrl", spriteMateiral);

		 }





		 //// >> Die
		 //{
			// std::shared_ptr<Texture> texture
			//	 = Resources::Load<Texture>(L"G_PlayerDie", L"..\\Resources\\Texture\\Player\\Gauntlet\\Die.png");

			// // === Material 생성
			// std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			// spriteMateiral->SetShader(AnimShader);
			// spriteMateiral->SetTexture(texture);
			// spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			// Resources::Insert(L"G_PlayerDieMtrl", spriteMateiral);

		 //}
		 // >> Guard
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerGuard", L"..\\Resources\\Texture\\Player\\Gauntlet\\Guard.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_PlayerGuardMtrl", spriteMateiral);

		 }
		 // >> Attack 1
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerAttack1", L"..\\Resources\\Texture\\Player\\Gauntlet\\Attack1.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_PlayerAttack1Mtrl", spriteMateiral);

		 }

		 // >> Attack 2
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerAttack2", L"..\\Resources\\Texture\\Player\\Gauntlet\\Attack2.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_PlayerAttack2Mtrl", spriteMateiral);

		 }

		 // >> Attack 3
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerAttack3", L"..\\Resources\\Texture\\Player\\Gauntlet\\Attack3.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_PlayerAttack3Mtrl", spriteMateiral);

		 }


		 // >> SP Attack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_PlayerspAttack", L"..\\Resources\\Texture\\Player\\Gauntlet\\spAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_PlayerspAttackMtrl", spriteMateiral);

		 }

		 // >> OverLoad attack 1 
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_Playeroverload_attack1", L"..\\Resources\\Texture\\Player\\Gauntlet\\Overload_Attack1(8.105.57).png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_Playeroverload_attack1Mtrl", spriteMateiral);

		 }


		 // >> OverLoad attack 2
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"G_Playeroverload_attack2", L"..\\Resources\\Texture\\Player\\Gauntlet\\Overload_Attack2(8.105.57).png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"G_Playeroverload_attack2Mtrl", spriteMateiral);

		 }


#pragma endregion 



#pragma region Monster [StoneEye]

		// >> Idle
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"StoneEyeIdle", L"..\\Resources\\Texture\\Monster\\StoneEye\\Idle\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"StoneEyeIdleMtrl", spriteMateiral);

		 }

		// >> Hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"StoneEyeHit", L"..\\Resources\\Texture\\Monster\\StoneEye\\Hit\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"StoneEyeHitMtrl", spriteMateiral);

		 }

		// >> Stun
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"StoneEyeStun", L"..\\Resources\\Texture\\Monster\\StoneEye\\Stun\\Stun.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"StoneEyeStunMtrl", spriteMateiral);

		 }

		// >> Die
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"StoneEyeDie", L"..\\Resources\\Texture\\Monster\\StoneEye\\Die\\Die.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"StoneEyeDieMtrl", spriteMateiral);

		 }

		// >> NearAttack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"StoneEyeNearAttack", L"..\\Resources\\Texture\\Monster\\StoneEye\\NearAttack\\NearAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"StoneEyeNearAttackMtrl", spriteMateiral);

		 }

		 // Near Attack Bullet
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"StoneEyeBorrow", L"..\\Resources\\Texture\\Monster\\StoneEye\\NearAttack\\Projectile.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(noCullingSpriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"StoneEyeBorrowMtrl", spriteMateiral);

		 }

		// >> Far Attack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"StoneEyeFarAttack", L"..\\Resources\\Texture\\Monster\\StoneEye\\FarAttack\\FarAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"StoneEyeFarAttackMtrl", spriteMateiral);

		 }

#pragma endregion 


#pragma region Monster [SkeletonArcher]

		 // >> Idle
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Archer_Idle", 
					 L"..\\Resources\\Texture\\Monster\\SkeletonArcher\\Idle\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Archer_IdleMtrl", spriteMateiral);

		 }

		 // >> Run
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Archer_Run",
					 L"..\\Resources\\Texture\\Monster\\SkeletonArcher\\Run\\Run.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Archer_RunMtrl", spriteMateiral);

		 }


		 // >> Hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Archer_Hit",
					 L"..\\Resources\\Texture\\Monster\\SkeletonArcher\\Hit\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Archer_HitMtrl", spriteMateiral);

		 }

		 // >> Die
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Archer_Die",
					 L"..\\Resources\\Texture\\Monster\\SkeletonArcher\\Die\\Die.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Archer_DieMtrl", spriteMateiral);

		 }

		 // >> Far Attack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Archer_FarAttack",
					 L"..\\Resources\\Texture\\Monster\\SkeletonArcher\\FarAttack\\FarAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Archer_FarAttackMtrl", spriteMateiral);

		 }

		 // Far Attack Arrow
		 {
		std::shared_ptr<Texture> texture
			= Resources::Load<Texture>(L"Archer_Arrow",
				L"..\\Resources\\Texture\\Monster\\SkeletonArcher\\FarAttack\\Enemy_SkeletonArcher_Object_Arrow.png");

		 // === Material 생성
		 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
		 spriteMateiral->SetShader(noCullingSpriteShader);
		 spriteMateiral->SetTexture(texture);
		 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

		 Resources::Insert(L"Archer_ArrowMtrl", spriteMateiral);

		 }


		 // >> Near Attack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Archer_NearAttack",
					 L"..\\Resources\\Texture\\Monster\\SkeletonArcher\\NearAttack\\NearAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Archer_NearAttackMtrl", spriteMateiral);

		 }


		 // >> Stun
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Archer_Stun",
					 L"..\\Resources\\Texture\\Monster\\SkeletonArcher\\Stun\\Stun.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Archer_StunMtrl", spriteMateiral);

		 }



#pragma endregion 


#pragma region Monster [SkeletonLizard]
		 // >> Idle
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Lizard_Idle",
					 L"..\\Resources\\Texture\\Monster\\SkeletonLizard\\Idle\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Lizard_IdleMtrl", spriteMateiral);

		 }

		 // >> Run
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Lizard_Run",
					 L"..\\Resources\\Texture\\Monster\\SkeletonLizard\\Run\\Run.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Lizard_RunMtrl", spriteMateiral);

		 }


		 // >> NearAttack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Lizard_NearAttack",
					 L"..\\Resources\\Texture\\Monster\\SkeletonLizard\\NearAttack\\NearAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Lizard_NearAttackMtrl", spriteMateiral);

		 }

		 // >> Hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Lizard_Hit",
					 L"..\\Resources\\Texture\\Monster\\SkeletonLizard\\Hit\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Lizard_HitMtrl", spriteMateiral);

		 }

		 // >> Stun
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Lizard_Stun",
					 L"..\\Resources\\Texture\\Monster\\SkeletonLizard\\Stun\\Stun.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Lizard_StunMtrl", spriteMateiral);

		 }

		 // >> Die
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Lizard_Die",
					 L"..\\Resources\\Texture\\Monster\\SkeletonLizard\\Die\\Die.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Lizard_DieMtrl", spriteMateiral);

		 }


#pragma endregion 


#pragma region Monster [WoodGolem]

		 // >> Idle
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wood_Idle",
					 L"..\\Resources\\Texture\\Monster\\Wood\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wood_IdleMtrl", spriteMateiral);

		 }

		 // >> Run
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wood_Run",
					 L"..\\Resources\\Texture\\Monster\\Wood\\Run.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wood_RunMtrl", spriteMateiral);

		 }

		 // >> NearAttack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wood_NearAttack",
					 L"..\\Resources\\Texture\\Monster\\Wood\\NearAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wood_NearAttackMtrl", spriteMateiral);

		 }

		 // >> FarAttack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wood_FarAttack",
					 L"..\\Resources\\Texture\\Monster\\Wood\\FarAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wood_FarAttackMtrl", spriteMateiral);

		 }


		 // >> Hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wood_Hit",
					 L"..\\Resources\\Texture\\Monster\\Wood\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wood_HitMtrl", spriteMateiral);

		 }


		 // >> Stun
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wood_Stun",
					 L"..\\Resources\\Texture\\Monster\\Wood\\Stun.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wood_StunMtrl", spriteMateiral);

		 }

		 // >> Die
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wood_Die",
					 L"..\\Resources\\Texture\\Monster\\Wood\\Die.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wood_DieMtrl", spriteMateiral);

		 }




#pragma endregion 


#pragma region Monster [Wolf]
		 // >> Idle
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wolf_Idle",
					 L"..\\Resources\\Texture\\Monster\\Wolf\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wolf_IdleMtrl", spriteMateiral);
		 }


		 // >> Run
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wolf_Run",
					 L"..\\Resources\\Texture\\Monster\\Wolf\\Run.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wolf_RunMtrl", spriteMateiral);
		 }


		 // >> Hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wolf_Hit",
					 L"..\\Resources\\Texture\\Monster\\Wolf\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wolf_HitMtrl", spriteMateiral);
		 }


		 // >> NearAttack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wolf_NearAttack",
					 L"..\\Resources\\Texture\\Monster\\Wolf\\NearAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wolf_NearAttackMtrl", spriteMateiral);
		 }

		 // >> FarAttack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wolf_FarAttack",
					 L"..\\Resources\\Texture\\Monster\\Wolf\\FarAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wolf_FarAttackMtrl", spriteMateiral);
		 }


		 // >> Stun
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wolf_Stun",
					 L"..\\Resources\\Texture\\Monster\\Wolf\\Stun.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wolf_StunMtrl", spriteMateiral);
		 }

		 // >> Die
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Wolf_Die",
					 L"..\\Resources\\Texture\\Monster\\Wolf\\Die.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Wolf_DieMtrl", spriteMateiral);
		 }

#pragma endregion 


#pragma region Monster [Zombie]
		 // >> Idle
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Zombie_Idle",
					 L"..\\Resources\\Texture\\Monster\\Zombie\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Zombie_IdleMtrl", spriteMateiral);
		 }

		 // Run
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Zombie_Run",
					 L"..\\Resources\\Texture\\Monster\\Zombie\\Run.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Zombie_RunMtrl", spriteMateiral);
		 }


		 // Hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Zombie_Hit",
					 L"..\\Resources\\Texture\\Monster\\Zombie\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Zombie_HitMtrl", spriteMateiral);
		 }

		 // Stun
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Zombie_Stun",
					 L"..\\Resources\\Texture\\Monster\\Zombie\\Stun.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Zombie_StunMtrl", spriteMateiral);
		 }

		 // NearAttack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Zombie_NearAttack",
					 L"..\\Resources\\Texture\\Monster\\Zombie\\NearAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Zombie_NearAttackMtrl", spriteMateiral);
		 }


		 // Far Attack
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Zombie_FarAttack",
					 L"..\\Resources\\Texture\\Monster\\Zombie\\FarAttack.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Zombie_FarAttackMtrl", spriteMateiral);
		 }


		 // Die
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Zombie_Die",
					 L"..\\Resources\\Texture\\Monster\\Zombie\\Die.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Zombie_DieMtrl", spriteMateiral);
		 }




#pragma endregion 

#pragma region Boss [Wolf]

		 // Idle (272 * 271 . 총 9개)
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_Idle",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_IdleMtrl", spriteMateiral);
		 }

		 // Spawn
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_Spawn",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Spawn.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_SpawnMtrl", spriteMateiral);
		 }

	
		 // Breath Attack Wolf Start
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_BreathAttackStart",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\BreathAttackStart.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_BreathAttackStartMtrl", spriteMateiral);
		 }

		 // Breath Attacking Wolf
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_BreathAttacking",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Breathing.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_BreathAttackingMtrl", spriteMateiral);
		 }

		 // Breath Attack Wolf End
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_BreathAttackEnd",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\BreathEnd.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_BreathAttackEndMtrl", spriteMateiral);
		 }




		 // BreathAttack Effect Start
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_BreathEffectStart",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\BreathEffectStart.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_BreathEffectStartMtrl", spriteMateiral);
		 }

		 // BreathAttack Effect End
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_BreathEffectEnd",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\BreathEffectEnd.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_BreathEffectEndMtrl", spriteMateiral);
		 }

		 // Dash
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_Dash",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Dash.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_DashMtrl", spriteMateiral);
		 }

		 // Die
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_Die",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Die.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_DieMtrl", spriteMateiral);
		 }


		 // Hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_Hit",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_HitMtrl", spriteMateiral);
		 }

		 // Howling Start
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_HowlingStart",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Howling_Start.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_HowlingStartMtrl", spriteMateiral);
		 }

		 // Howling
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_Howling",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Howling.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_HowlingMtrl", spriteMateiral);
		 }

		 // Howling End
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_HowlingEnd",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Howling_End.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_HowlingEndMtrl", spriteMateiral);
		 }



		 // HowlingEffect
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_HowlingEffect",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\HowlingEffect.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_HowlingEffectMtrl", spriteMateiral);
		 }

		 // HowlingPostioin
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_HowlingPostioin",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\HowlingPostioin.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_HowlingPostioinMtrl", spriteMateiral);
		 }

		 // MoveAppear
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_MoveAppear",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\MoveAppear.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_MoveAppearMtrl", spriteMateiral);
		 }

		 // Move Disappear
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_MoveDissappear",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\MoveDissappear.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_MoveDissappearMtrl", spriteMateiral);
		 }

		 // Shadow
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_Shadow",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Shadow.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_ShadowMtrl", spriteMateiral);
		 }

		 // ShadowEffect
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_ShadowEffect",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\ShadowEffect.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_ShadowEffectMtrl", spriteMateiral);
		 }

		 // Storm1
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_Storm1",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Storm1.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_Storm1Mtrl", spriteMateiral);
		 }

		 // Storm2
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_Storm2",
					 L"..\\Resources\\Texture\\Boss\\Wolf\\Storm2.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Boss_Wolf_Storm2Mtrl", spriteMateiral);
		 }

		 // ====== hit 관련

		 // 착지 hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_LandingHit",
					 L"..\\Resources\\Texture\\Boss\\wolf_Landing_Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Boss_Wolf_LandingHitMtrl", spriteMateiral);
		 }

		 // 포션 hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_PoistionHit",
					 L"..\\Resources\\Texture\\Boss\\positionHIt.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Boss_Wolf_PoistionHitMtrl", spriteMateiral);
		 }


		 // 보스 전체 맵 hit
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss_Wolf_HitFull",
					 L"..\\Resources\\Texture\\Boss\\wolfHitFull.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Boss_Wolf_HitFullMtrl", spriteMateiral);
		 }





#pragma endregion 


#pragma region Boss [Goddness]


		 // hit 관련
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_beam_hit",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Effect\\beam_hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Boss2_beam_hit_Mtrl", spriteMateiral);

		 }

		 // Beam 관련 742 406
		 
		 // 1
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_beam_Start",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\ShieldBeam\\Beam_Start.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Boss2_beam_Start_Mtrl", spriteMateiral);

		 }

		 // 7
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_beam_Ing",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\ShieldBeam\\Beam_Ing.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Boss2_beam_Ing_Mtrl", spriteMateiral);

		 }

		 // 5
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_beam_End",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\ShieldBeam\\Beam_End.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Boss2_beam_End_Mtrl", spriteMateiral);

		 }




		 // ======================================
		 // BackGround
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_BG_0",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Background\\BG_2_0.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_BG_0_Mtrl", spriteMateiral);

		 }

		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_BG_1",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Background\\BG_2_1.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_BG_1_Mtrl", spriteMateiral);

		 }

		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_BG_2",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Background\\BG_2_2.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);


			 Resources::Insert(L"Boss2_BG_2_Mtrl", spriteMateiral);

		 }


		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_BG_3",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Background\\BG_2_Object_28.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);


			 Resources::Insert(L"Boss2_BG_3_Mtrl", spriteMateiral);

		 }

		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_BG_4",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Background\\BG_2_3.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);


			 Resources::Insert(L"Boss2_BG_4_Mtrl", spriteMateiral);

		 }


		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_Tile",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\tile2.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);


			 Resources::Insert(L"Boss2_Tile_Mtrl", spriteMateiral);

		 }

		 // 횃불

		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Fire_Base",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Fire\\Base Edit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);


			 Resources::Insert(L"Fire_Base_Mtrl", spriteMateiral);

		 }

		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Fire_turnonstart",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Fire\\TurnOnStart.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			

			 Resources::Insert(L"Fire_turnonstart_Mtrl", spriteMateiral);

		 }


		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Fire_turnon",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Fire\\TurnOn.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Fire_turnon_Mtrl", spriteMateiral);

		 }


		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Fire_turnoff",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Fire\\TurnOff.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);


			 Resources::Insert(L"Fire_turnoff_Mtrl", spriteMateiral);

		 }




		 // ============================================
		 // >> Idle
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2-1_Idle",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase1\\Idle-eidt.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2-1_Idle_Mtrl", spriteMateiral);

		 }

		 // >> Stomp
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2-1_Stomp",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase1\\Stomp.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2-1_Stomp_Mtrl", spriteMateiral);

		 }

		 // >> Energyball Start
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2-1_EnergyballStart",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase1\\Energyball_Ready.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2-1_Stomp_Mtrl", spriteMateiral);

		 }

		 // >> Energyball End
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2-1_EnergyballEnd",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase1\\Energyball_End.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2-1_Stomp_Mtrl", spriteMateiral);

		 }

		 // >> Die
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2-1_Die",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase1\\Phase1_Die.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Phase1_Die_Mtrl", spriteMateiral);

		 }

		 // >> stomp effect
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2-1_stompEffect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase1\\Stomp_Effect.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2-1_stompEffect_Mtrl", spriteMateiral);

		 }

		 // >> stomp hit Effect 141 32
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"stompHitGround",
					 L"..\\Resources\\Texture\\Boss\\StompHIt.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"stompHitGroundt_Mtrl", spriteMateiral);

		 }

		 // 바람 불 때 막는 용도 14 26 132
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_GroundWind",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\Phase2\\Ground\\Wind\\GroundWind.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_GroundWind_Mtrl", spriteMateiral);

		 }



		 // ========== 에너지볼
		 // >> Energy Ball Big (패링x) : 빨간거 
		 // 소환
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_1_Energyball_B_noParrying_Spawn_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\AttackCircle\\Big\\noParrying\\Spawn\\Spawn.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_1_Energyball_B_noParrying_Spawn_Effect_Mtrl", spriteMateiral);

		 }


		 // 발사
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_1_Energyball_B_noParrying_Energying_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\AttackCircle\\Big\\noParrying\\Energying\\AfterSpawn.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_1_Energyball_B_noParrying_Energying_Effect_Mtrl", spriteMateiral);

		 }

		 // dead
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_1_Energyball_B_noParrying_End_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\AttackCircle\\Big\\noParrying\\End.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_1_Energyball_B_noParrying_End_Effect_Mtrl", spriteMateiral);

		 }

		 // >> Energy Ball Big (패링0) : 노란거
		 // 소환
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_1_Energyball_B_Parrying_Spawn_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\AttackCircle\\Big\\Parrying\\Spawn\\Spawn.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_1_Energyball_B_Parrying_Spawn_Effect_Mtrl", spriteMateiral);

		 }

		 // 발사
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_1_Energyball_B_Parrying_Energying_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\AttackCircle\\Big\\Parrying\\Energying\\AfterSpawn.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_1_Energyball_B_Parrying_Energying_Effect_Mtrl", spriteMateiral);

		 }

	

		 // >> Energy Ball Small (패링x) : 빨간거
		 // 소환
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_1_Energyball_S_noParrying_Spawn_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\AttackCircle\\Small\\noParrying\\Spawn\\Spawn.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_1_Energyball_S_noParrying_Spawn_Effect_Mtrl", spriteMateiral);

		 }

		 // 발사
				
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_1_Energyball_S_noParrying_AfterSpawn_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\AttackCircle\\Small\\noParrying\\AfterSpawn\\AfterSpawn.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_1_Energyball_S_noParrying_AfterSpawn_Effect_Mtrl", spriteMateiral);

		 }

		 // >> Energy Ball Small (패링0) : 노란거
		 // 소환
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_1_Energyball_S_Parrying_Spawn_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\AttackCircle\\Small\\Parrying\\Spawn\\Spawn.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_1_Energyball_S_Parrying_Spawn_Effect_Mtrl", spriteMateiral);

		 }

		 // 발사
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_1_Energyball_S_Parrying_AfterSpawn_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\AttackCircle\\Small\\Parrying\\AfterSpawn\\AfterSpawn.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_1_Energyball_S_Parrying_AfterSpawn_Effect_Mtrl", spriteMateiral);

		 }

		 // 끝
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_1_Energyball_S_Parrying_End_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\CommonEffect\\AttackCircle\\Small\\Parrying\\End.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_1_Energyball_S_Parrying_End_Effect_Mtrl", spriteMateiral);

		 }



		 // =============================================
		 // 사람

		 // Idle 8 269 276
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Idle",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Idle\\Idle.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Idle_Mtrl", spriteMateiral);

		 }


		 // Move 앞으로 6
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_MoveFront",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Run\\Forward\\Forward.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_MoveFront_Mtrl", spriteMateiral);

		 }


		 // Move 뒤로 6
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_MoveBack",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Run\\Back\\Back.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_MoveBack_Mtrl", spriteMateiral);

		 }


		 // Hit 5 269 276
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Hit",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Hit\\Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Hit_Mtrl", spriteMateiral);

		 }


		 // Intro 17 269 276
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Intro",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Intro\\Intro.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Intro_Mtrl", spriteMateiral);

		 }


		 // Intro End 8 269 276
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_IntroEnd",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Intro\\IntroEnd.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_IntroEnd_Mtrl", spriteMateiral);

		 }



		 // *** Counter
		 // start 13 269 276
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Counter_Start",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Counter\\Start\\Counter_Start.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Counter_Start_Mtrl", spriteMateiral);

		 }

		 // Countering 5 269 276
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Countering",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Counter\\Countering.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Countering_Mtrl", spriteMateiral);

		 }

		 // Counter_End 3 269 276
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Counter_End",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Counter\\End\\Counter_End.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Counter_End_Mtrl", spriteMateiral);

		 }

		 // Counter_Hit 5 269 276 (카운터중에 플레이어에게 공격 받았을 경우) 
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Counter_Hit",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Counter\\Hit\\Counter_Hit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Counter_Hit_Mtrl", spriteMateiral);

		 }


		 // *** DiagonalAttack
		 // Common Ready 8 269 276 
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_DiagonalAttack_Ready",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\DiagonalAttack\\Common_Ready.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_DiagonalAttack_Ready_Mtrl", spriteMateiral);

		 }



		 // Vertical 4 
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_DiagonalAttack_Vertical",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\DiagonalAttack\\Vertical\\Verctical.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_DiagonalAttack_Vertical_Mtrl", spriteMateiral);

		 }

		 // Diagonal 4
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_DiagonalAttack_Diagonal",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\DiagonalAttack\\Diagonal\\Diagonal.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_DiagonalAttack_Diagonal_Mtrl", spriteMateiral);

		 }


		 // **** Energy ball
		  // Start 10
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Energyball_Start",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\EnergyBall\\Ready\\Energyball_Start.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Energyball_Start_Mtrl", spriteMateiral);

		 }

		 // -ing 6
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Energyballing",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\EnergyBall\\Energing.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Energyballing_Mtrl", spriteMateiral);

		 }


		 // End 3
		 { 
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Energyball_End",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\EnergyBall\\End\\Energyball_End.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Energyball_End_Mtrl", spriteMateiral);

		 }

		 // **** Ground Spear
		 // 18
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_GroundSpearPeople",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\GroundSpear\\GroundSpear.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_GroundSpearPeople_Mtrl", spriteMateiral);

		 }

		 // **** Shiled Beam
		// Ready 15
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_ShiledBeam_Ready",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\ShieldBeam\\ShiledBeamReady.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Boss2_2_ShiledBeam_Ready_Mtrl", spriteMateiral);

		 }

		 // ing 10 
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_ShiledBeam_ing",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\ShieldBeam\\ShiledBeam.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			 Resources::Insert(L"Boss2_2_ShiledBeam_ing_Mtrl", spriteMateiral);

		 }

		 // End 9
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_ShiledBeam_End",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\ShieldBeam\\ShiledBeam_End.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);
			 Resources::Insert(L"Boss2_2_ShiledBeam_End_Mtrl", spriteMateiral);

		 }


		 // **** SummonSpear
	     // 17
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_SummonSpear",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\SummonSpear\\Summon_Spear.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_SummonSpear_Mtrl", spriteMateiral);

		 }


		 // **** Die
		// 43
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Die",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Die\\Die.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Die_Mtrl", spriteMateiral);

		 }



		 // ========
		 // 이펙트

		 // Ground Sword 10 89 63
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_GroundSword_Effect",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\CommonEffect\\GroundSword\\GroundSword.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_GroundSword_Effect_Mtrl", spriteMateiral);

		 }


		 // **** sword 
		 // base 89 114 1
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_Sword_Left_To_Right_Effect",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\CommonEffect\\Sword\\4545.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_Sword_Left_To_Right_Effect_Mtrl", spriteMateiral);


		 }

		 // base 89 114 1
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_Sword_Right_To_Left_Effect",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\CommonEffect\\Sword\\566.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_Sword_Right_To_Left_Effect_Mtrl", spriteMateiral);


		 }


		 // base 89 114 1
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_Sword_Down_To_Up_Effect",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\CommonEffect\\Sword\\cvdf.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_Sword_Down_To_Up_Effect_Mtrl", spriteMateiral);


		 }


		 // base 89 114 1
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_Sword_Up_To_Down_Effect",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\CommonEffect\\Sword\\2.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_Sword_Up_To_Down_Effect_Mtrl", spriteMateiral);


		 }





		 // wall hit 89 38 4 (벽에 부딪칠 때)
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_Sword_wallHit_Effect",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\CommonEffect\\Sword\\wallHit\\wallHit.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_Sword_wallHit_Effectt_Mtrl", spriteMateiral);

		 }





		 // Dir 672 9
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Dir_LeftToRight",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\CommonEffect\\Sword\\DirLeftToRight.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Dir_LeftToRight_Mtrl", spriteMateiral);


		 }

		 // Dir 672 9
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Dir_RightToLeft",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\CommonEffect\\Sword\\DirRightToLeft.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Dir_RightToLeft_Mtrl", spriteMateiral);


		 }

		 // Dir 9 672
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Dir_UpToDown",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\CommonEffect\\Sword\\DirUpToDown.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Dir_UpToDown_Mtrl", spriteMateiral);


		 }

		 // Dir 9 672
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Dir_DownToUp",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\CommonEffect\\Sword\\DirDownToUp.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(spriteShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Dir_DownToUp_Mtrl", spriteMateiral);


		 }


		 // *** Ground platform
		 // 그냥 바닥  12 100 24
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_GroundFloatform",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\Phase2\\Ground\\Groundplatform.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_GroundFloatform_Mtrl", spriteMateiral);

		 }

		  // 100 24 
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_GroundPlatformImage",
					 L"..\\Resources\\Texture\\Boss\\\Goddness\\Phase2\\Ground\\Boss_Goddess_Object_Platform_11.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_GroundPlatformImage_Mtrl", spriteMateiral);

		 }


		 // 사람 등 뒤에 달린 이펙트 10 142 142
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_Back_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Effect\\BackEffect.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_Back_Effect_Mtrl", spriteMateiral);

		 }

		 // ground spear 땅에 내려꽂는 창 6 93 120
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"Boss2_2_GroundSpear_Effect",
					 L"..\\Resources\\Texture\\Boss\\Goddness\\Phase2\\Boss\\Effect\\GroundSpear\\GroundSpear.png");

			 // === Material 생성
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"Boss2_2_GroundSpear_Effect_Mtrl", spriteMateiral);

		 }

#pragma endregion 








#pragma region Scene [Title]
		//// ===== 타이틀씬
		//// BG
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Title01", L"..\\Resources\\Texture\\Title\\Main_BG.png");

			// === Material 생성
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"TitleBG", spriteMateiral);

		}

#pragma endregion 




#pragma region Scene [Library]
		//// ==== 도서관 씬
		//// 밤하늘 배경
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Moon1", L"..\\Resources\\Texture\\Library\\BG_0_0.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
		

			Resources::Insert(L"MoonMtrl", spriteMateiral);
		}

		// 도서관
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Library", L"..\\Resources\\Texture\\Library\\LibraryMap.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);
			

			Resources::Insert(L"LibraryMtrl", spriteMateiral);

		}


		// 나무
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"WeaponTree", L"..\\Resources\\Texture\\Library\\Tree.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);
		

			Resources::Insert(L"WeaponTreeMtrl", spriteMateiral);
		}

		// 무기 칼
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MiniWeaponSword", L"..\\Resources\\Texture\\Library\\w1.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"w1Mtrl", spriteMateiral);
		}


		// 무기 총
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MiniWeaponPistol", L"..\\Resources\\Texture\\Library\\w2.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"w2Mtrl", spriteMateiral);
		}


		// 무기 건틀렛
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MiniWeaponBorrow", L"..\\Resources\\Texture\\Library\\w3.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"w3Mtrl", spriteMateiral);
		}
#pragma endregion 




#pragma region Scene [Entrance]
		// == 던전가는 입구 배경
		// 배경
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Entrance", L"..\\Resources\\Texture\\Library\\EntrenceMap.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"EntranceMtrl", spriteMateiral);
		}

		// 포탈
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Potal", L"..\\Resources\\Texture\\Library\\potal.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"PotalMtrl", spriteMateiral);
		}
#pragma endregion 




#pragma region Scene [Entry]
		// ==던전 진입 전 씬
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Entry", L"..\\Resources\\Texture\\Stage1\\entry2.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"EntryMtrl", spriteMateiral);
		}

		// == 수풀맆1
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Tree1", L"..\\Resources\\Texture\\Stage1\\BG_1_1.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);
	

			Resources::Insert(L"Tree1Mtrl", spriteMateiral);
		}


		//// == 수풀맆2
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Tree2", L"..\\Resources\\Texture\\Stage1\\BG_1_2.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Tree2Mtrl", spriteMateiral);
		}

		//// == 수풀맆3
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Tree3", L"..\\Resources\\Texture\\Stage1\\BG_1_3.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Tree3Mtrl", spriteMateiral);
		}

		//// == 수풀맆4
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Tree4", L"..\\Resources\\Texture\\Stage1\\BG_1_4.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Tree4Mtrl", spriteMateiral);
		}

		//// == 왼쪽 석상
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Moon1Statue", L"..\\Resources\\Texture\\Stage1\\212_124 Left.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Moon1StatueMtrl", spriteMateiral);
		}

		//// == 가운데 석상
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Moon2Statue", L"..\\Resources\\Texture\\Stage1\\BG_1_Object_39.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Moon2StatueMtrl", spriteMateiral);
		}


		//// == 오른쪽 석상
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Moon3Statue", L"..\\Resources\\Texture\\Stage1\\212_124 Right.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Moon3StatueMtrl", spriteMateiral);
		}
#pragma endregion 



#pragma region Scene [Stage1]
		// ==== stage 1
		// 둥근 달
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Moon2", L"..\\Resources\\Texture\\Stage1\\BG_1_0.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
		

			Resources::Insert(L"Moon2Mtrl", spriteMateiral);
		}

		// 맵
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Stage1", L"..\\Resources\\Texture\\Stage1\\Stage1.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Stage1Mtrl", spriteMateiral);
		}
#pragma endregion 




#pragma region scene [Boss 1 BackGround]
		// ===== 보스늑대
		// 보스 늑대 맵 타일 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Boss1Tile", L"..\\Resources\\Texture\\Boss\\wolfTile.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Boss1TileMtrl", spriteMateiral);
		}

		// 보스 늑대 맵 횃불
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"wolfFire", L"..\\Resources\\Texture\\Boss\\wolfFire.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"wolfFireMtrl", spriteMateiral);
		}

		// 보스 늑대 맵 상
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"WolfStatue", L"..\\Resources\\Texture\\Boss\\BossStatue.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"WolfStatueMtrl", spriteMateiral);
		}
#pragma endregion 



#pragma region Game UI
		// ==== UI
		// Letter Box UP
		{

			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"LetterBoxUp", L"..\\Resources\\Texture\\UI\\Etc\\letterBoxUp.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"LetterBoxUpMtrl", spriteMateiral);

		}

		// Letter Box Bottom
		{

			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"LetterBoxBottom", L"..\\Resources\\Texture\\UI\\Etc\\letterBoxBottom.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"LetterBoxBottomMtrl", spriteMateiral);

		}


		// Low UI
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UILowHP", L"..\\Resources\\Texture\\UI\\Etc\\UI_LowHP.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"UILowHPMtrl", spriteMateiral);
		}




		// WOLF BOSS 이름
		{

			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Boss1Name", L"..\\Resources\\Texture\\UI\\Etc\\Boss1Name.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"Boss1NameMtrl", spriteMateiral);

		}



		// goddness BOSS2 이름
		{

			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Boss2_2Name", L"..\\Resources\\Texture\\UI\\Etc\\Boss2_2Name.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"Boss2_2NameMtrl", spriteMateiral);

		}



		// 플레이어 Frame
		{

			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerFrame", L"..\\Resources\\Texture\\UI\\Player\\UI_MainPlayer_Frame.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"PlayerFrameMtrl", spriteMateiral);

		}

		// 몬스터 Frame
		{

			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MonsterFrame", L"..\\Resources\\Texture\\UI\\Monster\\UI_EnemyHp_Frame.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"MonsterFrameMtrl", spriteMateiral);

		}



		//(보스 HP 틀)
		{

			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BossHpFrame", L"..\\Resources\\Texture\\UI\\Boss\\UI_BossHp_Frame.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"BossHpFrameMtrl", spriteMateiral);

		}

		// -- HP바
		// 
		// 
		
		// 보스 hp바 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BossHPBar", L"..\\Resources\\Texture\\UI\\Boss\\UI_BossHp_Bar.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"BossHPBarMtrl", spriteMateiral);
		}

		// 몬스터 hp바 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MonsterHPBar", L"..\\Resources\\Texture\\UI\\Monster\\UI_EnemyHp_Bar.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"MonsterHPBarMtrl", spriteMateiral);
		}

		// 플레이어 hp바 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerBar", L"..\\Resources\\Texture\\UI\\Player\\HP\\UI_PlayerUI_Bar_Hp.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(progressShader);
			spriteMateiral->SetTexture(texture); // 0번 슬롯 사용 
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"PlayerBarMtrl", spriteMateiral);
		}

		// 플레이어 sp바 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerSPBar", L"..\\Resources\\Texture\\UI\\Player\\SP\\UI_PlayerUI_Bar_SpAttack.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(progressShader2);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"PlayerSPBarMtrl", spriteMateiral);
		}

		// 플레이어 Overload바 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerOverloadBar", L"..\\Resources\\Texture\\UI\\Player\\overload\\UI_PlayerUI_Bar_Overload.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"PlayerOverloadBarMtrl", spriteMateiral);
		}


		


#pragma endregion 


#pragma region Effect
		{
			//std::shared_ptr<Texture> texture
			//	= Resources::Load<Texture>(L"StunEffect", L"..\\Resources\\Texture\\Monster\\Effect\\Stun\\EnemyStun.png");

			//// == Material 생성(위에서 만든 shader와 texture를 이용)
			//std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			//spriteMateiral->SetShader(spriteShader);
			//spriteMateiral->SetTexture(texture);
			////spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			//Resources::Insert(L"StunEffectMtrl", spriteMateiral);
		}


		// UI 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"hitGround", L"..\\Resources\\Texture\\UI\\Etc\\hitGround.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"hitGroundMtrl", spriteMateiral);
		}


		// 플레이어 sword sp 이펙트 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"S_PlayerSPEffect", L"..\\Resources\\Texture\\Player\\Sword\\spAttack\\spSwordEffect.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"S_PlayerSPEffectMtrl", spriteMateiral);
		}


		// 플레이어 sword hit 이펙트 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"S_PlayerHitEffect", L"..\\Resources\\Texture\\Player\\Sword\\HitEffect(51 51).png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"S_PlayerHitEffectMtrl", spriteMateiral);
		}

		// 2. 피스톨
			// 플레이어 pistol attack 1,2 이펙트 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"P_PlayerAttack1_2_Effect", L"..\\Resources\\Texture\\Player\\Pistol\\ATTACK_EFFECT 51 55 4.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"P_PlayerAttack1_2_EffectMtrl", spriteMateiral);
		}

		// 플레이어 pistol attack 3 이펙트 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"P_PlayerAttack3_Effect", L"..\\Resources\\Texture\\Player\\Pistol\\ATTACK3_EFFECT 72 66 3.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"P_PlayerAttack3_EffectMtrl", spriteMateiral);
		}


		// 플레이어 pistol hit 이펙트 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"P_PlayerHit_Effect", L"..\\Resources\\Texture\\Player\\Pistol\\Effect_Weapon_Pistol_Hit(39-61).png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"P_PlayerHit_EffectMtrl", spriteMateiral);
		}


		// 플레이어 pistol spAttack 이펙트 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"P_PlayerspAttack_Effect", L"..\\Resources\\Texture\\Player\\Pistol\\spAttackEffect 86 67 4.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"P_PlayerspAttack_EffectMtrl", spriteMateiral);
		}


		// 플레이어 pistol overloading 이펙트 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"P_PlayerOverloading_Effect", L"..\\Resources\\Texture\\Player\\Pistol\\OverloadingEffect 88 157 2.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"P_PlayerOverloading_EffectMtrl", spriteMateiral);
		}


		// 플레이어 pistol overload End 이펙트 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"P_PlayerOverloadEnd_Effect", L"..\\Resources\\Texture\\Player\\Pistol\\OverloadingEndEffect 88 157.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"P_PlayerOverloadEnd_EffectMtrl", spriteMateiral);
		}

		// 플레이어 pistol overload circle 이펙트
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"P_PlayerOverloadCircle_Effect", L"..\\Resources\\Texture\\Player\\Pistol\\OverloadEffectCircle189 189 9.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"P_PlayerOverloadCircle_EffectMtrl", spriteMateiral);
		}

		// ================
		// 3. 건틀렛 관련 
		// 플레이어 건틀렛 SP Attack 1 이펙트
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"G_PlayerspAttack1_Effect", L"..\\Resources\\Texture\\Player\\Gauntlet\\spAttackEffect 1...194 50 6.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"G_PlayerspAttack1_EffectMtrl", spriteMateiral);
		}

		// 플레이어 건틀렛 SP Attack 2 이펙트
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"G_PlayerspAttack2_Effect", L"..\\Resources\\Texture\\Player\\Gauntlet\\spAttackEffect2.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"G_PlayerspAttack2_EffectMtrl", spriteMateiral);
		}

			// 플레이어 건틀렛 Overload 이펙트
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"G_PlayerOverload_Effect", L"..\\Resources\\Texture\\Player\\Gauntlet\\overloadEffect_36.38.10.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"G_PlayerOverload_EffectMtrl", spriteMateiral);
		}

		// =============
		// 4. 플레이어 공통 overload 관련 UI 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UI_OverloadScreen", L"..\\Resources\\Texture\\UI\\Etc\\UI_OverloadScreen.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"UI_OverloadScreenMtrl", spriteMateiral);
		}

		// 플레이어한테 띄워줄 overload effect 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerOverloadingStartEffect", L"..\\Resources\\Texture\\UI\\Player\\overload\\OverloadingStartEffect.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"PlayerOverloadingStartEffectMtrl", spriteMateiral);
		}

		// 플레이어한테 띄워줄 overloading 중일 떄의 effect 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerOverloadingEffect", L"..\\Resources\\Texture\\UI\\Player\\overload\\OverloadingEffect.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"PlayerOverloadingEffectMtrl", spriteMateiral);
		}


		// overload 화면에 띄워질 것 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Overload_LB", L"..\\Resources\\Texture\\UI\\Player\\overload\\LB.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"Overload_LBMtrl", spriteMateiral);
		}

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Overload_LT", L"..\\Resources\\Texture\\UI\\Player\\overload\\LT.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"Overload_LTMtrl", spriteMateiral);
		}

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Overload_RT", L"..\\Resources\\Texture\\UI\\Player\\overload\\RT.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"Overload_RTMtrl", spriteMateiral);
		}

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Overload_RB", L"..\\Resources\\Texture\\UI\\Player\\overload\\RB.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"Overload_RBMtrl", spriteMateiral);
		}

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Overload_BottomHorizion", L"..\\Resources\\Texture\\UI\\Player\\overload\\BottomHorizion.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"Overload_BottomHorizionMtrl", spriteMateiral);
		}


		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Overload_TopHorizion", L"..\\Resources\\Texture\\UI\\Player\\overload\\TopHorizion.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"Overload_TopHorizionMtrl", spriteMateiral);
		}

		// overload bar 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Overload_Bar", L"..\\Resources\\Texture\\UI\\Player\\overload\\UI_PlayerUI_Bar_Overload_no.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"Overload_BarMtrl", spriteMateiral);
		}


		// === guard 이펙트
	
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Player_GuardEffect", L"..\\Resources\\Texture\\Player\\Common\\PlayerParryEffect 24 55 5.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Player_GuardEffectMtrl", spriteMateiral);
		}



		// 키보드 자판
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Keyboard_A", L"..\\Resources\\Texture\\UI\\UI_KeySetting_a.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"Keyboard_A_Mtrl", spriteMateiral);
		}

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Keyboard_S", L"..\\Resources\\Texture\\UI\\UI_KeySetting_s.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"Keyboard_S_Mtrl", spriteMateiral);
		}

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Keyboard_D", L"..\\Resources\\Texture\\UI\\UI_KeySetting_d.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"Keyboard_D_Mtrl", spriteMateiral);
		}

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Keyboard_F", L"..\\Resources\\Texture\\UI\\UI_KeySetting_f.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"Keyboard_F_Mtrl", spriteMateiral);
		}

#pragma endregion 




#pragma region Item

		// overload 아이템 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Item_Overload", L"..\\Resources\\Texture\\Common\\Item\\Item_OverheatCore.png");

			// == Material 생성(위에서 만든 shader와 texture를 이용)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // 앞에 띄워야되는건 이걸로 

			Resources::Insert(L"Item_OverloadMtrl", spriteMateiral);
		}




#pragma endregion 

	}

	void LoadAudio()
	{
		// 배경음악
		Resources::Load<AudioClip>(L"Title_Bgm", L"..\\Resources\\Sound\\BGM_Main.wav");
		Resources::Load<AudioClip>(L"Library_Bgm", L"..\\Resources\\Sound\\BGM_Stage_0.wav");
		Resources::Load<AudioClip>(L"Stage1_Bgm", L"..\\Resources\\Sound\\BGM_Stage_1.wav");
		Resources::Load<AudioClip>(L"Boss1_Bgm", L"..\\Resources\\Sound\\BGM_VS_Lupus.wav");
		Resources::Load<AudioClip>(L"Boss2_Statue_Bgm", L"..\\Resources\\Sound\\BGM_VS_GoddessStatue.wav");
		Resources::Load<AudioClip>(L"Boss2_Goddess_Bgm", L"..\\Resources\\Sound\\BGM_VS_Goddess.wav");



		// ====== effect
		

		// 플레이어
		Resources::Load<AudioClip>(L"Player_Dash_Bgm", L"..\\Resources\\Sound\\SE_Player_Dash.wav");
		Resources::Load<AudioClip>(L"Player_GaurdHit_Bgm", L"..\\Resources\\Sound\\SE_Player_Guard_Hit.wav");
		Resources::Load<AudioClip>(L"Player_Hit_Bgm", L"..\\Resources\\Sound\\SE_Player_Hit.wav"); // 플레이어가 공격받았을 때
		Resources::Load<AudioClip>(L"Player_Jump_Bgm", L"..\\Resources\\Sound\\SE_Player_Jump.wav");
		Resources::Load<AudioClip>(L"Player_Overload_Bgm", L"..\\Resources\\Sound\\SE_Player_Overload.wav");
		Resources::Load<AudioClip>(L"Player_Parrying_Bgm", L"..\\Resources\\Sound\\SE_Player_Parrying.wav");
		Resources::Load<AudioClip>(L"Player_WeaponChange_Bgm", L"..\\Resources\\Sound\\SE_Player_WeaponChange.wav");
		
		// 플레이어 무기
		// 1. 건틀렛
		Resources::Load<AudioClip>(L"Gauntlet_Attack1_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Gauntlet_Attack.wav");
		Resources::Load<AudioClip>(L"Gauntlet_Attack2_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Gauntlet_Attack_3.wav");
		Resources::Load<AudioClip>(L"Gauntlet_Attack3_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Gauntlet_Attack_4.wav");

		Resources::Load<AudioClip>(L"Gauntlet_OverloadBuff_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Gauntlet_OverloadBuff.wav");

		Resources::Load<AudioClip>(L"Gauntlet_SPAttackReady_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Gauntlet_SPAttack_Ready.wav");
		Resources::Load<AudioClip>(L"Gauntlet_SPAttack_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Gauntlet_SPAttack.wav");
		Resources::Load<AudioClip>(L"Gauntlet_SPAttack2_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Gauntlet_SPAttack_.wav");


		// 2. 피스톨
		Resources::Load<AudioClip>(L"Pistol_Attack1_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Pistol_Attack.wav");
		Resources::Load<AudioClip>(L"Pistol_Attack1_2Bgm", L"..\\Resources\\Sound\\SE_Weapon_Pistol_Attack.wav");
		Resources::Load<AudioClip>(L"Pistol_Attack2_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Pistol_Attack_3.wav");
		Resources::Load<AudioClip>(L"Pistol_OverloadAttack_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Pistol_Attack_Overload.wav");


		// 3. 스워드
		Resources::Load<AudioClip>(L"Sword_Attack1_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Sword_Attack_0_0.wav");
		Resources::Load<AudioClip>(L"Sword_Attack2_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Sword_Attack_0_1.wav");
		Resources::Load<AudioClip>(L"Sword_Attack3_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Sword_Attack_3.wav");

		Resources::Load<AudioClip>(L"Sword_SPAttack_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Sword_SPAttack.wav");

		Resources::Load<AudioClip>(L"Sword_OverloadAttack1_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Sword_Attack_Overload_0.wav");
		Resources::Load<AudioClip>(L"Sword_OverloadAttack2_Bgm", L"..\\Resources\\Sound\\SE_Weapon_Sword_Attack_Overload_1.wav");



		// 기타 오브젝트
		Resources::Load<AudioClip>(L"Door_Open_Bgm", L"..\\Resources\\Sound\\SE_Object_Door_Open.wav");
		Resources::Load<AudioClip>(L"Door_Close_Bgm", L"..\\Resources\\Sound\\SE_Object_Door_Close.wav");


		// Enemy 공통 
		Resources::Load<AudioClip>(L"Enemy_Die_Bgm", L"..\\Resources\\Sound\\SE_Enemy_Die.wav");
		Resources::Load<AudioClip>(L"Enemy_Hit_Bgm", L"..\\Resources\\Sound\\SE_Enemy_Hit.wav");

		// ==== Enemy

		// 1. 스켈레톤 archer
		Resources::Load<AudioClip>(L"Archer_NearAttack_Bgm", L"..\\Resources\\Sound\\SE_Enemy_SkeletonArcher_Attack_0.wav");
		Resources::Load<AudioClip>(L"Archer_FarAttackReady_Bgm", L"..\\Resources\\Sound\\SE_Enemy_SkeletonArcher_Attack_1_Ready.wav");
		Resources::Load<AudioClip>(L"Archer_FarAttackFire_Bgm", L"..\\Resources\\Sound\\SE_Enemy_SkeletonArcher_Attack_1.wav");
		
		
		
		// 2. StoneEye
		Resources::Load<AudioClip>(L"Stoneeye_NearAttackReady_Bgm", L"..\\Resources\\Sound\\SE_Enemy_StoneEye_Attack_0_Ready.wav");
		Resources::Load<AudioClip>(L"Stoneeye_NearAttack_Bgm", L"..\\Resources\\Sound\\SE_Enemy_StoneEye_Attack_0.wav");
		Resources::Load<AudioClip>(L"Stoneeye_FarAttackReady_Bgm", L"..\\Resources\\Sound\\SE_Enemy_StoneEye_Attack_1_Ready.wav");
		Resources::Load<AudioClip>(L"Stoneeye_FarAttack_Bgm", L"..\\Resources\\Sound\\SE_Enemy_StoneEye_Attack_1.wav");


		// 3. Wolf
		Resources::Load<AudioClip>(L"Wolf_Attack1_Bgm", L"..\\Resources\\Sound\\SE_Enemy_Wolf_Attack_0.wav");

		Resources::Load<AudioClip>(L"Wolf_Attack2_Ready_Bgm", L"..\\Resources\\Sound\\SE_Enemy_Wolf_Attack_1_Ready.wav");
		Resources::Load<AudioClip>(L"Wolf_Attack2_Bgm", L"..\\Resources\\Sound\\SE_Enemy_Wolf_Attack_1.wav");

		// 4. 스켈레톤 Lizard
		Resources::Load<AudioClip>(L"Lizard_Attack_Bgm", L"..\\Resources\\Sound\\SE_Enemy_SkeletonLizard_Attack_0.wav");

		// 5. 골렘
		Resources::Load<AudioClip>(L"WoodGolem_Attack1_Ready_Bgm", L"..\\Resources\\Sound\\SE_Enemy_WoodGolem_Attack_0_Ready.wav");
		Resources::Load<AudioClip>(L"WoodGolem_Attack1_Bgm", L"..\\Resources\\Sound\\SE_Enemy_WoodGolem_Attack_0.wav");

		Resources::Load<AudioClip>(L"WoodGolem_Attack2_Ready_Bgm", L"..\\Resources\\Sound\\SE_Enemy_WoodGolem_Attack_1_Ready.wav");
		Resources::Load<AudioClip>(L"WoodGolem_Attack2_Bgm", L"..\\Resources\\Sound\\SE_Enemy_WoodGolem_Attack_1.wav");

		
		// ==== Boss
		// 1. 큰 늑대
		Resources::Load<AudioClip>(L"Boss1_Spawn_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Spawn.wav");

		Resources::Load<AudioClip>(L"Boss1_Stom_Ready_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Stomp_Ready.wav");
		Resources::Load<AudioClip>(L"Boss1_Stom_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Stomp.wav");

		Resources::Load<AudioClip>(L"Boss1_MoveDisappear_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Move_Disappear.wav");
		Resources::Load<AudioClip>(L"Boss1_MoveAppear_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Move_Appear.wav");
									
		Resources::Load<AudioClip>(L"Boss1_HowlingReady_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Howling_Ready.wav");
		Resources::Load<AudioClip>(L"Boss1_Howling_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Howling.wav");

		Resources::Load<AudioClip>(L"Boss1_DashReady_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Dash_Ready.wav");
		Resources::Load<AudioClip>(L"Boss1_Dash_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Dash.wav");

		Resources::Load<AudioClip>(L"Boss1_BreathReady_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Breath_Ready.wav");
		Resources::Load<AudioClip>(L"Boss1_Breath", L"..\\Resources\\Sound\\SE_Boss_Lupus_Breath.wav");

		Resources::Load<AudioClip>(L"Boss1_DieHowl_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Die_Howl.wav");
		Resources::Load<AudioClip>(L"Boss1_Die_Bgm", L"..\\Resources\\Sound\\SE_Boss_Lupus_Die.wav");

		// 2.보스2_석상
		Resources::Load<AudioClip>(L"Statue_StompReady_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Statue_Stomp_Ready.wav");
		Resources::Load<AudioClip>(L"Statue_Stomp_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Statue_Stomp.wav");
		Resources::Load<AudioClip>(L"Statue_EnergyballReady_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Statue_EnergyBall_Ready.wav");
		Resources::Load<AudioClip>(L"Statue_Die_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Statue_Die.wav");

		// 3. 보스2_Goddness
		Resources::Load<AudioClip>(L"Goddness_Spawn_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Spawn.wav");
		Resources::Load<AudioClip>(L"Goddness_SpawnGrab_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Spawn_Grab.wav");

		Resources::Load<AudioClip>(L"Goddness_ShieldBeamReady_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_ShieldBeam_Ready.wav");
		Resources::Load<AudioClip>(L"Goddness_ShiledBeamStart_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_ShieldBeam_Start.wav");
		Resources::Load<AudioClip>(L"Goddness_ShiledBeamLoop_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_ShieldBeam_Loop.wav");

		Resources::Load<AudioClip>(L"Goddness_SummonSpear_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_SummonSpear.wav");

		Resources::Load<AudioClip>(L"Goddness_Energyball_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_EnergyBall.wav");

		Resources::Load<AudioClip>(L"Goddness_Die_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Die.wav");
		Resources::Load<AudioClip>(L"Goddness_DieFall_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Die_Fall.wav");

		Resources::Load<AudioClip>(L"Goddness_Counter_Ready_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Counter_Ready.wav");
		Resources::Load<AudioClip>(L"Goddness_Counter_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Counter.wav");

		// 보스2 오브젝트 사운드 
		Resources::Load<AudioClip>(L"GoddnessObj_BigBall_Spawn_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Object_BigBall_Spawn.wav");
		Resources::Load<AudioClip>(L"GoddnessObj_BigBall_Hit_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Object_BigBall_Hit.wav");

		Resources::Load<AudioClip>(L"GoddnessObj_Ball_Spawn_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Object_Ball_Spawn.wav");
		Resources::Load<AudioClip>(L"GoddnessObj_Ball_SpawnFire_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Object_Ball_Spawn_Fire.wav");
		Resources::Load<AudioClip>(L"GoddnessObj_Ball_SpawnHit_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Object_Ball_Hit.wav");

		Resources::Load<AudioClip>(L"GoddnessObj_Ball_SummonSpear_Hit_Bgm", L"..\\Resources\\Sound\\SE_Boss_Goddess_Object_SummonSpear_Hit.wav");


	}

	

	void Release()
	{
		for ( ConstantBuffer* buff : constantBuffer )
		{
			if (buff == nullptr)
				continue;

			delete buff;
			buff = nullptr;
		}
	}

	void PushDebugMeshAttribute(DebugMesh& mesh)
	{
		debugMeshs.push_back(mesh);
	}

}



