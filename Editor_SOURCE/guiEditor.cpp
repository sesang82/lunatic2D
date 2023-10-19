#include "guiEditor.h"
#include "ssGridScript.h"
#include "..\\Engine_SOURCE\\ssMesh.h"
#include "..\\Engine_SOURCE\\ssResources.h"
#include "..\\Engine_SOURCE\\ssTransform.h"
#include "..\\Engine_SOURCE\\ssMeshRenderer.h"
#include "..\\Engine_SOURCE\\ssMaterial.h"
#include "..\\Engine_SOURCE\\ssRenderer.h"
#include "..\\Engine_SOURCE\\ssConstantBuffer.h"
#include "..\\Engine_SOURCE\\ssInput.h"

namespace gui
{
	using namespace ss::enums;
	std::vector<Widget*> Editor::mWidgets = {};
	std::vector<EditorObject*> Editor::mEditorObjects = {};
	std::vector<DebugObject*> Editor::mDebugObjects = {};

	bool Editor::m_bDebugRender = true;

	void Editor::Initialize()
	{
		mDebugObjects.resize((UINT)eColliderType::End);

		std::shared_ptr<ss::Mesh> mesh
			= ss::Resources::Find<ss::Mesh>(L"DebugRect");
		std::shared_ptr<ss::Material> material
			= ss::Resources::Find<ss::Material>(L"DebugMaterial");


		// �簢��
		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<ss::Transform>();
		ss::MeshRenderer* mr
			= mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<ss::MeshRenderer>();
		mr->SetMaterial(material);
		mr->SetMesh(mesh);


		// ��
		mesh = ss::Resources::Find<ss::Mesh>(L"DebugCircle");
		mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();
		mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<ss::Transform>();
		mr = mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<ss::MeshRenderer>();
		mr->SetMaterial(material);
	
		mr->SetMesh(mesh);



	}
	void Editor::Run()
	{
		Update();
		LateUpdate();
		Render();
	}
	void Editor::Update()
	{

		for (EditorObject* obj : mEditorObjects)
		{
			obj->Update();
		}

		if (ss::Input::GetKeyDown(ss::eKeyCode::P))
		{

			if (m_bDebugRender)
				m_bDebugRender = false;

			else
				m_bDebugRender = true;
		}

	}
	void Editor::LateUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->LateUpdate();
		}
	}
	void Editor::Render()
	{
	
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Render();
		}

		for (const ss::graphics::DebugMesh& mesh 
			: renderer::debugMeshs)
		{
			DebugRender(mesh);
		}

		renderer::debugMeshs.clear();
	}
	void Editor::Release()
	{
		for (auto widget : mWidgets)
		{
			delete widget;
			widget = nullptr;
		}

		for (auto editorObj : mEditorObjects)
		{
			delete editorObj;
			editorObj = nullptr;
		}

		for (auto debugObj : mDebugObjects)
		{
			delete debugObj;
			debugObj = nullptr;
		}
	}

	void Editor::DebugRender(const ss::graphics::DebugMesh& mesh)
	{

		if (!m_bDebugRender)
			return;



		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];

		// ��ġ ũ�� ȸ�� ������ �޾ƿͼ�
		// �ش� ���ӿ�����Ʈ���� �׷��ָ�ȴ�.
		ss::Transform* tr = debugObj->GetComponent<ss::Transform>();

		Vector3 pos = mesh.position;
		pos.z -= 0.01f; // z����ŭ�� ������ ����ؼ� ��ó�� ��

		tr->SetPosition(pos);
		tr->SetRotation(mesh.rotation);

		if (mesh.type == ss::enums::eColliderType::Rect)
		{
			tr->SetScale(mesh.scale);
		}
		else
		{
			float radius = mesh.radius;
			Vector3 scale(radius * 2, radius * 2, 1.0f); 
			tr->SetScale(scale);
		}


		tr->LateUpdate();

		// ī�޶󸶴� ����� �ٸ� ���̴�. �츮�� �ʿ��� �� ����ī�޶��� ����̶�
		// �Ʒ�ó�� ����ī�޶��� ����� ���� �´�. (����� ��ü�� ī�޶� ������ �� ���� ���������ϹǷ�)

		// ���ε� �ϴ� �� �����
		renderer::ColCB colCB = {};
		colCB.c_ColCount = mesh.ColCount;

		ConstantBuffer * cb = renderer::constantBuffer[(UINT)eCBType::Collision];
		cb->SetData(&colCB); // ���� ��� ������ ��� ���ۿ� �־��ش�.
		cb->Bind(eShaderStage::PS); // ��� ���۴� ��� ���̴� �ܰ��̵� ���ε��� �� �ִٴ°� �����̴�. 
		
		ss::Camera* mainCamara = renderer::mainCamera;
		ss::Camera::SetGpuViewMatrix(mainCamara->GetViewMatrix());
		ss::Camera::SetGpuProjectionMatrix(mainCamara->GetProjectionMatrix());


	debugObj->Render();
	}

}
