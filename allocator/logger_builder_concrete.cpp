#include <iostream>
#include <fstream>
#include "json.hpp"
#include "logger_builder_concrete.h"
#include "logger_concrete.h"

logger_builder_concrete:: logger_builder_concrete(
    logger *log)
    : _logger(log)
{
    this->trace_with_guard("logger_builder_concrete instance constructed.");
}

logger_builder_concrete::~logger_builder_concrete() noexcept
{
    this->trace_with_guard("logger_builder_concrete instance destructed.");
}

logger_builder *logger_builder_concrete::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    if (stream_file_path.empty())
    {
        throw std::invalid_argument("file path can't be empty");
    }

    _streams_collected_information[stream_file_path] = severity;

    return this;
}

logger_builder *logger_builder_concrete::add_console_stream(
    logger::severity severity)
{
    _streams_collected_information[{}] = severity;

    return this;
}

logger_builder* logger_builder_concrete::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    std::ifstream configuration_file_stream(configuration_file_path);
    if (!configuration_file_stream.is_open())
    {
        throw std::runtime_error(std::string("File \"") + configuration_file_path + "\" can't be opened.");
    }

    auto json_full_configuration = nlohmann::json::parse(configuration_file_stream);
    auto json_target_configuration = json_full_configuration.at(configuration_path);
    for (auto const &json_logger_configuration_part : json_target_configuration.at("paths"))
    {
        add_file_stream(json_logger_configuration_part.at("path"), string_to_severity(json_logger_configuration_part.at("severity")));
    }

    return this;
}

logger_builder *logger_builder_concrete::clear()
{
    _streams_collected_information.clear();

    return this;
}

logger *logger_builder_concrete::build() const
{
    return new logger_concrete(_streams_collected_information);
}

logger *logger_builder_concrete::get_logger() const noexcept
{
    return _logger;
}
