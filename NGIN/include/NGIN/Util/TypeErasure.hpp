#pragma once
namespace NGIN::Util
{
    /**
     * @brief A custom deleter for the type-erased classes
     *
     * This structure facilitates deletion of type-erased objects. It is intended
     * to be used with `std::unique_ptr` to handle custom deallocation scenarios.
     *
     * Example usage:
     *
     * ```cpp
     * std::unique_ptr<void, Deleter> myPointer(myRawPointer, Deleter(myRawPointer));
     * ```
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