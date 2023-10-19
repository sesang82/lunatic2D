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


		// 사각형
		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<ss::Transform>();
		ss::MeshRenderer* mr
			= mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<ss::MeshRenderer>();
		mr->SetMaterial(material);
		mr->SetMesh(mesh);


		// 원
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

		// 위치 크기 회전 정보를 받아와서
		// 해당 게임오브젝트위에 그려주면된다.
		ss::Transform* tr = debugObj->GetComponent<ss::Transform>();

		Vector3 pos = mesh.position;
		pos.z -= 0.01f; // z값만큼은 변동을 줘야해서 이처럼 함

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

		// 카메라마다 행렬이 다를 것이다. 우리가 필요한 건 메인카메라의 행렬이라
		// 아래처럼 메인카메라의 행렬을 갖고 온다. (디버그 물체가 카메라가 움직일 때 같이 움직여야하므로)

		// 바인딩 하는 거 만들기
		renderer::ColCB colCB = {};
		colCB.c_ColCount = mesh.ColCount;

		ConstantBuffer * cb = renderer::constantBuffer[(UINT)eCBType::Collision];
		cb->SetData(&colCB); // 월드 행렬 정보를 상수 버퍼에 넣어준다.
		cb->Bind(eShaderStage::PS); // 상수 버퍼는 어느 쉐이더 단계이든 바인딩할 수 있다는게 장점이다. 
		
		ss::Camera* mainCamara = renderer::mainCamera;
		ss::Camera::SetGpuViewMatrix(mainCamara->GetViewMatrix());
		ss::Camera::SetGpuProjectionMatrix(mainCamara->GetProjectionMatrix());


	debugObj->Render();
	}

}
