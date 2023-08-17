#pragma once
#include "ssGameObject.h"

namespace ss
{
    class Platform :
        public GameObject
    {

     public:
		
		 Platform();
		virtual ~Platform();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
		Collider2D* mCollider;
    };
}

