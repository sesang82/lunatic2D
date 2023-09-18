#pragma once
#include <ssScript.h>

namespace ss
{
    // 각 캐릭터들은 해당 스크립트를 멤버로 갖고 있으면 됨 
	class CharacterState :
		public Script
	{
        enum class eBarState
        {
            Damaged,
            Heal,
            // 다른 상태들 추가 가능
        };

        struct tState
        {
            float mCurrentHP = 0;
            float mMaxHP = 0;
            float mCurrentSP = 0;
            float mMaxSP = 0;
            float mCurOverload = 0;
            float mMaxOverload = 0;
            eBarState mBarState; // 0 = 감소하는 형태, 1 = 증가하는 형태
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
        // GPU에 생성한 상수 버퍼에 데이터 넣어주는 용도 (game state처럼 씬 넘어가도 정보 유지되어야하는 녀석들은 이걸 써야함) 
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

        // 0이면 감소, 1이면 증가
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
            // 데미지 무력화 
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

        
        // 쉐이더에 switch case로 구분 지어 주기 
        // enum 파일 참고 eType 
        // 이걸 한다고 해당 이미지에 해당하는게 바뀌는게 아니었음...
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


