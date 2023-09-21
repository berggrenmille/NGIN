#pragma once

#include "Delegate/StaticDelegate.hpp"
#include "Delegate/DynamicDelegate.hpp"

namespace NGIN
{
    template <typename FuncType>
    using StaticDelegate = NGIN::Util::StaticDelegate<FyncType>;
    using DynamicDelegate = NGIN::Util::DynamicDelegate;
}