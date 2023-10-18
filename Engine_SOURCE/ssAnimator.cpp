#include "ssAnimator.h"
#include "ssGameObject.h"
#include "ssCollider2D.h"
#include "../SesangEngine/ssPlayerScript.h"
#include "../SesangEngine/ssMonsterScript.h"

namespace ss
{
	Animator::Animator()
		: Component(eComponentType::Animator)
		, mbLoop(true)
		, mStartIdx(0)
		, mEndIdx(0)
		, mAnimCount(0)
		, mAgainAttack(false)
		, mHit(false)
	{
	}
	Animator::~Animator()
	{
		//  
		for (auto& iter : mAnimations)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		for (auto& iter : mEvents)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}
	void Animator::Initialize()
	{
	}
	void Animator::Update()
	{
		if (mActiveAnimation == nullptr)
			return;

		if (mPrevAnimation != nullptr
			&& mPrevAnimation != mActiveAnimation)
		{

			mPrevAnimation->Reset();
		}

		//if (mActiveAnimation->IsComplete() && mbLoop)
		//{
		//	Events* events
		//		= FindEvents(mActiveAnimation->GetKey());

		//	if (events)
		//	{
		//		events->completeEvent();

		//		// 이벤트 호출 플래그 리셋
		//		std::fill(events->mEventCalledFlags.begin(), events->mEventCalledFlags.end(), false);
		//	}

		//	mActiveAnimation->Reset();
		//}

		//else if (mActiveAnimation->IsComplete() && mbLoop == false)
		//{

		//	mPrevAnimation = mActiveAnimation;

		//	
		//	
		//}

		if (mActiveAnimation->IsComplete())
		{
			Events* events = FindEvents(mActiveAnimation->GetKey());

			if (events && !mbLoop)
			{
				// 애니메이션 완료 이벤트 호출
				events->completeEvent();

				// mEventCalledFlags 초기화
				std::fill(events->mEventCalledFlags.begin(), events->mEventCalledFlags.end(), false);
			}

			// 만약 애니메이션이 루프 상태라면 Reset
			else if (events && mbLoop)
			{
				// mEventCalledFlags 초기화
				std::fill(events->mEventCalledFlags.begin(), events->mEventCalledFlags.end(), false);

				mActiveAnimation->Reset();
			}
		}



		// 현재 애니메이션의 프레임 인덱스를 가져옵니다.
		int currentFrameIndex = mActiveAnimation->GetIndex();

		// 해당 프레임에 이벤트가 등록되어 있는지 확인합니다.
		Events* events = FindEvents(mActiveAnimation->GetKey());
		 
		if (events &&
			currentFrameIndex < events->mFrameEvents.size() &&
			currentFrameIndex < events->mEventCalledFlags.size() &&
			!events->mEventCalledFlags[currentFrameIndex] &&
			events->mFrameEvents[currentFrameIndex].mEvent)
		{
			// 이벤트를 호출합니다.
			events->mFrameEvents[currentFrameIndex].mEvent();

			// 이벤트 호출 플래그를 설정합니다.
			events->mEventCalledFlags[currentFrameIndex] = true;
		}


//		// 현재 애니메이션의 프레임 인덱스를 가져옵니다.
//		int currentFrameIndex = mActiveAnimation->GetIndex();
//
//		// 해당 프레임에 이벤트가 등록되어 있는지 확인합니다.
//		Events* events = FindEvents(mActiveAnimation->GetKey());
//		if (events &&
//			currentFrameIndex < events->mFrameEvents.size() &&
//			events->mFrameEvents[currentFrameIndex].mEvent)
//{		
//			// 이벤트를 호출합니다.
//			events->mFrameEvents[currentFrameIndex]();
//
//		}
	}


	
	void Animator::LateUpdate()
	{
		if (mActiveAnimation != nullptr)
		mActiveAnimation->Upadte(); // 이거 안돌면 애니메이션 멈춰서 나옴 
	}
	void Animator::Render()
	{
	}


	void Animator::Binds()
	{
		if (nullptr == mActiveAnimation)
			return;

		mActiveAnimation->Binds();

	}


	std::function<void()>& Animator::StartEvent(const std::wstring key)
	{
		Events* events = FindEvents(key);

		return events->startEvent.mEvent;
	}

	std::function<void()>& Animator::CompleteEvent(const std::wstring key)
	{
		Events* events = FindEvents(key);
		return events->completeEvent.mEvent;
	}

	std::function<void()>& Animator::EndEvent(const std::wstring key)
	{
		// 키 값에 해당하는 이벤트가 있다면 찾아와서 
		Events* events = FindEvents(key);

		// endEvent 함수 포인터를 반환해준다. 
		return events->endEvent.mEvent;
	}

	std::function<void()>& Animator::RegisterFrameEvent(const std::wstring& animationName, int frameIndex)
	{
		mPrevEventIndex = frameIndex;
		mEventIndex = frameIndex;

		Events* events = FindEvents(animationName);

		if (frameIndex >= events->mFrameEvents.size()) {
			// Error handling: the frameIndex is out of range
			throw std::out_of_range("frameIndex out of range");
		}

		return events->mFrameEvents[frameIndex].mEvent;

		//mPrevEventIndex = frameIndex;
		//mEventIndex = frameIndex;

		//// 키 값에 해당하는 이벤트가 있다면 찾아와서 
		//Events* events = FindEvents(animationName);
		//
		//return events->mFrameEvents[mEventIndex].mEvent;

	}

	//std::function<void()>& Animator::FrameEvent(const std::wstring& _name, UINT _idx)
	//{
	//	Events* events = FindEvents(_name);

	//	return events->mFrameEvents[_idx].mEvent;
	//}

	void Animator::PlayAnimation(const std::wstring& name, bool loop)
	{
		Animation* prevAnimation = nullptr;
		// 활성화된 애니메이션을 이전 애니메이션 값으로 저장
		
		if (mActiveAnimation != nullptr)
		{
			prevAnimation = mActiveAnimation;

			// 활성화된 애니메이션과 이름이 같고, 같은 공격 중인게 아니면서 hit상태도 아니라면 리턴 
			if (mActiveAnimation->GetKey() == name && !mAgainAttack)
				return;
		}


		// Events 객체 생성
		Events* events;

		// 이전 애니메이션 정보가 있다면 (재생 중이었다는 뜻이므로)
		if (nullptr != prevAnimation)
		{
			// 이전 애니메이션으로부터 키 값을 가져와서 이전 애니메이션에 묶인 이벤트가 있었는지 찾음
			events = FindEvents(prevAnimation->GetKey());

			// 이벤트가 있었다면
			if (events)

				// 종료 이벤트를 실행한다. 
				events->endEvent();
		}


		// 인자로 들어온 이름의 애니메이션을 찾는다.
		Animation* anim = FindAnimation(name);

		// 애니메이션을 찾았다면
		if (anim)
		{
			// 활성화 중인 애니메이션으로 저장한다.
			mActiveAnimation = anim;
		}


		// 활성화 중인 애니메이션으로부터 키를 얻어서 이벤트를 찾는다. 
		events = FindEvents(mActiveAnimation->GetKey());

		// 이벤트가 있었다면
		if (events)
		{
			// 이벤트의 시작을 재생한다. 
			events->startEvent();
		}

		// 특정 인덱스의 프레임 이벤트를 호출한다.
	

		// 인자로 들어온 loop 여부를 저장한다.
		mbLoop = loop;

		// 몇 초후에 loop를 false로 바꿔줄지 결정한다.



		Collider2D* col = GetOwner()->GetComponent<Collider2D>();


		mActiveAnimation->Reset();

	
	

	}

	void Animator::Create(const std::wstring& name
		, std::shared_ptr<graphics::Texture> atlas
		, Vector2 leftTop
		, Vector2 size
		, UINT columnLength
		, Vector2 backsize
		, Vector2 offset
		, float duration
		, bool reverse)
	{

		// 애니메이션 찾음
		Animation* animation = FindAnimation(name);
		if (nullptr != animation)
			return;

		// 애니메이션 생성
		animation = new Animation();
		animation->SetKey(name);

		// 만듦
		animation->Create(name
			, atlas
			, leftTop
			, size
			, columnLength
			, backsize
			, offset
			, duration
			, reverse);

		mAnimations.insert(std::make_pair(name, animation));
		

		Events* events = FindEvents(name);

		// 이미 있는 이벤트라면 리턴 
		if (events != nullptr)
			return;

		// 없는 이벤트라면 새로 생성 
		events = new Events();
		events->mFrameEvents.resize(columnLength); // 애니메이션마다 갯수가 다를테므로 resize로 해준다. 
		events->mEventCalledFlags.resize(columnLength, false);  // 추가된 코드

		mEvents.insert(std::make_pair(name, events));

	}

	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		std::map<std::wstring, Animation*>::iterator iter
			= mAnimations.find(name);

		if (iter == mAnimations.end())
			return nullptr;

		return iter->second;

	}


	Animator::Events* Animator::FindEvents(const std::wstring& name)
	{
		// 이벤트가 들어가있는 map에서 이름으로 해당하는 event를 찾는다.
		std::map<std::wstring, Events*>::iterator iter = mEvents.find(name);

		if (iter == mEvents.end())
			return nullptr;

		// 이름에 해당하는 Events가 있다면 해당 이벤트(메모리 주소값)을 반환
		return iter->second;
	}
}