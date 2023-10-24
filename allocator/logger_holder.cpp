#include "logger_holder.h"

logger_holder const *logger_holder::log_with_guard(
    std::string const &message,
    logger::severity severity) const
{
    auto *got_logger = get_logger();
    if (got_logger != nullptr)
    {
        got_logger->log(message, severity);
    }

    return this;
}

logger_holder const *logger_holder::trace_with_guard(
    std::string const &message) const
{
    return log_with_guard(message, logger::severity::trace);
}

logger_holder const *logger_holder::debug_with_guard(
    std::string const &message) const
{
    return log_with_guard(message, logger::severity::debug);
}

logger_holder const *logger_holder::information_with_guard(
    std::string const &message) const
{
    return log_with_guard(message, logger::severity::information);
}

logger_holder const *logger_holder::warning_with_guard(
    std::string const &message) const
{
    return log_with_guard(message, logger::severity::warning);
}

logger_holder const *logger_holder::error_with_guard(
    std::string const &message) const
{
    return log_with_guard(message, logger::severity::error);
}

logger_holder const *logger_holder::critical_with_guard(
    std::string const &message) const
{
    return log_with_guard(message, logger::severity::critical);
}