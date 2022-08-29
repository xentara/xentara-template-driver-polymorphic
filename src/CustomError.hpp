// Copyright (c) embedded ocean GmbH
#pragma once

#include <system_error>
#include <type_traits>

// TODO: rename namespace
namespace xentara::plugins::templateDriver
{

// Custom error codes used by the driver
enum class CustomError
{
	// The base value for custom errors, so they don't overlap with system errors
	NoError = 0,

	// No data has been read yet.
	NoData,

	// TODO: Add more error codes here

	// An unknown error occurred
	UnknownError = 999
};

// The error category for custom errors
auto customErrorCategory() noexcept -> const std::error_category &;

// Support automatic conversion from CustomError to std::error_code
inline auto make_error_code(CustomError error) noexcept -> std::error_code
{
	return { int(error), customErrorCategory() };
}

} // namespace xentara::plugins::templateDriver

namespace std
{

// This specialization enables automatic conversion from CustomError to std::error_code.
template<>
struct is_error_code_enum<xentara::plugins::templateDriver::CustomError> : public std::true_type
{
};

}