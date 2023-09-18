#pragma once
#include <ssScript.h>

namespace ss
{
    // �� ĳ���͵��� �ش� ��ũ��Ʈ�� ����� ���� ������ �� 
	class CharacterState :
		public Script
	{
        enum class eBarState
        {
            Damaged,
            Heal,
            // �ٸ� ���µ� �߰� ����
        };

        struct tState
        {
            float mCurrentHP = 0;
            float mMaxHP = 0;
            float mCurrentSP = 0;
            float mMaxSP = 0;
            float mCurOverload = 0;
            float mMaxOverload = 0;
            eBarState mBarState; // 0 = �����ϴ� ����, 1 = �����ϴ� ����
            std::wstring Name;
        };

 

    public:
        CharacterState();
        ~CharacterState();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

        virtual void OnTriggerEnter(Collider2D* collider) {};
        virtual void OnTriggerStay(Collider2D* collider) {};
        virtual void OnTriggerExit(Collider2D* collider) {};


    public: 
        // GPU�� ������ ��� ���ۿ� ������ �־��ִ� �뵵 (game stateó�� �� �Ѿ�� ���� �����Ǿ���ϴ� �༮���� �̰� �����) 
       void BindConstantBuffer(const std::wstring& _name);  


    protected:
        tState mStateType;
        eBarState mBarState;

        std::vector<tState> mStateList;




    public:
        void AddState(float _MaxHP, float _curHP, float _MaxSP, float _curSP, std::wstring _Name, eBarState _Type)
        {
            tState state;
           state.mCurrentHP = _curHP;
           state.mMaxHP = _MaxHP;
           state.mCurrentSP =  _curSP;
           state.mMaxSP = _MaxSP;
           state.mBarState = _Type;
           state.Name = _Name;

           mStateList.push_back(state);
        }

        // 0�̸� ����, 1�̸� ����
        virtual void SetBarType(eBarState _type)
        {

            mStateType.mBarState = _type;
        
        }

        virtual void SetCurrentHP(float _currentHP)
        {

            mStateType.mCurrentHP = _currentHP;

        }

        virtual void SetMaxHP(float _maxHP)
        {
            mStateType.mMaxHP = _maxHP;

        }

        virtual void SetCurrentSP(float _currentSP)
        {
            mStateType.mCurrentSP = _currentSP;

        }

        virtual void SetMaxSP(float _maxSP)
        {
            mStateType.mMaxSP = _maxSP;

        }

        virtual void SetDamage(float _damage, bool _IsDash = false)
        {
            // ������ ����ȭ 
            if (mStateType.mCurrentHP <= 0 || _IsDash == true)
                return;

            mStateType.mCurrentHP -= _damage;

        }

        virtual void SetHeal(float _heal)
        {
            if (mStateType.mCurrentHP >= mStateType.mMaxHP)
                return;

            mStateType.mCurrentHP += _heal;

        }

        void SetCurOverload(float _over)
        {

            mStateType.mCurOverload = _over;
        }


        void SetMaxOverload(float _over)
        {
            mStateType.mMaxOverload = _over;
        }

        
        // ���̴��� switch case�� ���� ���� �ֱ� 
        // enum ���� ���� eType 
        // �̰� �Ѵٰ� �ش� �̹����� �ش��ϴ°� �ٲ�°� �ƴϾ���...
  //      virtual void SetTexType(eTextureType _type)
  //      {
  //          mStateType.mTexType = _type;
		//}



    public:
        float GetCurrentHP() { return mStateType.mCurrentHP; }
        float GetMaxHP() { return mStateType.mMaxHP; }

        float GetCurrentSP() { return mStateType.mCurrentSP; }
        float GetMaxSP() { return mStateType.mMaxSP; }

        float GetCurrentOverload() { return mStateType.mCurOverload; }
        float GetMaxOverload() { return mStateType.mMaxOverload; }

        eBarState   GetBarType() { return mStateType.mBarState; }



	};
}


