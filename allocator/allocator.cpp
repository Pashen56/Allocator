#include <sstream>
#include "not_implemented.h"
#include "allocator.h"

allocator::memory_exception::memory_exception(
    std::string exception_message)
    : _exception_message(std::move(exception_message))
{

}

char const * allocator::memory_exception::what() const noexcept
{
    return _exception_message.c_str();
}

size_t allocator::get_trusted_memory_size() const
{
    throw not_implemented("size_t memory::get_trusted_memory_size() const");
}

size_t allocator::get_allocator_service_block_size() const
{
    throw not_implemented("size_t memory::get_allocator_service_block_size() const");
}

size_t allocator::get_available_block_service_block_size() const
{
    throw not_implemented("size_t memory::get_available_block_service_block_size() const");
}

size_t allocator::get_occupied_block_service_block_size() const
{
    throw not_implemented("size_t memory::get_occupied_block_service_block_size() const");
}

bool allocator::get_block_occupancy(
    void const *block_pointer) const
{
    throw not_implemented("bool memory::get_block_occupancy(void * const) const");
}

void **allocator::get_first_available_block_address_address() const
{
    throw not_implemented("void **memory::get_first_available_block_address_address() const");
}

void *allocator::get_first_available_block_address() const
{
    throw not_implemented("void *memory::get_first_available_block_address() const");
}

void **allocator::get_first_occupied_block_address_address() const
{
    throw not_implemented("void **memory::get_first_occupied_block_address_address() const");
}

void *allocator::get_first_occupied_block_address() const
{
    throw not_implemented("void *memory::get_first_occupied_block_address() const");
}

size_t allocator::get_available_block_size(
    void const *current_block_address) const
{
    throw not_implemented("size_t memory::get_available_block_size(void * const) const");
}

size_t allocator::get_occupied_block_size(
    void const *current_block_address) const
{
    throw not_implemented("size_t memory::get_occupied_block_size(void * const) const");
}

void *allocator::get_available_block_previous_available_block_address(
    void const *current_block_address) const
{
    throw not_implemented("void *memory::get_available_block_previous_available_block_address(void * const) const");
}

void *allocator::get_available_block_next_available_block_address(
    void const *current_block_address) const
{
    throw not_implemented("void *memory::get_available_block_next_available_block_address(void * const) const");
}

void *allocator::get_occupied_block_previous_occupied_block_address(
    void const *current_block_address) const
{
    throw not_implemented("void *memory::get_occupied_block_previous_occupied_block_address(void * const) const");
}

void *allocator::get_occupied_block_next_occupied_block_address(
    void const *current_block_address) const
{
    throw not_implemented("void *memory::get_occupied_block_next_occupied_block_address(void * const) const");
}

void *allocator::get_available_block_previous_block_address(
    void const *current_block_address) const
{
    throw not_implemented("void *memory::get_available_block_previous_block_address(void * const) const");
}

void *allocator::get_available_block_next_block_address(
    void const *current_block_address) const
{
    throw not_implemented("void *memory::get_available_block_next_block_address(void * const) const");
}

void *allocator::get_occupied_block_previous_block_address(
    void const *current_block_address) const
{
    throw not_implemented("void *memory::get_occupied_block_previous_block_address(void * const) const");
}

void *allocator::get_occupied_block_next_block_address(
    void const *current_block_address) const
{
    throw not_implemented("void *memory::get_occupied_block_next_block_address(void * const) const");
}

void allocator::dump_occupied_block_before_deallocate(
    void const *current_block_address,
    logger *logger) const
{
    if (logger == nullptr)
    {
        return;
    }

    logger->trace("Method memory::dump_occupied_block_before_deallocate(void * const current_block_address, logger *logger) const execution started");

    auto const current_block_size = get_occupied_block_size(current_block_address);
    auto const *dump_iterator = reinterpret_cast<unsigned char const *>(reinterpret_cast<size_t const *>(current_block_address) + 1);
    std::string result;

    for (auto i = 0; i < current_block_size; i++)
    {
        result += std::to_string(static_cast<unsigned short>(*dump_iterator++));

        if (i != current_block_size - 1)
        {
            result += ' ';
        }
    }

    logger->trace("Memory at " + address_to_hex(current_block_address) + " = [" + result + "]")
        ->trace("Method memory::dump_occupied_block_before_deallocate(void * const current_block_address, logger *logger) const execution finished");
}

void allocator::dump_trusted_memory_blocks_state() const
{
    throw not_implemented("void memory::dump_trusted_memory_blocks_state() const");
}

void *allocator::operator+=(
    size_t requested_block_size)
{
    return allocate(requested_block_size);
}

void allocator::operator-=(
    void *block_to_deallocate_address)
{
    deallocate(block_to_deallocate_address);
}

void *allocator::allocate(
    size_t entities_count,
    size_t entity_size)
{
    return allocate(entities_count * entity_size);
}

std::string allocator::address_to_hex(
    void const * const pointer) noexcept
{
    return std::string { (std::stringstream() << pointer).str() };
}
