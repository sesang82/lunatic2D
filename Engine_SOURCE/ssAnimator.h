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
            // =�� �Լ� �����ͷ� ���� ��
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
            Event startEvent;	 // �ִϸ��̼� ������ �� ȣ��
            Event completeEvent;  // �ִϸ��̼� �� ����Ŭ ���� �� ����
            Event endEvent;      // �ִϸ��̼� ��ü�� interrupt �ɸ��� ȣ��
            std::vector<Event> mFrameEvents; // �ش� ��������Ʈ(�ε���) ���� ����� �̺�Ʈ  
            std::vector<bool> mEventCalledFlags; // �ش� �������� �̺�Ʈ�� ȣ��Ǿ������� �÷���
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
        bool mAgainAttack; // ���� �ִϸ��̼��� ������ �ٽ� �� �� return �Ǵ°� ���� �Լ� 
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
        // backsize���� �̹������� �ٸ��Ƿ� ������ �� ã��. (�� �ڵ�� �׳� 200/200���� �����ؼ� �����ְ� �־���) 
        // offset ���� ���� �ڸ� �̹����� �Ʒ��� ������ ���� ������ ���� �����ϴ� ���̴�. 
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
        void Binds(); // �ִϸ��̼��� ���ε�

         void SetIndex(int _index) { mEventIndex = _index;}

         void SetAgainAttack(bool _again) { mAgainAttack = _again; }
         bool IsAgainAttack() { return mAgainAttack; }

         void SetHitCheck(bool _hit) { mHit = _hit; }
         bool IsHit() { return mHit; }


         void SetLoop(bool _loop) { mbLoop = _loop; };
         bool IsLoop() { return mbLoop; }

        //== �ִϸ��̼� ���� �Լ� ������

        // �ִϸ��̼��� ���۵� �� ȣ��Ǵ� �Լ�
        std::function<void()>& StartEvent(const std::wstring key);

        // �ִϸ��̼� ����� �Ϸ���� �� ȣ��� �Լ�
        std::function<void()>& CompleteEvent(const std::wstring key);

        // �ִϸ��̼��� �߰��� ���� ���� ������ �� ȣ��� �Լ�
        std::function<void()>& EndEvent(const std::wstring key);

        // Ư�� �ִϸ��̼� 1�� �����ӿ� ���� �̺�Ʈ�� ����Ѵ�. 
      //  std::function<void()>& FrameEvent(const std::wstring& _name, UINT _idx);

        std::function<void()>& RegisterFrameEvent(const std::wstring& animationName, int frameIndex);



    };
}