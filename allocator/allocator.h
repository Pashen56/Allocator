#ifndef DATA_STRUCTURES_CPP_MEMORY_H
#define DATA_STRUCTURES_CPP_MEMORY_H

// #include <corecrt.h>
#include <string>
#include "logger.h"

class allocator
{

public:

    class memory_exception final:
        public std::exception
    {

    private:

        std::string _exception_message;

    public:

        explicit memory_exception(
            std::string exception_message);

    public:

        [[nodiscard]] char const *what() const noexcept override;

    };

public:

    virtual ~allocator() noexcept = default;

    allocator(
        allocator const &other) = delete;

    explicit allocator(
        allocator &&other) = delete;

    allocator &operator=(
        allocator const &other) = delete;

    allocator &operator=(
        allocator &&other) = delete;

protected:

    allocator() = default;

protected:

    [[nodiscard]] virtual size_t get_trusted_memory_size() const;

    [[nodiscard]] virtual size_t get_allocator_service_block_size() const;

    [[nodiscard]] virtual size_t get_available_block_service_block_size() const;

    [[nodiscard]] virtual size_t get_occupied_block_service_block_size() const;

    [[nodiscard]] virtual bool get_block_occupancy(
        void const *block_pointer) const;

    [[nodiscard]] virtual void **get_first_available_block_address_address() const;

    [[nodiscard]] virtual void *get_first_available_block_address() const;

    [[nodiscard]] virtual void **get_first_occupied_block_address_address() const;

    [[nodiscard]] virtual void *get_first_occupied_block_address() const;

    [[nodiscard]] virtual size_t get_available_block_size(
        void const *current_block_address) const;

    [[nodiscard]] virtual size_t get_occupied_block_size(
        void const *current_block_address) const;

    virtual void *get_available_block_previous_available_block_address(
        void const *current_block_address) const;

    virtual void *get_available_block_next_available_block_address(
        void const *current_block_address) const;

    virtual void *get_occupied_block_previous_occupied_block_address(
        void const *current_block_address) const;

    virtual void *get_occupied_block_next_occupied_block_address(
        void const *current_block_address) const;

    virtual void *get_available_block_previous_block_address(
        void const *current_block_address) const;

    virtual void *get_available_block_next_block_address(
        void const *current_block_address) const;

    virtual void *get_occupied_block_previous_block_address(
        void const *current_block_address) const;

    virtual void *get_occupied_block_next_block_address(
        void const *current_block_address) const;

    void dump_occupied_block_before_deallocate(
        void const *current_block_address,
        logger *logger) const;

    virtual void dump_trusted_memory_blocks_state() const;

public:

    [[nodiscard]] virtual void *allocate(
        size_t requested_block_size) = 0;

    virtual void deallocate(
        void *block_to_deallocate_address) = 0;

    [[nodiscard]] virtual void *reallocate(
        void *block_to_reallocate_address,
        size_t new_block_size) = 0;

    virtual bool reallocate(
        void **block_to_reallocate_address_address,
        size_t new_block_size) = 0;

public:

    void *operator+=(
        size_t requested_block_size);

    void operator-=(
        void *block_to_deallocate_address);

public:

    [[nodiscard]] void *allocate(
        size_t entities_count,
        size_t entity_size);

protected:

    [[nodiscard]] static std::string address_to_hex(
        void const *pointer) noexcept;

};

#endif // DATA_STRUCTURES_CPP_MEMORY_H
