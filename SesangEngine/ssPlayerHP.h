#pragma once
#include "ssProgressbar.h"

namespace ss
{
	class PlayerHP :
		public Progressbar
	{
	public:
		PlayerHP();
		virtual ~PlayerHP();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void BindConstantBuffer();


	};
}


