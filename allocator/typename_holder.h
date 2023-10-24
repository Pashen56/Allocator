#ifndef DATA_STRUCTURES_CPP_TYPENAME_HOLDER_H
#define DATA_STRUCTURES_CPP_TYPENAME_HOLDER_H

#include <string>

class typename_holder
{

public:

    virtual ~typename_holder() noexcept = default;

protected:

    [[nodiscard]] virtual std::string get_typename() const noexcept = 0;

};

#endif //DATA_STRUCTURES_CPP_TYPENAME_HOLDER_H