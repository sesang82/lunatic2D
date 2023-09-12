#pragma once
#include "ssCharacter.h"

namespace ss
{

    class Monster :
        public Character
    {
  


    public:
        Monster();
        virtual ~Monster();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    private:

   

    };
}
