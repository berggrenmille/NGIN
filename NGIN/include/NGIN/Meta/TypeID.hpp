#include <cstddef>

namespace NGIN::Meta
{
    template <typename T>
    struct TypeIDResolver
    {
        static void ID() {}
    };

    template <typename T>
    size_t TypeID()
    {
        return reinterpret_cast<size_t>(&TypeIDResolver<T>::ID);
    }
}