#include "allocator_descriptor.h"

allocator_descriptor::allocator_descriptor(
    size_t memory_size,
    allocator* outer_allocator,
    logger* log,
    allocator_fit_allocation::allocation_mode allocation_mode)
{
    auto got_typename = get_typename();

    if (log != nullptr)
    {
        log->trace(got_typename + " allocator instance construction started")
            ->debug("requested memory size: " + std::to_string(memory_size) + " bytes");
    }

    auto const minimal_trusted_memory_size = get_available_block_service_block_size();

    if (memory_size < minimal_trusted_memory_size)
    {
        auto error_message = "trusted memory size should be GT " + std::to_string(minimal_trusted_memory_size) + " bytes";

        if (log != nullptr)
        {
            log->error(error_message);
        }

        throw allocator::memory_exception(error_message);
    }

    auto const allocator_service_block_size = get_allocator_service_block_size();

    _trusted_memory = outer_allocator == nullptr
        ? ::operator new(memory_size + allocator_service_block_size)
        : outer_allocator->allocate(memory_size + allocator_service_block_size);

    auto* const memory_size_space = reinterpret_cast<size_t*>(_trusted_memory);
    *memory_size_space = memory_size;

    auto* const outer_allocator_pointer_space = reinterpret_cast<allocator**>(memory_size_space + 1);
    *outer_allocator_pointer_space = outer_allocator;

    auto* const logger_pointer_space = reinterpret_cast<logger**>(outer_allocator_pointer_space + 1);
    *logger_pointer_space = log;

    auto* const allocation_mode_space = reinterpret_cast<allocator_fit_allocation::allocation_mode*>(logger_pointer_space + 1);
    *allocation_mode_space = allocation_mode;

    auto* const first_available_block_pointer_space = reinterpret_cast<void**>(allocation_mode_space + 1);
    *first_available_block_pointer_space = reinterpret_cast<void*>(first_available_block_pointer_space + 1);

    auto* const first_available_block_size_space = reinterpret_cast<size_t*>(*first_available_block_pointer_space);
    *first_available_block_size_space = memory_size;

    auto* const first_available_block_next_block_address_space = reinterpret_cast<void**>(first_available_block_size_space + 1);
    *first_available_block_next_block_address_space = nullptr;

    this->trace_with_guard(got_typename + " allocator instance construction finished");
}

allocator_descriptor::~allocator_descriptor() noexcept
{
    auto got_typename = get_typename();
    this->trace_with_guard(got_typename + " allocator instance destruction started");

    auto const* const logger = get_logger();

    deallocate_with_guard(_trusted_memory);

    if (logger != nullptr)
    {
        logger->trace(got_typename + " allocator instance destruction finished");
    }
}

size_t allocator_descriptor::get_trusted_memory_size() const noexcept
{
    return *reinterpret_cast<size_t*>(_trusted_memory);
}

allocator_fit_allocation::allocation_mode allocator_descriptor::get_allocation_mode() const noexcept
{
    return *reinterpret_cast<allocator_fit_allocation::allocation_mode*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(size_t) + sizeof(allocator*) + sizeof(logger*));
}

size_t allocator_descriptor::get_allocator_service_block_size() const noexcept
{
    auto const memory_size_size = sizeof(size_t);
    auto const outer_allocator_pointer_size = sizeof(allocator*);
    auto const logger_pointer_size = sizeof(logger*);
    auto const allocation_mode_size = sizeof(allocator_fit_allocation::allocation_mode);
    auto const first_available_block_pointer_size = sizeof(void*);

    return memory_size_size + outer_allocator_pointer_size + logger_pointer_size + allocation_mode_size + first_available_block_pointer_size;
}

size_t allocator_descriptor::get_available_block_service_block_size() const noexcept
{
    auto const current_block_size = sizeof(size_t);
    auto const next_available_block_pointer_size = sizeof(void*);

    return current_block_size + next_available_block_pointer_size;
}

size_t allocator_descriptor::get_occupied_block_service_block_size() const noexcept
{
    auto const current_block_size = sizeof(size_t);

    return current_block_size;
}

void** allocator_descriptor::get_first_available_block_address_address() const noexcept
{
    return reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(size_t) + sizeof(allocator*) + sizeof(logger*) + sizeof(allocator_fit_allocation::allocation_mode));
}

void* allocator_descriptor::get_first_available_block_address() const noexcept
{
    return *get_first_available_block_address_address();
}

size_t allocator_descriptor::get_available_block_size(
    void const* current_block_address) const
{
    return *reinterpret_cast<size_t const*>(current_block_address);
}

void* allocator_descriptor::get_available_block_next_available_block_address(
    void const* current_block_address) const
{
    return *reinterpret_cast<void* const*>(reinterpret_cast<size_t const*>(current_block_address) + 1);
}

size_t allocator_descriptor::get_occupied_block_size(
    void const* current_block_address) const
{
    return *reinterpret_cast<size_t const*>(current_block_address);
}

void allocator_descriptor::dump_trusted_memory_blocks_state() const
{
    if (get_logger() == nullptr)
    {
        return;
    }

    std::string to_dump("|");
    auto memory_size = get_trusted_memory_size();
    auto current_available_block = get_first_available_block_address();
    unsigned char* first_block = reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_service_block_size();
    unsigned char* current_block = first_block;

    while (current_block - first_block < memory_size)
    {
        size_t current_block_size;
        if (current_block == current_available_block)
        {
            current_block_size = get_available_block_size(current_block);
            to_dump += "avl ";
            current_available_block = get_available_block_next_available_block_address(current_available_block);
        }
        else
        {
            current_block_size = get_occupied_block_size(current_block);
            to_dump += "occ ";
        }

        to_dump += std::to_string(current_block_size) + "|";
        current_block += current_block_size;
    }

    this->debug_with_guard("Memory state: " + to_dump);
}

void* allocator_descriptor::allocate(
    size_t requested_block_size)
{
    auto const got_typename = get_typename();
    this->trace_with_guard("Method `void *" + got_typename + "::allocate(size_t requested_block_size)` execution started")
        ->debug_with_guard("Requested " + std::to_string(requested_block_size) + " bytes of memory");

    auto requested_block_size_overridden = requested_block_size;
    if (requested_block_size_overridden < sizeof(void*))
    {
        requested_block_size_overridden = sizeof(void*);
    }

    void* previous_block = nullptr, * current_block = get_first_available_block_address();
    void* target_block = nullptr, * previous_to_target_block = nullptr, * next_to_target_block = nullptr;
    auto const available_block_service_block_size = get_available_block_service_block_size();
    auto const occupied_block_service_block_size = get_occupied_block_service_block_size();
    auto const allocation_mode = get_allocation_mode();

    while (current_block != nullptr)
    {
        auto const current_block_size = get_available_block_size(current_block);
        auto const next_block = get_available_block_next_available_block_address(current_block);

        if (current_block_size >= requested_block_size_overridden + occupied_block_service_block_size)
        {
            if (allocation_mode == allocator_fit_allocation::allocation_mode::first_fit ||
                allocation_mode == allocator_fit_allocation::allocation_mode::the_best_fit && (target_block == nullptr || current_block_size < get_available_block_size(target_block)) ||
                allocation_mode == allocator_fit_allocation::allocation_mode::the_worst_fit && (target_block == nullptr || current_block_size > get_available_block_size(target_block)))
            {
                previous_to_target_block = previous_block;
                target_block = current_block;
                next_to_target_block = next_block;
            }

            if (allocation_mode == allocator_fit_allocation::allocation_mode::first_fit)
            {
                break;
            }
        }

        previous_block = current_block;
        current_block = next_block;
    }

    if (target_block == nullptr)
    {
        auto const warning_message = "no memory available to allocate";

        this->warning_with_guard(warning_message)
            ->trace_with_guard("Method `void *" + got_typename + "::allocate(size_t requested_block_size)` execution finished");

        throw memory_exception(warning_message);
    }

    auto const target_block_size = get_available_block_size(target_block);

    if (target_block_size - requested_block_size_overridden - occupied_block_service_block_size < available_block_service_block_size)
    {
        requested_block_size_overridden = target_block_size - occupied_block_service_block_size;
    }

    if (requested_block_size_overridden != requested_block_size)
    {
        this->trace_with_guard("Requested " + std::to_string(requested_block_size) + " bytes, but reserved " + std::to_string(requested_block_size_overridden) + " bytes in according to correct work of allocator");

        requested_block_size = requested_block_size_overridden;
    }

    void* updated_next_block_to_previous_block;

    if (requested_block_size == target_block_size - occupied_block_service_block_size)
    {
        updated_next_block_to_previous_block = next_to_target_block;
    }
    else
    {
        updated_next_block_to_previous_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(target_block) + occupied_block_service_block_size + requested_block_size);

        auto* const target_block_leftover_size = reinterpret_cast<size_t*>(updated_next_block_to_previous_block);
        *target_block_leftover_size = target_block_size - occupied_block_service_block_size - requested_block_size;

        auto* const target_block_leftover_next_block_address = reinterpret_cast<void**>(target_block_leftover_size + 1);
        *target_block_leftover_next_block_address = next_to_target_block;
    }

    previous_to_target_block == nullptr
        ? *get_first_available_block_address_address() = updated_next_block_to_previous_block
        : *reinterpret_cast<void**>(reinterpret_cast<size_t*>(previous_to_target_block) + 1) = updated_next_block_to_previous_block;

    auto* target_block_size_address = reinterpret_cast<size_t*>(target_block);
    *target_block_size_address = requested_block_size + sizeof(size_t);

    auto* const allocated_block = reinterpret_cast<void*>(target_block_size_address + 1);

    this->trace_with_guard("Allocated block placed at " + address_to_hex(allocated_block))
        ->trace_with_guard("Method `void *" + got_typename + "::allocate(size_t requested_block_size)` execution finished");

    this->debug_with_guard("After `allocate` for " + std::to_string(requested_block_size) + " bytes (addr == " +
        address_to_hex(target_block_size_address) + "):");
    dump_trusted_memory_blocks_state();
    return allocated_block;
}

void allocator_descriptor::deallocate(
    void* block_to_deallocate_address)
{
    auto got_typename = get_typename();
    this->trace_with_guard(got_typename + "::deallocate(void *block_to_deallocate_address) execution started");

    auto* block_to_deallocate_size_descriptor = reinterpret_cast<size_t*>(block_to_deallocate_address) - 1;
    size_t block_to_deallocate_size = *block_to_deallocate_size_descriptor;

    // Получаем указатель на следующий доступный блок
    void** next_block_address = reinterpret_cast<void**>(block_to_deallocate_size_descriptor + 1);
    void* next_block = *next_block_address;

    // Получаем указатель на предыдущий доступный блок
    void* previous_block = nullptr;
    unsigned char* current_block_address = reinterpret_cast<unsigned char*>(get_first_available_block_address());

    while (current_block_address != nullptr && current_block_address < block_to_deallocate_address) {
        previous_block = current_block_address;
        current_block_address = reinterpret_cast<unsigned char*>(reinterpret_cast<size_t>(current_block_address) + 1);
    }

    if (previous_block == nullptr) {
        // Предыдущего доступного блока нет, поэтому обновляем начало списка доступных блоков
        *get_first_available_block_address_address() = block_to_deallocate_address;
    }
    else {
        // Обновляем указатель предыдущего доступного блока на следующий доступный блок
        *reinterpret_cast<void**>(reinterpret_cast<size_t*>(previous_block) + 1) = block_to_deallocate_address;
    }

    if (current_block_address != nullptr) {
        // Если есть следующий доступный блок, объединяем блоки
        size_t new_block_size = block_to_deallocate_size + (size_t)(current_block_address - reinterpret_cast<unsigned char*>(block_to_deallocate_address));

        // Обновляем размер и указатель на следующий блок
        *block_to_deallocate_size_descriptor = new_block_size;
        *next_block_address = reinterpret_cast<void*>(reinterpret_cast<size_t*>(block_to_deallocate_address) + 1 + new_block_size / sizeof(size_t));
    }
    else {
        // Если нет следующего доступного блока, обновляем размер и указатель на следующий блок
        *block_to_deallocate_size_descriptor = block_to_deallocate_size;
        *next_block_address = nullptr;
    }

    this->debug_with_guard("After `deallocate` (addr == " + address_to_hex(block_to_deallocate_address) + "):");
    dump_trusted_memory_blocks_state();
    this->trace_with_guard(got_typename + "::deallocate method execution finished");

}

void* allocator_descriptor::reallocate(
    void* block_to_reallocate_address,
    size_t new_block_size)
{
    auto* new_block = allocate(new_block_size);
    auto occupied_block_service_block_size = get_occupied_block_service_block_size();
    auto data_to_move_size = std::min(get_occupied_block_size(reinterpret_cast<unsigned char const*>(new_block) - occupied_block_service_block_size), get_occupied_block_size(reinterpret_cast<unsigned char const*>(block_to_reallocate_address) - occupied_block_service_block_size)) - occupied_block_service_block_size;
    memcpy(new_block, block_to_reallocate_address, data_to_move_size);
    deallocate(block_to_reallocate_address);
    return new_block;
}

bool allocator_descriptor::reallocate(
    void** block_to_reallocate_address_address,
    size_t new_block_size)
{
    try {
        *block_to_reallocate_address_address = reallocate(*block_to_reallocate_address_address, new_block_size);
        return true;
    }
    catch (std::exception const& ex)
    {
        this->warning_with_guard(ex.what());
        return false;
    }
}

void allocator_descriptor::setup_allocation_mode(
    allocator_fit_allocation::allocation_mode mode)
{
    *reinterpret_cast<allocator_fit_allocation::allocation_mode*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(size_t) + sizeof(allocator*) + sizeof(logger*)) = mode;
}

logger* allocator_descriptor::get_logger() const noexcept
{
    return *reinterpret_cast<logger**>(reinterpret_cast<allocator**>(reinterpret_cast<size_t*>(_trusted_memory) + 1) + 1);
}

std::string allocator_descriptor::get_typename() const noexcept
{
    return "allocator_descriptor";
}

allocator* allocator_descriptor::get_allocator() const noexcept
{
    return *reinterpret_cast<allocator**>(reinterpret_cast<size_t*>(_trusted_memory) + 1);
}
