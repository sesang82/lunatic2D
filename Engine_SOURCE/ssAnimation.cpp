#include "ssAnimation.h"
#include "ssTime.h"
#include "ssAnimator.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"
#include "ssCollider2D.h"
#include "ssGameObject.h"

namespace ss
{
	Animation::Animation()
		: Resource(enums::eResourceType::Animation)
		, mIndex(-1) // ó���� �ƹ� �ε������� ������ �����Ƿ�...
		, mbComplete(false)
		, mTime(0.0f)
		, mAnimator(nullptr)
		, mSprites{}
		, mAtlas(nullptr)
		, mDirection(true)
	{
	}

	Animation::~Animation()
	{
	}

	void Animation::Upadte()
	{
		if (mbComplete)
			return;

		mTime += Time::DeltaTime();

		// 
		if (mSprites[mIndex].duration <= mTime)
		{
			mIndex++;
			mTime = 0.0f;

			// �����Ϸ��� index ���� sprites ���� ũ�� ���� ũ�ٸ� (���� �ٴٶ��ٴ� ���̹Ƿ�)
			if (mSprites.size() <= mIndex)
			{
				// ���� �ε����� ������ �ε����� �־��ش�.
				mIndex = mSprites.size() - 1; // ������ �ε����� ��ġ�� -1�̹Ƿ�.

				// complete�� true�� �ٲ��ش�. �� ������ Animator::Update()����
				// complete Event�� ȣ���� �� �ְ� ���� ��
				mbComplete = true;
			}

		}
		
	}

	// ���⿡�� Ư�� �ε����� ũ�� ���ڱ� Ŀ���� �ϴ°� �̺�Ʈ �����Ű�� ���� 
	void Animation::LateUpdate()
	{

	

	}

	void Animation::Render()
	{
	}


	void Animation::Create(std::wstring Name
		, std::shared_ptr<graphics::Texture> Atlas
		, Vector2 LT
		, Vector2 Size
		, UINT ColumnLength
		, Vector2 BackSize
		, Vector2 Offset
		, float Duration
		, bool Reverse)
	{
		SetKey(Name);
		mAtlas = Atlas;


		float width = (float)Atlas->GetWidth();
		float height = (float)Atlas->GetHeight();

		mAnimCount = ColumnLength;

		for (size_t i = 0; i < ColumnLength; i++)
		{
			tSprite sprite = {};

			// UV�� �������� ���� �����ַ��� ���� �ػ� ũ��� �����ָ� �ȴ�. �׷� UV �� �������� ���� ���´�. 
			sprite.LT.x = LT.x + (i * Size.x) / width;
			sprite.LT.y = LT.y / height;

			// ��Ʋ�� �� 1 �������� ũ�⸦ UV������ ���� 
			sprite.size.x = Size.x / width;
			sprite.size.y = Size.y / height;

			// �������� �ȼ� ��ǥ �״�� ���޵Ǿ���. ������ �ؽ�ó �ػ󵵷� ������ uv�� �������� �� �� �ְԲ� �̰͵� �������
			sprite.offset = Vector2(Offset.x / width, Offset.y / height);

			// BackSize�����̴�. ĳ���͸��� backsize�� �ٸ��Ƿ� �׿� �°� �ο����ָ� ��
			sprite.atlasSize = Vector2(BackSize.x / width, BackSize.y / height);

			sprite.duration = Duration;
			sprite.reverse = Reverse;

			mSprites.push_back(sprite);
			
		}
	}


	void Animation::Binds()
	{
		// ��Ʋ�� �̹����� 12�� ���Կ� ���ε� 
		mAtlas->BindShader(graphics::eShaderStage::PS, 12);

		// �ִϸ��̼� �������
		renderer::AnimatorCB data = {};

		data.spriteLeftTop = mSprites[mIndex].LT;
		data.spriteSize = mSprites[mIndex].size;
		data.spriteOffset = mSprites[mIndex].offset;
		data.atlasSize = mSprites[mIndex].atlasSize;
		data.animationType = 1;
		data.reverse = (UINT)mSprites[mIndex].reverse;

		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Animator];
		cb->SetData(&data);

		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	void Animation::Reset()
	{
		mTime = 0.0f;
		mbComplete = false;
		mIndex = 0;


	}

}
