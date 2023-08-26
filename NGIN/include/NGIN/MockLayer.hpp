/******************************************************************************
 *  Copyright (c) Fri Aug 25 2023 Author. All rights reserved.
 ******************************************************************************/

#pragma once
#include <NGIN/Core.h>
// NGIN include(s)
#include <NGIN/Util/TypeErasure.hpp>
// Standard library include(s)
#include <memory>
#include <type_traits>
#include <utility>
#include <concepts>

namespace NGIN
{

    class MockLayer
    {
    public:
        /**
         * @brief Default constructor.
         */
        MockLayer() = default;

        /**
         * @brief Copy constructor that accepts an instance of any conforming type.
         *
         * @param instance The object to be copied into this type-erased class.
         */
        template <typename T>
        explicit MockLayer(const T &instance)
        {
        }

        /**
         * @brief Move constructor that accepts an instance of any conforming type.
         *
         * @param instance The object to be moved into this type-erased class.
         */
        template <typename T>
        explicit MockLayer(T &&instance)
        {
        }

        void OnAttach() {}
        void OnDetach() {}
        void OnUpdate() {}

    private:
    };
}
