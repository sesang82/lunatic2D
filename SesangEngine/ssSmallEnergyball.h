#pragma once
#include "ssBullet.h"

namespace ss
{

    class SmallEnergyball :
        public Bullet
    {

        class Engeryball;

    public:
        SmallEnergyball();
        virtual ~SmallEnergyball();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    private:
        GameObject* mOriginOwner;
        SmallEnergyball* mFirstEnergyball;
        float mfTime;
        Vector3 mPlayerPos;
        Vector3 mDir;


        int miMaxSpawnedBalls;
        bool mbSpawnComplete;
        bool mStorePlayerPos;

        std::vector<Vector2> mEnergyball_GroupMainPos[4];

        class Animator* mAnimator;
        class Transform* mTransform;
        class GameObject* mOwner;  // ��� ��ü�� ����ϵ��� �ص� 
        class Player* mPlayer;


        static bool  mbSpawn;



    public:
        void SetOriginOwenr(GameObject* obj) { mOwner = obj; }
        void SetFirstEnergyball(SmallEnergyball* obj) { mFirstEnergyball = obj; }
    
        void SetOriginOwner(GameObject* obj) { mOriginOwner = obj; }


        void CreateCircleBalls();

        // ������ �������� ��ȯ (���߰� �˾Ƶ��)
        inline float ToRadian(float degree)
        {
            const float PI = 3.14159265358979323846f;

            return degree * (PI / 180.0f);
        }

        void SetDir(Vector3 dir)
        {
            mDir = dir;
        }

        Vector3 GetDir()
        {
            return mDir;
        }



    };
}