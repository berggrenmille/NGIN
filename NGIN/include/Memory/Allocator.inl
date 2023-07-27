#include <Memory/Allocator.h>
namespace NGIN
{
	template <class T, class... Args>
	inline T* Allocator::New(Args&&... args, const std::source_location& location)
	{
		return new (Allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
	}

	template<class T>
	inline void Allocator::Delete(T* object, const std::source_location& location)
	{
		object.~T();
		Deallocate(&object);
	}
}