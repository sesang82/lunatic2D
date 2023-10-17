#pragma once
#include "ssComponent.h"
#include "ssAnimation.h"

namespace ss
{
    class Animator :
        public Component
    {
    public:
        struct Event
        {
            // =을 함수 포인터로 쓰게 함
            void operator=(std::function<void()> func)
            {
                mEvent = std::move(func);
            }

            void operator()()
            {
                if (mEvent)
                    mEvent();
            }

            std::function<void()> mEvent;
        };

        // 
        struct Events
        {
            Event startEvent;	 // 애니메이션 시작할 때 호출
            Event completeEvent;  // 애니메이션 한 사이클 끝날 때 마다
            Event endEvent;      // 애니메이션 교체나 interrupt 걸릴때 호출
            std::vector<Event> mFrameEvents; // 해당 스프라이트(인덱스) 에서 실행될 이벤트  
            std::vector<bool> mEventCalledFlags; // 해당 프레임의 이벤트가 호출되었는지의 플래그
        };

    public:
        Animator();
        ~Animator();

    public:
        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

    private:
        bool mbLoop;
        bool mInRange;
        bool mAgainAttack; // 같은 애니메이션인 어택을 다시 할 때 return 되는거 막는 함수 
        bool mHit; 

        UINT mStartIdx;
        UINT mEndIdx;

        UINT mAnimCount;

        UINT mEventIndex;
        UINT mPrevEventIndex;

        std::function<void()> mEndEvent;

        std::map<std::wstring, Animation*> mAnimations;
        std::map<std::wstring, Events*> mEvents;

        Animation* mActiveAnimation;
        Animation* mPrevAnimation;


        class PlayerScript* mPlayerScript;
        class MonsterScript* mMonsterScript;


    public:
        // backsize값은 이미지마다 다르므로 적절한 값 찾기. (쌤 코드는 그냥 200/200으로 고정해서 나눠주고 있었음) 
        // offset 값은 내가 자른 이미지를 아래로 내릴지 위로 내릴지 등을 결정하는 값이다. 
        void Create(const std::wstring& name
            , std::shared_ptr<graphics::Texture> atlas
            , Vector2 leftTop
            , Vector2 size
            , UINT columnLength
            , Vector2 backsize
            , Vector2 offset = Vector2::Zero
            , float duration = 0.1f
            , bool reverse = false);

        Animation* FindAnimation(const std::wstring& name);
        Events* FindEvents(const std::wstring& name);
        const std::wstring& GetCurActiveAnim() { return mActiveAnimation->GetKey(); }
        Animation* GetCurActiveAnimation() { return mActiveAnimation; }
        void PlayAnimation(const std::wstring& name, bool loop);
        void Binds(); // 애니메이션을 바인딩

         void SetIndex(int _index) { mEventIndex = _index;}

         void SetAgainAttack(bool _again) { mAgainAttack = _again; }
         bool IsAgainAttack() { return mAgainAttack; }

         void SetHitCheck(bool _hit) { mHit = _hit; }
         bool IsHit() { return mHit; }


         void SetLoop(bool _loop) { mbLoop = _loop; };
         bool IsLoop() { return mbLoop; }

        //== 애니메이션 관련 함수 포인터

        // 애니메이션이 시작될 때 호출되는 함수
        std::function<void()>& StartEvent(const std::wstring key);

        // 애니메이션 재생이 완료됐을 때 호출될 함수
        std::function<void()>& CompleteEvent(const std::wstring key);

        // 애니메이션이 중간에 멈출 때나 끝났을 때 호출될 함수
        std::function<void()>& EndEvent(const std::wstring key);

        // 특정 애니메이션 1개 프레임에 행할 이벤트를 등록한다. 
      //  std::function<void()>& FrameEvent(const std::wstring& _name, UINT _idx);

        std::function<void()>& RegisterFrameEvent(const std::wstring& animationName, int frameIndex);



    };
}