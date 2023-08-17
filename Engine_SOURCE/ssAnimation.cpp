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
		, mIndex(-1) // 처음엔 아무 인덱스에도 속하지 않으므로...
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

			// 접근하려는 index 값이 sprites 벡터 크기 보다 크다면 (끝에 다다랐다는 뜻이므로)
			if (mSprites.size() <= mIndex)
			{
				// 최종 인덱스를 마지막 인덱스로 넣어준다.
				mIndex = mSprites.size() - 1; // 마지막 인덱스의 위치는 -1이므로.

				// complete를 true로 바꿔준다. 이 값으로 Animator::Update()에서
				// complete Event를 호출할 수 있게 해준 것
				mbComplete = true;
			}

		}
		
	}

	// 여기에서 특정 인덱스의 크기 갑자기 커지고 하는거 이벤트 연결시키기 에휴 
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

			// UV값 기준으로 값을 보내주려면 실제 해상도 크기로 나눠주면 된다. 그럼 UV 값 기준으로 값이 나온다. 
			sprite.LT.x = LT.x + (i * Size.x) / width;
			sprite.LT.y = LT.y / height;

			// 아틀라스 중 1 프레임의 크기를 UV값으로 구함 
			sprite.size.x = Size.x / width;
			sprite.size.y = Size.y / height;

			// 기존에는 픽셀 좌표 그대로 전달되었음. 때문에 텍스처 해상도로 나눠서 uv값 기준으로 들어갈 수 있게끔 이것도 만들어줌
			sprite.offset = Vector2(Offset.x / width, Offset.y / height);

			// BackSize개념이다. 캐릭터마다 backsize값 다르므로 그에 맞게 부여해주면 됨
			sprite.atlasSize = Vector2(BackSize.x / width, BackSize.y / height);

			sprite.duration = Duration;
			sprite.reverse = Reverse;

			mSprites.push_back(sprite);
			
		}
	}


	void Animation::Binds()
	{
		// 아틀라스 이미지를 12번 슬롯에 바인딩 
		mAtlas->BindShader(graphics::eShaderStage::PS, 12);

		// 애니메이션 상수버퍼
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
