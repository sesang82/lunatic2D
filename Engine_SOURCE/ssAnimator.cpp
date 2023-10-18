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

		//		// �̺�Ʈ ȣ�� �÷��� ����
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
				// �ִϸ��̼� �Ϸ� �̺�Ʈ ȣ��
				events->completeEvent();

				// mEventCalledFlags �ʱ�ȭ
				std::fill(events->mEventCalledFlags.begin(), events->mEventCalledFlags.end(), false);
			}

			// ���� �ִϸ��̼��� ���� ���¶�� Reset
			else if (events && mbLoop)
			{
				// mEventCalledFlags �ʱ�ȭ
				std::fill(events->mEventCalledFlags.begin(), events->mEventCalledFlags.end(), false);

				mActiveAnimation->Reset();
			}
		}



		// ���� �ִϸ��̼��� ������ �ε����� �����ɴϴ�.
		int currentFrameIndex = mActiveAnimation->GetIndex();

		// �ش� �����ӿ� �̺�Ʈ�� ��ϵǾ� �ִ��� Ȯ���մϴ�.
		Events* events = FindEvents(mActiveAnimation->GetKey());
		 
		if (events &&
			currentFrameIndex < events->mFrameEvents.size() &&
			currentFrameIndex < events->mEventCalledFlags.size() &&
			!events->mEventCalledFlags[currentFrameIndex] &&
			events->mFrameEvents[currentFrameIndex].mEvent)
		{
			// �̺�Ʈ�� ȣ���մϴ�.
			events->mFrameEvents[currentFrameIndex].mEvent();

			// �̺�Ʈ ȣ�� �÷��׸� �����մϴ�.
			events->mEventCalledFlags[currentFrameIndex] = true;
		}


//		// ���� �ִϸ��̼��� ������ �ε����� �����ɴϴ�.
//		int currentFrameIndex = mActiveAnimation->GetIndex();
//
//		// �ش� �����ӿ� �̺�Ʈ�� ��ϵǾ� �ִ��� Ȯ���մϴ�.
//		Events* events = FindEvents(mActiveAnimation->GetKey());
//		if (events &&
//			currentFrameIndex < events->mFrameEvents.size() &&
//			events->mFrameEvents[currentFrameIndex].mEvent)
//{		
//			// �̺�Ʈ�� ȣ���մϴ�.
//			events->mFrameEvents[currentFrameIndex]();
//
//		}
	}


	
	void Animator::LateUpdate()
	{
		if (mActiveAnimation != nullptr)
		mActiveAnimation->Upadte(); // �̰� �ȵ��� �ִϸ��̼� ���缭 ���� 
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
		// Ű ���� �ش��ϴ� �̺�Ʈ�� �ִٸ� ã�ƿͼ� 
		Events* events = FindEvents(key);

		// endEvent �Լ� �����͸� ��ȯ���ش�. 
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

		//// Ű ���� �ش��ϴ� �̺�Ʈ�� �ִٸ� ã�ƿͼ� 
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
		// Ȱ��ȭ�� �ִϸ��̼��� ���� �ִϸ��̼� ������ ����
		
		if (mActiveAnimation != nullptr)
		{
			prevAnimation = mActiveAnimation;

			// Ȱ��ȭ�� �ִϸ��̼ǰ� �̸��� ����, ���� ���� ���ΰ� �ƴϸ鼭 hit���µ� �ƴ϶�� ���� 
			if (mActiveAnimation->GetKey() == name && !mAgainAttack)
				return;
		}


		// Events ��ü ����
		Events* events;

		// ���� �ִϸ��̼� ������ �ִٸ� (��� ���̾��ٴ� ���̹Ƿ�)
		if (nullptr != prevAnimation)
		{
			// ���� �ִϸ��̼����κ��� Ű ���� �����ͼ� ���� �ִϸ��̼ǿ� ���� �̺�Ʈ�� �־����� ã��
			events = FindEvents(prevAnimation->GetKey());

			// �̺�Ʈ�� �־��ٸ�
			if (events)

				// ���� �̺�Ʈ�� �����Ѵ�. 
				events->endEvent();
		}


		// ���ڷ� ���� �̸��� �ִϸ��̼��� ã�´�.
		Animation* anim = FindAnimation(name);

		// �ִϸ��̼��� ã�Ҵٸ�
		if (anim)
		{
			// Ȱ��ȭ ���� �ִϸ��̼����� �����Ѵ�.
			mActiveAnimation = anim;
		}


		// Ȱ��ȭ ���� �ִϸ��̼����κ��� Ű�� �� �̺�Ʈ�� ã�´�. 
		events = FindEvents(mActiveAnimation->GetKey());

		// �̺�Ʈ�� �־��ٸ�
		if (events)
		{
			// �̺�Ʈ�� ������ ����Ѵ�. 
			events->startEvent();
		}

		// Ư�� �ε����� ������ �̺�Ʈ�� ȣ���Ѵ�.
	

		// ���ڷ� ���� loop ���θ� �����Ѵ�.
		mbLoop = loop;

		// �� ���Ŀ� loop�� false�� �ٲ����� �����Ѵ�.



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

		// �ִϸ��̼� ã��
		Animation* animation = FindAnimation(name);
		if (nullptr != animation)
			return;

		// �ִϸ��̼� ����
		animation = new Animation();
		animation->SetKey(name);

		// ����
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

		// �̹� �ִ� �̺�Ʈ��� ���� 
		if (events != nullptr)
			return;

		// ���� �̺�Ʈ��� ���� ���� 
		events = new Events();
		events->mFrameEvents.resize(columnLength); // �ִϸ��̼Ǹ��� ������ �ٸ��׹Ƿ� resize�� ���ش�. 
		events->mEventCalledFlags.resize(columnLength, false);  // �߰��� �ڵ�

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
		// �̺�Ʈ�� ���ִ� map���� �̸����� �ش��ϴ� event�� ã�´�.
		std::map<std::wstring, Events*>::iterator iter = mEvents.find(name);

		if (iter == mEvents.end())
			return nullptr;

		// �̸��� �ش��ϴ� Events�� �ִٸ� �ش� �̺�Ʈ(�޸� �ּҰ�)�� ��ȯ
		return iter->second;
	}
}