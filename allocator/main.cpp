#include <iostream>
#include "allocator.h"
#include <string>
#include <cstddef>
#include <new>
#include "logger.h"
#include "logger_concrete.h"
#include "logger_builder.h"
#include "logger_builder_concrete.h"
#include "allocator.h"
#include "allocator_sorted_list.h"
#include "allocator_fit_allocation.h"
#include "allocator_base.h"
#include "allocator_descriptor.h"
#include "allocator_double_system.h"

class A
{
private:
    int _value;
    char *_ptr;
public:
    A(int value) : _value(value), _ptr(new char[10]) {}
    // 5 rules
    int get_value() const noexcept
    {
        return _value;
    }
};

int main() {
    
    std::cout << "Program started!" << std::endl << std::endl << std::endl;
    
    // Задание 3
    std::cout << "Allocator - Sorted List:" << std::endl;
    logger_builder *builder_3 = new logger_builder_concrete();
    auto *logger_3 = builder_3
        ->add_console_stream(logger::severity::debug)
        ->add_file_stream("sorted list allocator trace logs.txt", logger::severity::trace)
        ->build();
    delete builder_3;
    
    allocator *alc_3 = new allocator_sorted_list(10000, nullptr, logger_3, allocator_fit_allocation::allocation_mode::first_fit);

    try
    {
        void *ptr_3 = alc_3->allocate(1000);
        void *ptr_3_2 = alc_3->reallocate(ptr_3, 2000);
        
        // TODO: work with memory...
        
        A *obj2 = new A(13);
        obj2->get_value();
        A *obj = reinterpret_cast<A *>(alc_3->allocate(sizeof(A)));
        new (obj) A(13);
        obj->get_value();
        
        // TODO: this is incorrect
        // auto obj_to_copy_from = A(13);
        // std::memcpy(obj, &obj_to_copy_from, sizeof(A));
        
        delete obj2;
        obj->~A();
        alc_3->deallocate(ptr_3_2);
    }
    catch (allocator::memory_exception const &ex)
    {
        std::cout << "memory of size 100 can't be allocated!" << std::endl;
    }
    
    std::cout << std::endl << std::endl;
    
    // Задание 2
    std::cout << "Allocator - Base:" << std::endl;
     logger_builder* builder_2 = new logger_builder_concrete();
     auto* logger_2 = builder_2
         ->add_console_stream(logger::severity::debug)
         ->add_file_stream("base allocator trace logs.txt", logger::severity::trace)
         ->build();
     delete builder_2;

     allocator* alc_2 = new allocator_base(10000, nullptr, logger_2, allocator_fit_allocation::allocation_mode::first_fit);

     try
     {
         void* ptr_2 = alc_2->allocate(999);
         ptr_2 = alc_2->reallocate(ptr_2, 1999);

         // TODO: work with memory...

         alc_2->deallocate(ptr_2);
     }
     catch (allocator::memory_exception const& ex)
     {
         std::cout << "memory of size 100 can't be allocated!" << std::endl;
     }
    
    std::cout << std::endl << std::endl;
    
    // Задание 4
    std::cout << "Allocator - Descriptor:" << std::endl;
    logger_builder* builder_4 = new logger_builder_concrete();
    auto* logger_4 = builder_4
        ->add_console_stream(logger::severity::debug)
        ->add_file_stream("descriptor allocator trace logs.txt", logger::severity::trace)
        ->build();
    delete builder_4;

    allocator* alc_4 = new allocator_descriptor(1200, alc_3, logger_4, allocator_fit_allocation::allocation_mode::first_fit);

    try
    {
        void* ptr_4 = alc_4->allocate(900);
        ptr_4 = alc_4->reallocate(ptr_4, 800);

        // TODO: work with memory...

        alc_4->deallocate(ptr_4);
    }
    catch (allocator::memory_exception const& ex)
    {
        std::cout << "memory of size 100 can't be allocated!" << std::endl;
    }
    
    std::cout << std::endl << std::endl;
    
    // Задание 5
    std::cout << "Allocator - Double System:" << std::endl;
    logger_builder* builder_5 = new logger_builder_concrete();
    auto* logger_5 = builder_5
        ->add_console_stream(logger::severity::debug)
        ->add_file_stream("double system allocator trace logs.txt", logger::severity::trace)
        ->build();
    delete builder_5;

    allocator* alc_5 = new allocator_double_system(10000, nullptr, logger_5, allocator_fit_allocation::allocation_mode::first_fit);

    try
    {
        void* ptr_5 = alc_5->allocate(1000);
        ptr_5 = alc_5->reallocate(ptr_5, 2000);

        // TODO: work with memory...

        alc_5->deallocate(ptr_5);
    }
    catch (allocator::memory_exception const& ex)
    {
        std::cout << "memory of size 100 can't be allocated!" << std::endl;
    }


    delete alc_5;
    delete logger_5;
    
    delete alc_2;
    delete logger_2;
    
    delete alc_4;
    delete logger_4;
    
    delete alc_3;
    delete logger_3;
    
    std::cout << std::endl << std::endl;
    std::cout << "Program ended!" << std::endl << std::endl << std::endl;
    
    return 0;
}
