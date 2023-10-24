#ifndef DATA_STRUCTURES_CPP_LOGGER_BUILDER_CONCRETE_H
#define DATA_STRUCTURES_CPP_LOGGER_BUILDER_CONCRETE_H

#include <map>
#include "logger_builder.h"
#include "logger_holder.h"

class logger_builder_concrete final:
    public logger_builder,
    protected logger_holder
{

private:

    std::map<std::string, logger::severity> _streams_collected_information;
    logger *_logger;

public:

    explicit logger_builder_concrete(
        logger *log = nullptr);

    ~logger_builder_concrete() noexcept override;

public:

    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) override;

    logger_builder *add_console_stream(
        logger::severity severity) override;

    logger_builder* transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path) override;

    logger_builder *clear() override;

    [[nodiscard]] logger *build() const override;

private:

    [[nodiscard]] logger *get_logger() const noexcept override;

};

#endif // DATA_STRUCTURES_CPP_LOGGER_BUILDER_CONCRETE_H
