#pragma once
#include "ssComponent.h"
#include "ssMesh.h"
#include "ssMaterial.h"



// �ش� ������Ʈ�� ��ü�� ȭ�鿡 ������� ������ �����Ѵ�.
// �̸� �̿��ؼ� ��ü�� ȭ�鿡 ������� �ʰ�, �浹�� ó���� ���� �ִ�.
// ������ �ʴ� �浹ü�� ���� ������ �����ϴ� ��.....
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