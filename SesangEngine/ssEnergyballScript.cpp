#include "ssEnergyballScript.h"
#include "ssEnergyball.h"

namespace ss
{
	EnergyballScript::EnergyballScript()
		: mvStartPosition(Vector3::Zero)
	{

		const float x1 = 100.0f; // 예시 값
		const float x2 = 150.0f; // 예시 값
		const float x3 = 200.0f; // 예시 값
		const float x4 = 250.0f; // 예시 값
		const float y = 50.0f;   // 예시 값
		const float z = 0.0f;    // 예시 값


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
		const float delayBetweenBalls = 0.5f;  // 에너지볼 사이의 딜레이 (초단위).

		// TODO: 여기에 delayBetweenBalls를 사용하는 로직을 추가하세요.
		// 예를 들면, 게임의 시간을 추적하거나 다음 에너지볼 생성까지의 대기 시간을 관리할 수 있습니다.

		Vector3 StartPos = Vector3::Zero;

		for (int group = 0; group < 4; ++group) {  // 4개의 그룹
			for (int i = 0; i < 3; ++i) {  // 그룹당 3개의 에너지볼
				Vector3 spawnPosition = CalculateSpawnPosition(StartPos, group, i);  // 에너지볼이 생성될 위치를 계산합니다.
				CreateBall(spawnPosition);  // 에너지볼을 생성합니다.
				// Sleep 함수 대신 다른 방법을 사용하여 딜레이를 구현하세요.
			}
		}
	}

	Energyball* EnergyballScript::CreateBall(Vector3 position)
	{
		Energyball* energyBall = new Energyball();  // 에너지볼 게임 오브젝트 생성
		energyBall->GetComponent<Transform>()->SetPosition(position);  // 에너지볼의 위치를 설정합니다.
		// 필요한 경우, 추가적인 설정을 여기에 추가하십시오 (예: 크기, 색상, 텍스처 등).
		return energyBall;
	}

	Vector3  EnergyballScript::CalculateSpawnPosition(Vector3 startPosition, int group, int index)
	{
		// 여기에서 각 그룹과 인덱스에 따른 에너지볼의 생성 위치를 계산합니다.
  // 이 함수의 구현은 당신의 게임의 요구 사항에 따라 달라질 수 있습니다.
  // 예제로 간단한 구현을 제공하겠습니다.
		Vector3 offset(10.0f * index, -10.0f * group, 0);
		return startPosition + offset;
	}


}


