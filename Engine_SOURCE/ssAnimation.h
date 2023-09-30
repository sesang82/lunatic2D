#pragma once
#include "ssResource.h"
#include "ssTexture.h"

namespace ss
{

    using namespace math;
    using namespace graphics;

    class Animator;

    class Animation :
        public Resource
    {

    public:
        struct tSprite
        {
            Vector2 LT;
            Vector2 size;
            Vector2 offset;
            Vector2 atlasSize; // ���� ũ�⸦ �״�� �����ֵ� ������ǥ��(0~1) uv�� �°� ��ȯ�ؼ� �� ����
            float duration;
            bool reverse;

            tSprite()
                : LT(Vector2::Zero), size(Vector2::Zero)
                , offset(Vector2::Zero), atlasSize(Vector2::Zero), duration(0.0f), reverse(false)
            {

            }
        };


    public:
        Animation();
        ~Animation();

    public:
        void Upadte();
        void LateUpdate();
        void Render();


    private:
        int mIndex;
        bool mbComplete; // �ִϸ��̼��� ��������
        bool mDirection;
        float mTime;
        float mTimeScale;

        UINT mAnimCount;

        Animator* mAnimator;
        std::vector<tSprite> mSprites;
        std::shared_ptr<graphics::Texture> mAtlas;


    public:
        virtual HRESULT Load(const std::wstring& path) { return S_FALSE; }

        void Reset();
        void Binds();

        bool IsComplete() const { return mbComplete; }
        void SetComplete(bool complete) { mbComplete = complete; }
        bool GetDirection() { return mDirection; }
        void SetDirection(bool direction) { mDirection = direction; }


        void SetTimeScale(float scale) { mTimeScale = scale; }



        // Ư�� �ε����� ��� �ð� ���� �ٲٱ� ���� �Լ� (GetIndex �Լ��� �� ���� �� �ȿ��ٰ� �����)
        // �⺻�� 0.1���� ������ ������, ũ�� ���� 
        void SetCurSpriteDuration(float newDuration)
        {
           
             mSprites[mIndex].duration = newDuration;      
        
        }

        UINT GetAnimCount()  { return mAnimCount; }


        int GetIndex() { return mIndex; }

        // columnLength => ���� ��������Ʈ �� ���� �ִ���
        void Create(std::wstring Name
            , std::shared_ptr<graphics::Texture> Atlas
            , Vector2 LT
            , Vector2 Size
            , UINT ColumnLength
            , Vector2 BackSize // ĳ���͸��� backsize�� �ٸ��Ƿ� �׿� �°� �ο����ָ� ��. ��ũ�� 200���� ����
            , Vector2 Offset = Vector2::Zero
            , float Duration = 0.0f
            , bool Reverse = false);

    };

}