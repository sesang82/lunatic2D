#pragma once
#include "ssProgressbar.h"

namespace ss
{
	class PlayerSP :
		public Progressbar
	{
	public:
		PlayerSP();
		virtual ~PlayerSP();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;


		void BindConstantBuffer();

	};
}


