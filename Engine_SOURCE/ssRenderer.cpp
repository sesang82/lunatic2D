#include "ssRenderer.h"
#include "ssResources.h"
#include "ssTexture.h"
#include "ssMaterial.h"



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

	// ���� ���� ������ uv��ǥ �������� �־��ش�. 
	void Initialize()
	{
		LoadMesh();
		LoadBuffer();
		LoadShader();
		SetupState();
		LoadMaterial();
	}


	void Render()
	{
		// �������� ī�޶� ���� �̷����Ƿ�, ���� ���� ī�޶���� ��ȸ�ϸ�
		// ī�޶� Ŭ���� ���� �Լ��� ȣ���Ѵ�.
		for (Camera* cam : cameras)
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		// �߰��� ī�޶� �߰��ɼ��ִ� ��쵵 ���� ���̴�.Ư���� ȿ���� ��� ���Դٰ� ������� ��찡 �ֱ� ����
		// �׷��� ī�޶� ���� ��ü�� ������ �� �Ŀ� ī�޶� clear�ؼ� �����ְ�,
		// �� �����Ӹ��� ȣ��Ǵ� ī�޶��� lateupdate���� �ٽ� ī�޶� ������ְ�, �� Render�Լ��� ȣ��Ǹ鼭
		// ��ϵ� ī�޶�� �������� �ϰ�, ����ߴ� ī�޶� �ٽ� �����ְ�.. �̷������� �ݺ��ǰԲ� ���ش�.
		// (�˾Ƽ� �����Ҵ� �ǰԲ�) 
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

		// ====== ���ؽ� ���� �����
		// ���ҽ� ���� shared_ptr�� �����Ǳ� ������ new ������ ��� std::make_shared<>�� ����Ѵ�
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert(L"RectMesh", mesh);

		mesh->CreateVertexBuffer(vertexes.data(), vertexes.size());

		// ====== �ε��� ���� �����
				// 1. �ε��� ���۷� �׷��� ������ ������ ������ �迭�� �־� �����Ѵ�.
		// ��, �ִ� ������ �ݵ�� '�ð� ����'���� �־���Ѵ�.
		// �ݽð� �������� ������ �����ϸ� �޸����� �ν��Ͽ� �׷����� �ʰ� �ȴ�.
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
		// Input layout ���� ���� ������ �Ѱ�����Ѵ�.
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

	

	// ��� ���� ����� 
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
	}

	void LoadShader()
	{

		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		spriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		spriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		ss::Resources::Insert(L"SpriteShader", spriteShader);

		// guard�� ƨ�ܳ��� ��ü���� �̰� ���̴� �����ϱ� 
		std::shared_ptr<Shader> NoCullingSpriteShader = std::make_shared<Shader>();
		NoCullingSpriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		NoCullingSpriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		NoCullingSpriteShader->SetRSState(eRSType::SolidNone); // �޸����� ����� culling ���� �ʵ��� �� 
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
		progressShader->Create(eShaderStage::VS, L"ProgressVS.hlsl", "main");
		progressShader->Create(eShaderStage::PS, L"ProgressPS.hlsl", "main");
		ss::Resources::Insert(L"ProgressHPbarShader", progressShader);


		std::shared_ptr<Shader> progressShader2 = std::make_shared<Shader>();
		progressShader2->Create(eShaderStage::VS, L"spProgressVS.hlsl", "main");
		progressShader2->Create(eShaderStage::PS, L"spProgressPS.hlsl", "main");
		ss::Resources::Insert(L"ProgressSPbarShader", progressShader2);
	



	}



	// +.+.+.+. ����
	// 1. �⺻������ ���� �� ���������� �Ѵ�. (UI ����)
	// 2. ����� �ȵ� �� ���������� �ٲ۴�.
	// 3. ���������� �ٲ� �� �ȳ��´ٸ� �� �� �������Ͻ� �ϳ��� Grid ���̾�� �ִ´�
	// 4. UI�� �׻� �������� �صα�(�Ⱥ��δٸ� �Ⱥ��̴� �༮�� ���������� ������)
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




		std::shared_ptr<Shader> gridShader
			= Resources::Find<Shader>(L"GridShader");

		material = std::make_shared<Material>();
		material->SetShader(gridShader);
		Resources::Insert(L"GridMaterial", material);

		std::shared_ptr<Shader> debugShader
			= Resources::Find<Shader>(L"DebugShader");

		material = std::make_shared<Material>();
		material->SetShader(debugShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"DebugMaterial", material);


		// ============================================================
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"temp", L"..\\Resources\\Texture\\temp.png");

			// === Material ����
			std::shared_ptr<Material> testMateiral = std::make_shared<Material>();
			testMateiral->SetShader(spriteShader);
			testMateiral->SetTexture(texture);
			testMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"tempMtrl", testMateiral);

		}
#pragma region Player [Default]
		//// ==========================
		//// +.+. �糪ƽ�� +.+.
		//// ====== ĳ���͵�
		//// 1. �÷��̾�
		// [Default]
		
		// << Spawn
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Spawn", L"..\\Resources\\Texture\\Player\\Spawn\\merge_from_ofoct (4).png");

			// === Material ����
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_SpawnMtrl", spriteMateiral);

		}


		// << Idle
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Idle", L"..\\Resources\\Texture\\Player\\Default\\Idle\\merge_from_ofoct.png");

			// === Material ����
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);


			Resources::Insert(L"D_IdleMtrl", spriteMateiral);

		}

		// << Run
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Run", L"..\\Resources\\Texture\\Player\\Default\\Run\\merge_from_ofoct (3).png");

			// === Material ����
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_RunMtrl", spriteMateiral);

		}


		// << Jump (����ģ��)
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Jump22222", L"..\\Resources\\Texture\\Player\\Default\\Jump\\merge_from_ofoct (1).png");

			// === Material ����
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_JumpMtrl", spriteMateiral);

		}

		// ��ģ�� 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Jump", L"..\\Resources\\Texture\\Player\\Default\\Jump\\jump.png");

			// === Material ����
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_JumpMtrl", spriteMateiral);

		}


		// << Landing
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Landing", L"..\\Resources\\Texture\\Player\\Default\\Landing\\merge_from_ofoct (2).png");

			// === Material ����
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);

			Resources::Insert(L"D_LandingMtrl", spriteMateiral);

		}

		// Fall
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"D_Fall", L"..\\Resources\\Texture\\Player\\Default\\Fall\\Fall.png");

			// === Material ����
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

			// === Material ����
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

			// === Material ����
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(AnimShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"S_PlayerRunMtrl", spriteMateiral);

		}
		// >> Jump (fall ����ģ��)
		 {
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"S_PlayerJump", L"..\\Resources\\Texture\\Player\\Sword\\Jump\\Jump.png");

			// === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

		 // === Material ����
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

			 // === Material ����
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);
			 spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			 Resources::Insert(L"S_PlayerspAttackMtrl", spriteMateiral);

		 }



#pragma endregion 

#pragma region Player [Bow]
		// >> Idle
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"B_PlayerIdle", L"..\\Resources\\Texture\\Player\\Bow\\Idle\\Idle.png");

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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
		 // ���׷� �Ÿ��°� �̻� ����
		{
		 std::shared_ptr<Texture> texture
			 = Resources::Load<Texture>(L"B_PlayerDie", L"..\\Resources\\Texture\\Player\\Bow\\Die\\Die.png");

		 // === Material ����
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

			 // === Material ����
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
 


#pragma endregion 



#pragma region Monster [StoneEye]

		// >> Idle
		 {
			 std::shared_ptr<Texture> texture
				 = Resources::Load<Texture>(L"StoneEyeIdle", L"..\\Resources\\Texture\\Monster\\StoneEye\\Idle\\Idle.png");

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

		 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
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

			 // === Material ����
			 std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			 spriteMateiral->SetShader(AnimShader);
			 spriteMateiral->SetTexture(texture);

			 Resources::Insert(L"Zombie_DieMtrl", spriteMateiral);
		 }




#pragma endregion 



#pragma region Scene [Title]
		//// ===== Ÿ��Ʋ��
		//// BG
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Title01", L"..\\Resources\\Texture\\Title\\Main_BG.png");

			// === Material ����
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"TitleBG", spriteMateiral);

		}

#pragma endregion 




#pragma region Scene [Library]
		//// ==== ������ ��
		//// ���ϴ� ���
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Moon1", L"..\\Resources\\Texture\\Library\\BG_0_0.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
		

			Resources::Insert(L"MoonMtrl", spriteMateiral);
		}

		// ������
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Library", L"..\\Resources\\Texture\\Library\\LibraryMap.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);
			

			Resources::Insert(L"LibraryMtrl", spriteMateiral);

		}


		// ����
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"WeaponTree", L"..\\Resources\\Texture\\Library\\Tree.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);
		

			Resources::Insert(L"WeaponTreeMtrl", spriteMateiral);
		}

		// ���� Į
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MiniWeaponSword", L"..\\Resources\\Texture\\Library\\w1.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"w1Mtrl", spriteMateiral);
		}


		// ���� ��
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MiniWeaponPistol", L"..\\Resources\\Texture\\Library\\w2.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"w2Mtrl", spriteMateiral);
		}


		// ���� Ȱ
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MiniWeaponBorrow", L"..\\Resources\\Texture\\Library\\w3.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"w3Mtrl", spriteMateiral);
		}
#pragma endregion 




#pragma region Scene [Entrance]
		// == �������� �Ա� ���
		// ���
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Entrance", L"..\\Resources\\Texture\\Library\\EntrenceMap.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"EntranceMtrl", spriteMateiral);
		}

		//// ��Ż
		//{
		//	std::shared_ptr<Texture> texture
		//		= Resources::Load<Texture>(L"Potal", L"..\\Resources\\Texture\\Library\\potal.png");

		//	// == Material ����(������ ���� shader�� texture�� �̿�)
		//	std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
		//	spriteMateiral->SetShader(spriteShader);
		//	spriteMateiral->SetTexture(texture);
		//	spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

		//	Resources::Insert(L"PotalMtrl", spriteMateiral);
		//}
#pragma endregion 




#pragma region Scene [Entry]
		// ==���� ���� �� ��
		// ��(Ÿ��+�� ���)
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Entry", L"..\\Resources\\Texture\\Stage1\\entrymap.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			//spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"EntryMtrl", spriteMateiral);
		}

		// == ��Ǯ��1
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Tree1", L"..\\Resources\\Texture\\Stage1\\BG_1_1.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);
	

			Resources::Insert(L"Tree1Mtrl", spriteMateiral);
		}


		//// == ��Ǯ��2
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Tree2", L"..\\Resources\\Texture\\Stage1\\BG_1_2.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Tree2Mtrl", spriteMateiral);
		}

		//// == ��Ǯ��3
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Tree3", L"..\\Resources\\Texture\\Stage1\\BG_1_3.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Tree3Mtrl", spriteMateiral);
		}

		//// == ��Ǯ��4
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Tree4", L"..\\Resources\\Texture\\Stage1\\BG_1_4.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Tree4Mtrl", spriteMateiral);
		}

		//// == ���� ����
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Moon1Statue", L"..\\Resources\\Texture\\Stage1\\212_124 Left.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Moon1StatueMtrl", spriteMateiral);
		}

		//// == ��� ����
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Moon2Statue", L"..\\Resources\\Texture\\Stage1\\BG_1_Object_39.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Moon2StatueMtrl", spriteMateiral);
		}


		//// == ������ ����
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Moon3Statue", L"..\\Resources\\Texture\\Stage1\\212_124 Right.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Moon3StatueMtrl", spriteMateiral);
		}
#pragma endregion 



#pragma region Scene [Stage1]
		// ==== stage 1
		// �ձ� ��
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Moon2", L"..\\Resources\\Texture\\Stage1\\BG_1_0.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
		

			Resources::Insert(L"Moon2Mtrl", spriteMateiral);
		}

		// ��
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Stage1", L"..\\Resources\\Texture\\Stage1\\Stage1.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Stage1Mtrl", spriteMateiral);
		}
#pragma endregion 




#pragma region scene [Boss 1 BackGround]
		// ===== ��������
		// ������
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Boss1", L"..\\Resources\\Texture\\Boss\\WolfMap.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			//spriteMateiral->SetRenderingMode(eRenderingMode::CutOut);

			Resources::Insert(L"Boss1Mtrl", spriteMateiral);
		}
#pragma endregion 



#pragma region Game UI
		// ==== UI
		// �÷��̾� Frame
		{

			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerFrame", L"..\\Resources\\Texture\\UI\\Player\\UI_MainPlayer_Frame.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"PlayerFrameMtrl", spriteMateiral);

		}

		// ���� Frame
		{

			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MonsterFrame", L"..\\Resources\\Texture\\UI\\Monster\\UI_EnemyHp_Frame.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"MonsterFrameMtrl", spriteMateiral);

		}



		//(���� HP Ʋ)
		{

			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BossHpFrame", L"..\\Resources\\Texture\\UI\\Boss\\UI_BossHp_Frame.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"BossHpFrameMtrl", spriteMateiral);

		}

		// -- HP��
		// 
		// 
		
		// ���� hp�� 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BossHPBar", L"..\\Resources\\Texture\\UI\\Boss\\UI_BossHp_Bar.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(progressShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"BossHPBarMtrl", spriteMateiral);
		}

		// ���� hp�� 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MonsterHPBar", L"..\\Resources\\Texture\\UI\\Monster\\UI_EnemyHp_Bar.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"MonsterHPBarMtrl", spriteMateiral);
		}

		// �÷��̾� hp�� 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerBar", L"..\\Resources\\Texture\\UI\\Player\\HP\\UI_PlayerUI_Bar_Hp.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(progressShader);
			spriteMateiral->SetTexture(texture); // 0�� ���� ��� 
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"PlayerBarMtrl", spriteMateiral);
		}

		// �÷��̾� sp�� 
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerSPBar", L"..\\Resources\\Texture\\UI\\Player\\SP\\UI_PlayerUI_Bar_SpAttack.png");

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(progressShader2);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::Transparent);

			Resources::Insert(L"PlayerSPBarMtrl", spriteMateiral);
		}
#pragma endregion 


#pragma region Effect
		{
			//std::shared_ptr<Texture> texture
			//	= Resources::Load<Texture>(L"StunEffect", L"..\\Resources\\Texture\\Monster\\Effect\\Stun\\EnemyStun.png");

			//// == Material ����(������ ���� shader�� texture�� �̿�)
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

			// == Material ����(������ ���� shader�� texture�� �̿�)
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			spriteMateiral->SetRenderingMode(eRenderingMode::CutOut); // �տ� ����ߵǴ°� �̰ɷ� 

			Resources::Insert(L"hitGroundMtrl", spriteMateiral);
		}




#pragma endregion 


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



