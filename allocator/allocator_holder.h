#ifndef DATA_STRUCTURES_CPP_MEMORY_HOLDER_H
#define DATA_STRUCTURES_CPP_MEMORY_HOLDER_H

#include "allocator.h"

class allocator_holder
{

public:

    virtual ~allocator_holder() noexcept = default;

public:

    [[nodiscard]] void *allocate_with_guard(
        size_t size) const;

    void deallocate_with_guard(
        void *block_pointer) const;

protected:

    [[nodiscard]] virtual allocator *get_allocator() const noexcept = 0;

};

#endif // DATA_STRUCTURES_CPP_MEMORY_HOLDER_H
