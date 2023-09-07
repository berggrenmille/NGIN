#pragma once

namespace NGIN::Meta
{
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