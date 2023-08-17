#pragma once
#include "ssResource.h"

namespace ss
{
	class Resources
	{
	public:
		template <typename T>
		static std::shared_ptr<T> Find(const std::wstring& key)
		{
			//리소스맵에서 데이터를 탐색한다 데이터가 있다면 해당데이터를 반환하고
			//데이터가 없다면 end를 반환해준다.
			std::map<std::wstring, std::shared_ptr<Resource>>::iterator iter = mResources.find(key);

			//찾고자 하는 데이터가 존재한다면
			//해당타입으로 형변환하여 반환
			if (iter != mResources.end())
			{
				// shared_ptr을 쓴다면 dynamice_cast<> 대신 
				// std::dynamic_pointer_cast<>를 써야한다.
				// dynamic_cast<T*> => dynamic_pointer_cast<T>
				return std::dynamic_pointer_cast<T>(iter->second);
			}

			//데이터 없다면 널을 반환
			return nullptr;
		}

		template <typename T>
		static std::shared_ptr<T> Load(const std::wstring& key, const std::wstring& path)
		{
			// 키값으로 탐색
			std::shared_ptr<T> resource = Resources::Find<T>(key);
			if (resource != nullptr)
			{
				return resource;
			}

			// 해당 리소스가 없다면
			// shared_ptr을 쓴다면 new 연산자 대신에 std::make_shared<>를 써야한다.
			resource = std::make_shared<T>();
			if (FAILED(resource->Load(path)))
			{
				assert(false);
				return nullptr;
			}

			resource->SetKey(key);
			resource->SetPath(path);
			mResources.insert(std::make_pair(key, resource));

			return std::dynamic_pointer_cast<T>(resource);
		}

		// 리소스 배열에 넣어주는 함수(Resources::Find함수를 쓰려면 필수)
		template <typename T>
		static void Insert(const std::wstring& key, std::shared_ptr<T> resource)
		{
			// 중복된 키가 없다면 
			if (mResources.find(key) == mResources.end())
			{
				mResources.insert(std::make_pair(key, resource));
			}

			return;
		}

	private:
		static std::map<std::wstring, std::shared_ptr<Resource>> mResources;
	};
}

