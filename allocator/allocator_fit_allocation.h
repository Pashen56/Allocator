#ifndef DATA_STRUCTURES_CPP_MEMORY_WITH_FIT_ALLOCATION_H
#define DATA_STRUCTURES_CPP_MEMORY_WITH_FIT_ALLOCATION_H

#include "allocator.h"

class allocator_fit_allocation:
    public allocator
{

public:

    enum class allocation_mode
    {
        first_fit,
        the_best_fit,
        the_worst_fit
    };

public:

    allocator_fit_allocation(
        allocator_fit_allocation const &) = delete;

    allocator_fit_allocation &operator=(
        allocator_fit_allocation const &) = delete;

    allocator_fit_allocation(
        allocator_fit_allocation const &&) noexcept = delete;

    allocator_fit_allocation &operator=(
        allocator_fit_allocation &&) noexcept = delete;

protected:

    allocator_fit_allocation() = default;

protected:

    [[nodiscard]] virtual allocation_mode get_allocation_mode() const = 0;

public:

    virtual void setup_allocation_mode(
        allocation_mode mode) = 0;

};

#endif // DATA_STRUCTURES_CPP_MEMORY_WITH_FIT_ALLOCATION_H
