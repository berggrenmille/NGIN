#pragma once
namespace NGIN::Util
{
    /**
     * @brief A structure responsible for providing custom deletion for the type-erased allocator.
     *        Mainly used for std::unique_ptr.
     */
    struct Deleter
    {
        void (*deleterFn)(void *) = nullptr;

        template <typename T>
        Deleter(T *)
        {
            deleterFn = [](void *ptr)
            {
                delete static_cast<T *>(ptr);
            };
        }

        void operator()(void *ptr) const
        {
            if (deleterFn)
            {
                deleterFn(ptr);
            }
        }
    };
}