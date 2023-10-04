//
// Created by Maximiliam Berggren on 03/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/Memory/Allocator.hpp>
// STL Include(s)
// Other Include(s)

namespace NGIN::Memory
{
    template<typename AllocT, typename ObjectT, typename... Args>
    inline Ref<ObjectT> AllocateRef(Allocator<AllocT>& allocator, Args&& ... args)
    {
        // Use allocator to create a new ObjectT instance.
        ObjectT* rawPtr = allocator.template New<ObjectT>(std::forward<Args>(args)...);

        // Wrap the raw pointer in a Ref.
        // Use a custom deleter that calls the allocator's Delete function.
        return Ref<ObjectT>(
                rawPtr,
                [&allocator](ObjectT* obj)
                {
                    allocator.Delete(obj);
                }
        );
    }

    template<typename AllocT, typename ObjectT, typename... Args>
    inline Scope<ObjectT> AllocateScope(Allocator<AllocT>& allocator, Args&& ... args)
    {
        // Use allocator to create a new ObjectT instance.
        ObjectT* rawPtr = allocator.template New<ObjectT>(std::forward<Args>(args)...);

        // Wrap the raw pointer in a Scope.
        // Use a custom deleter that calls the allocator's Delete function.
        return Scope<ObjectT>(
                rawPtr,
                [&allocator](ObjectT* obj)
                {
                    allocator.Delete(obj);
                }
        );
    }

}


