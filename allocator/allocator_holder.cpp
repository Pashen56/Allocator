#include "allocator_holder.h"

void *allocator_holder::allocate_with_guard(
    size_t block_size) const
{
    auto *allocator = get_allocator();

    return allocator == nullptr
        ? ::operator new(block_size)
        : allocator->allocate(block_size);
}

void allocator_holder::deallocate_with_guard(
    void *block_pointer) const
{
    auto *allocator = get_allocator();

    allocator == nullptr
        ? ::operator delete(block_pointer)
        : allocator->deallocate(block_pointer);
}
