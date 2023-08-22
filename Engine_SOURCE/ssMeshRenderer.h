#pragma once
#include "ssComponent.h"
#include "ssMesh.h"
#include "ssMaterial.h"



// 해당 컴포넌트는 물체를 화면에 출력할지 말지를 결정한다.
// 이를 이용해서 물체를 화면에 출력하지 않고, 충돌만 처리할 수도 있다.
// 보이지 않는 충돌체로 게임 로직을 구현하는 등.....
namespace ss
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void SetMesh(std::shared_ptr<Mesh> mesh) { mMesh = mesh; }
		void SetMaterial(std::shared_ptr<Material> material) { mMaterial = material; }
		std::shared_ptr<Material> GetMaterial() { return mMaterial; }

	private:
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Material> mMaterial;

	public:
		std::shared_ptr<Material> GetMateiral() { return mMaterial; }
	};
}