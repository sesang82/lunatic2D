#pragma once
#include "ssCharacter.h"

namespace ss
{
    class MeshRenderer;

    class Player :
        public Character
    {

        
     public:
         Player();
        virtual ~Player();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    private:
        Vector2 mSize;

        class GameObject* mAttackCol;
        Collider2D* mCol;
        Transform* mTransform;

        bool mEnableRender;

    public:
        void SetEnableRender(bool _enable) { mEnableRender = _enable; }
	    bool GetEnableRender() { return mEnableRender; }


    };
}