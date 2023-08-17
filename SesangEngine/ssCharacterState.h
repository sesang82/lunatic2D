#pragma once
#include <ssScript.h>

namespace ss
{
    // �� ĳ���͵��� �ش� ��ũ��Ʈ�� ����� ���� ������ �� 
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
            UINT   Type; // 0 = �����ϴ� ����, 1 = �����ϴ� ����
            eTextureType mTexType; // eType �Ϲ� ���͵��� ���� hp�� �̹����� �Ȱ����� ������ �޶� �̰ɷ� �������� 
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
        void BindConstantBuffer(); // �Ϲ� ���� ��
        // GPU�� ������ ��� ���ۿ� ������ �־��ִ� �뵵 (game stateó�� �� �Ѿ�� ���� �����Ǿ���ϴ� �༮���� �̰� �����) 
       void BindConstantBuffer(const std::wstring& _name);  


    protected:
        tBarType mBarType;

    public:
        // 0�̸� ����, 1�̸� ����
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
            // ������ ����ȭ 
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

        // ���̴��� switch case�� ���� ���� �ֱ� 
        // enum ���� ���� eType 
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


        // enum ���� ���� eTextureType 
        eTextureType GetTexType() { return mBarType.mTexType; }


	};
}


