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
            Vector2 atlasSize; // 원본 크기를 그대로 보내주되 비율좌표계(0~1) uv에 맞게 변환해서 쓸 것임
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
        bool mbComplete; // 애니메이션이 끝났는지
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



        // 특정 인덱스의 재생 시간 값을 바꾸기 위한 함수 (GetIndex 함수를 쓴 다음 그 안에다가 써야함)
        // 기본값 0.1보다 작으면 빠르고, 크면 느림 
        void SetCurSpriteDuration(float newDuration)
        {
           
             mSprites[mIndex].duration = newDuration;      
        
        }

        UINT GetAnimCount()  { return mAnimCount; }


        int GetIndex() { return mIndex; }

        // columnLength => 열로 스프라이트 몇 개가 있는지
        void Create(std::wstring Name
            , std::shared_ptr<graphics::Texture> Atlas
            , Vector2 LT
            , Vector2 Size
            , UINT ColumnLength
            , Vector2 BackSize // 캐릭터마다 backsize값 다르므로 그에 맞게 부여해주면 됨. 링크는 200으로 해쥼
            , Vector2 Offset = Vector2::Zero
            , float Duration = 0.0f
            , bool Reverse = false);

    };

}