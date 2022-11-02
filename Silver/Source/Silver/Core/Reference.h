#pragma once
#include "IssueReporting.h"
#include <atomic>
#include <concepts>

namespace Silver {

	// Derive your class from RefTarget, then reference it using Ref<classname>

	class RefTarget
	{
	public:
		inline RefTarget() = default;
		inline RefTarget(const RefTarget&) { /* Don't copy refcount */ }
		inline ~RefTarget() { AG_ASSERT(m_RefCount == 0) }

		inline RefTarget& operator=(const RefTarget&) { /* Don't copy refcount */ return *this; }
	private:
		void AddRef() const { ++m_RefCount; }
		void RemoveRef() const { --m_RefCount; }
		uint32_t GetRefCount() const { return m_RefCount.load(); }

		mutable std::atomic<uint32_t> m_RefCount;

		template<typename T, typename BaseType>
		friend class Ref;
	};

	template<typename T, typename BaseType = RefTarget>
	class Ref
	{
	public:
		// Constructors
		Ref() : m_Ptr(nullptr) {}
		Ref(std::nullptr_t) : m_Ptr(nullptr) {}
		Ref(T* InPtr) : m_Ptr(InPtr) { AddRef(); }
		Ref(const Ref<T, BaseType>& InOther) : m_Ptr(InOther.m_Ptr) { AddRef(); }
		Ref(Ref<T, BaseType>&& InOther) noexcept { m_Ptr = InOther.m_Ptr; InOther.m_Ptr = nullptr; }

		template<typename Other, typename OtherBase>
		Ref(const Ref<Other, OtherBase>& InOther) { m_Ptr = static_cast<T*>(InOther.m_Ptr); AddRef(); }

		template<typename Other, typename OtherBase>
		Ref(Ref<Other, OtherBase>&& InOther) noexcept { m_Ptr = static_cast<T*>(InOther.m_Ptr); InOther.m_Ptr = nullptr; }

		~Ref() { RemoveRef(); }

		// Assignment operators
		Ref& operator=(std::nullptr_t) { RemoveRef(); m_Ptr = nullptr; return *this; }
		Ref& operator=(const Ref<T, BaseType>& InOther) { RemoveRef(); m_Ptr = InOther.m_Ptr; AddRef(); return *this; }
		Ref& operator=(Ref<T, BaseType>&& InOther) noexcept { RemoveRef(); m_Ptr = InOther.m_Ptr; InOther.m_Ptr = nullptr; return *this; }

		template<typename Other, typename OtherBase>
		Ref& operator=(const Ref<Other, OtherBase>& InOther) { RemoveRef(); m_Ptr = static_cast<T*>(InOther.m_Ptr); AddRef(); return *this; }

		template<typename Other, typename OtherBase>
		Ref& operator=(Ref<Other, OtherBase>&& InOther) noexcept { RemoveRef(); m_Ptr = static_cast<T*>(InOther.m_Ptr); InOther.m_Ptr = nullptr; return *this; }

		operator bool() { return m_Ptr != nullptr; }
		operator bool() const { return m_Ptr != nullptr; }

		// For accessing like a normal pointer
		T* operator->() { return static_cast<T*>(m_Ptr); }
		const T* operator->() const { return static_cast<const T*>(m_Ptr); }
		T& operator*() { return *static_cast<T*>(m_Ptr); }
		const T& operator*() const { return *static_cast<const T*>(m_Ptr); }

		// Getters
		T* GetPtr() { return static_cast<T*>(m_Ptr); }
		const T* GetPtr() const { return static_cast<T*>(m_Ptr); }

		void Reset(T* InPtr = nullptr) { RemoveRef(); m_Ptr = InPtr; AddRef(); }

		template<typename Other>
		Ref<Other, BaseType> As() const { return Ref<Other, BaseType>(*this); }

		// Comparison
		bool operator==(const Ref<T, BaseType>& InOther) const { return m_Ptr == InOther.m_Ptr; }
		bool operator!=(const Ref<T, BaseType>& InOther) const { return !(*this == InOther); }

		template<typename... TArgs>
		static Ref<T, BaseType> Create(TArgs&&... InArgs) requires std::constructible_from<T, TArgs...>
		{
			return Ref<T, BaseType>(new T(std::forward<TArgs>(InArgs)...));
		}

	private:
		void AddRef()
		{ 
			if (m_Ptr) 
				m_Ptr->AddRef();
		}

		void RemoveRef()
		{
			if (!m_Ptr)
				return;

			m_Ptr->RemoveRef();

			// Free the object and the storage if reference count is 0
			if (m_Ptr->GetRefCount() == 0)
			{
				delete m_Ptr;
				m_Ptr = nullptr;
			}
		}

	private:
		template<class Other, class OtherBase>
		friend class Ref;

		mutable T* m_Ptr;
	};

}