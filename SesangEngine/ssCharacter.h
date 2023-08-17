#pragma once
#include "ssGameObject.h"

namespace ss
{


    class Character :
        public GameObject
    {
    public:
        enum class eType
        {
            NONE,
            PLAYER,
            MONSTER,
            BOSS,
            END,
        };

    public:
        Character(eType _type);
        virtual ~Character();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    protected:
        eType mCharacterType;




    };
}
