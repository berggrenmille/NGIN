#include <cstddef>

namespace NGIN::Meta
{
    template <typename T>
    struct type
    {
        static void id() {}
    };

    template <typename T>
    size_t TypeId()
    {
        return reinterpret_cast<size_t>(&type<T>::id);
    }
}