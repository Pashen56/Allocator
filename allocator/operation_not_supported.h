#ifndef DATA_STRUCTURES_CPP_OPERATION_NOT_SUPPORTED_H
#define DATA_STRUCTURES_CPP_OPERATION_NOT_SUPPORTED_H

#include <stdexcept>

class operation_not_supported final:
    public std::logic_error
{

public:

    explicit operation_not_supported();

};

#endif //DATA_STRUCTURES_CPP_OPERATION_NOT_SUPPORTED_H