#pragma once
#include <ssScript.h>

namespace ss
{
    // 각 캐릭터들은 해당 스크립트를 멤버로 갖고 있으면 됨 
	class CharacterState :
		public Script
	{
        struct tBarType
        {
            float mCurrentHP;
            float mMaxHP;
            float mCurrentSP;
            float mMaxSP;

            int   mDamage;
            int   mHeal;
            UINT   Type; // 0 = 감소하는 형태, 1 = 증가하는 형태
            eTextureType mTexType; // eType 일반 몬스터들은 쓰는 hp바 이미지는 똑같은데 정보가 달라서 이걸로 구분해줌 
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
        void BindConstantBuffer(); // 일반 몬스터 용
        // GPU에 생성한 상수 버퍼에 데이터 넣어주는 용도 (game state처럼 씬 넘어가도 정보 유지되어야하는 녀석들은 이걸 써야함) 
       void BindConstantBuffer(const std::wstring& _name);  


    protected:
        tBarType mBarType;

    public:
        // 0이면 감소, 1이면 증가
        virtual void SetBarType(UINT _type)
        {

            mBarType.Type = _type;
        }

        virtual void SetCurrentHP(float _currentHP)
        {

            mBarType.mCurrentHP = _currentHP;
        }

        virtual void SetMaxHP(float _maxHP)
        {
            mBarType.mMaxHP = _maxHP;
        }

        virtual void SetCurrentSP(float _currentSP)
        {

            mBarType.mCurrentSP = _currentSP;
        }

        virtual void SetMaxSP(float _maxSP)
        {
            mBarType.mMaxSP = _maxSP;
        }

        virtual void SetDamage(float _damage, bool _IsDash = false)
        {
            // 데미지 무력화 
            if (mBarType.mCurrentHP <= 0 || _IsDash == true)
                return;

            mBarType.mCurrentHP -= _damage;
        }

        virtual void SetHeal(float _heal)
        {
            if (mBarType.mCurrentHP >= mBarType.mMaxHP)
                return;

            mBarType.mCurrentHP += _heal;

        }

        // 쉐이더에 switch case로 구분 지어 주기 
        // enum 파일 참고 eType 
        virtual void SetTexType(eTextureType _type)
        {
			mBarType.mTexType = _type;
		}



    public:
        float GetCurrentHP() { return mBarType.mCurrentHP; }
        float GetMaxHP() { return mBarType.mMaxHP; }

        float GetCurrentSP() { return mBarType.mCurrentSP; }
        float GetMaxSP() { return mBarType.mMaxSP; }

        float GetDamage() { return mBarType.mDamage; }
        float GetHeal() { return mBarType.mHeal; }

        int   GetBarType() { return mBarType.Type; }


        // enum 파일 참고 eTextureType 
        eTextureType GetTexType() { return mBarType.mTexType; }


	};
}


