#ifndef DATA_STRUCTURES_CPP_LOGGER_CONCRETE_H
#define DATA_STRUCTURES_CPP_LOGGER_CONCRETE_H

#include "logger.h"
#include "logger_builder_concrete.h"
#include <map>

class logger_concrete final:
    public logger
{

    friend class logger_builder_concrete;

private:

    std::map<std::string, std::pair<std::ofstream *, logger::severity> > _logger_streams;

private:

    static std::map<std::string, std::pair<std::ofstream *, size_t> > _streams;

private:

    explicit logger_concrete(
        std::map<std::string, logger::severity> const &);

public:

    logger_concrete(
        logger_concrete const &other) = delete;

    logger_concrete &operator=(
        logger_concrete const &other) = delete;

    ~logger_concrete() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif // DATA_STRUCTURES_CPP_LOGGER_CONCRETE_H
