#pragma once

#include <list>
#include <string>
#include <ionshared/tracking/symbol_table.h>
#include <ionshared/misc/helpers.h>
#include <ionshared/container/map.h>

namespace ionir {
    template<typename T>
    using PtrSymbolTable = std::shared_ptr<ionshared::SymbolTable<std::shared_ptr<T>>>;

    template<typename TKey, typename TValue>
    using ScopeListItem = ionshared::Map<TKey, TValue>;

    template<typename TKey, typename TValue>
    using ScopeList = std::list<ScopeListItem<TKey, TValue>>;
}
