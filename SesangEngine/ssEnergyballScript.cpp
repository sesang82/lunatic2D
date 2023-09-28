#include "ssEnergyballScript.h"
#include "ssEnergyball.h"

namespace ss
{
	EnergyballScript::EnergyballScript()
		: mvStartPosition(Vector3::Zero)
	{

		const float x1 = 100.0f; // ���� ��
		const float x2 = 150.0f; // ���� ��
		const float x3 = 200.0f; // ���� ��
		const float x4 = 250.0f; // ���� ��
		const float y = 50.0f;   // ���� ��
		const float z = 0.0f;    // ���� ��


		std::vector<EnergyBallPattern> firstGroupPattern = 
		{
		{ Vector3(x1, y, z), 0.0f },
		{ Vector3(x2, y, z), 0.5f },
		{ Vector3(x3, y, z), 1.0f }
		};

		std::vector<EnergyBallPattern> secondGroupPattern = 
		{
			{ Vector3(x1, y, z), 0.0f },
			{ Vector3(x4, y, z), 0.5f },
			{ Vector3(x3, y, z), 1.0f }
		};


		std::vector<EnergyBallPattern> ThirdGroupPattern =
		{
			{ Vector3(x1, y, z), 0.0f },
			{ Vector3(x3, y, z), 1.f },
			{ Vector3(x2, y, z), 0.5f }
		};


		std::vector<EnergyBallPattern> FourGroupPattern =
		{
			{ Vector3(x1, y, z), 1.0f },
			{ Vector3(x2, y, z), 0.5f },
			{ Vector3(x3, y, z), 0.0f }
		};





	}

	EnergyballScript::~EnergyballScript()
	{
	}

	void EnergyballScript::Initialize()
	{
	}

	void EnergyballScript::Update()
	{
	}

	void EnergyballScript::OnCollisionEnter(Collider2D* other)
	{
	}

	void EnergyballScript::OnCollisionStay(Collider2D* other)
	{
	}

	void EnergyballScript::OnCollisionExit(Collider2D* other)
	{
	}
	void EnergyballScript::SpawnEnergyBalls()
	{
		const float delayBetweenBalls = 0.5f;  // �������� ������ ������ (�ʴ���).

		// TODO: ���⿡ delayBetweenBalls�� ����ϴ� ������ �߰��ϼ���.
		// ���� ���, ������ �ð��� �����ϰų� ���� �������� ���������� ��� �ð��� ������ �� �ֽ��ϴ�.

		Vector3 StartPos = Vector3::Zero;

		for (int group = 0; group < 4; ++group) {  // 4���� �׷�
			for (int i = 0; i < 3; ++i) {  // �׷�� 3���� ��������
				Vector3 spawnPosition = CalculateSpawnPosition(StartPos, group, i);  // ���������� ������ ��ġ�� ����մϴ�.
				CreateBall(spawnPosition);  // ���������� �����մϴ�.
				// Sleep �Լ� ��� �ٸ� ����� ����Ͽ� �����̸� �����ϼ���.
			}
		}
	}

	Energyball* EnergyballScript::CreateBall(Vector3 position)
	{
		Energyball* energyBall = new Energyball();  // �������� ���� ������Ʈ ����
		energyBall->GetComponent<Transform>()->SetPosition(position);  // ���������� ��ġ�� �����մϴ�.
		// �ʿ��� ���, �߰����� ������ ���⿡ �߰��Ͻʽÿ� (��: ũ��, ����, �ؽ�ó ��).
		return energyBall;
	}

	Vector3  EnergyballScript::CalculateSpawnPosition(Vector3 startPosition, int group, int index)
	{
		// ���⿡�� �� �׷�� �ε����� ���� ���������� ���� ��ġ�� ����մϴ�.
  // �� �Լ��� ������ ����� ������ �䱸 ���׿� ���� �޶��� �� �ֽ��ϴ�.
  // ������ ������ ������ �����ϰڽ��ϴ�.
		Vector3 offset(10.0f * index, -10.0f * group, 0);
		return startPosition + offset;
	}


}


