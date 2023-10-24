#ifndef DATA_STRUCTURES_CPP_MEMORY_WITH_BASE_DEALLOCATION_H
#define DATA_STRUCTURES_CPP_MEMORY_WITH_BASE_DEALLOCATION_H

#include "typename_holder.h"
#include "logger.h"
#include "logger_holder.h"
#include "allocator.h"
#include "allocator_fit_allocation.h"
#include "allocator_holder.h"

class allocator_base final :
    public allocator_fit_allocation,
    protected logger_holder,
    protected typename_holder,
    protected allocator_holder
{

private:

    void* _trusted_memory;
    allocator* outer_allocator_;

public:

    explicit allocator_base(
        size_t memory_size,
        allocator* outer_allocator = nullptr,
        logger* logger = nullptr,
        allocator_fit_allocation::allocation_mode allocation_mode = allocator_fit_allocation::allocation_mode::first_fit);

    allocator_base(
        allocator_base const& other) = delete;

    allocator_base& operator=(
        allocator_base const& other) = delete;

    ~allocator_base() noexcept;

private:

    [[nodiscard]] size_t get_trusted_memory_size() const noexcept override;

    [[nodiscard]] allocator_fit_allocation::allocation_mode get_allocation_mode() const noexcept override;

    [[nodiscard]] size_t get_allocator_service_block_size() const noexcept override;

    [[nodiscard]] size_t get_available_block_service_block_size() const noexcept override;

    [[nodiscard]] size_t get_occupied_block_service_block_size() const noexcept override;

    [[nodiscard]] void** get_first_available_block_address_address() const noexcept override;

    [[nodiscard]] void* get_first_available_block_address() const noexcept override;

    size_t get_available_block_size(
        void const* current_block_address) const override;

    void* get_available_block_next_available_block_address(
        void const* current_block_address) const override;

    size_t get_occupied_block_size(
        void const* current_block_address) const override;

    void dump_trusted_memory_blocks_state() const override;

public:

    void* allocate(
        size_t requested_block_size) override;

    void deallocate(
        void* block_to_deallocate_address) override;

    [[nodiscard]] void* reallocate(
        void* block_to_reallocate_address,
        size_t new_block_size) override;

    bool reallocate(
        void** block_to_reallocate_address_address,
        size_t new_block_size) override;

public:

    void setup_allocation_mode(
        allocator_fit_allocation::allocation_mode mode) override;

private:

    [[nodiscard]] logger* get_logger() const noexcept override;

private:

    [[nodiscard]] std::string get_typename() const noexcept override;

private:

    [[nodiscard]] allocator* get_allocator() const noexcept override;

};

#endif //DATA_STRUCTURES_CPP_MEMORY_WITH_BASE_DEALLOCATION_H
