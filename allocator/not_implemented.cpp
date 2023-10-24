#include "not_implemented.h"
#include <string>

not_implemented::not_implemented(
    std::string const &method_name)
    : logic_error("method `" + method_name + "` not implemented")
{

}


