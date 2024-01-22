// Copyright (c) embedded ocean GmbH
#include "TemplateInput.hpp"

#include "AbstractTemplateInputHandler.hpp"
#include "Tasks.hpp"
#include "TemplateInputHandler.hpp"

#include <xentara/config/Errors.hpp>
#include <xentara/model/Attribute.hpp>
#include <xentara/model/ForEachAttributeFunction.hpp>
#include <xentara/model/ForEachEventFunction.hpp>
#include <xentara/model/ForEachTaskFunction.hpp>
#include <xentara/process/ExecutionContext.hpp>
#include <xentara/utils/json/decoder/Object.hpp>
#include <xentara/utils/json/decoder/Errors.hpp>

namespace xentara::plugins::templateDriver
{
	
using namespace std::literals;

auto TemplateInput::load(utils::json::decoder::Object &jsonObject, config::Context &context) -> void
{
	// Go through all the members of the JSON object that represents this object
	for (auto && [name, value] : jsonObject)
    {
		if (name == "dataType"sv)
		{
			// Create the handler
			_handler = createHandler(value);
		}
		/// @todo load custom configuration parameters
		else if (name == "TODO"sv)
		{
			/// @todo parse the value correctly
			auto todo = value.asNumber<std::uint64_t>();

			/// @todo check that the value is valid
			if (!"TODO")
			{
				/// @todo use an error message that tells the user exactly what is wrong
				utils::json::decoder::throwWithLocation(value, std::runtime_error("TODO is wrong with TODO parameter of template input"));
			}

			/// @todo set the appropriate member variables
		}
		else
		{
            config::throwUnknownParameterError(name);
		}
    }

	// Make sure that a data type was specified
	if (!_handler)
	{
		/// @todo replace "template input" with a more descriptive name
		utils::json::decoder::throwWithLocation(jsonObject, std::runtime_error("Missing data type in template input"));
	}
	/// @todo perform consistency and completeness checks
	if (!"TODO")
	{
		/// @todo use an error message that tells the user exactly what is wrong
		utils::json::decoder::throwWithLocation(jsonObject, std::runtime_error("TODO is wrong with template input"));
	}
}

auto TemplateInput::createHandler(utils::json::decoder::Value &value) -> std::unique_ptr<AbstractTemplateInputHandler>
{
	// Get the keyword from the value
	auto keyword = value.asString<std::string>();
	
	/// @todo use keywords that are appropriate to the I/O component
	if (keyword == "bool"sv)
	{
		return std::make_unique<TemplateInputHandler<bool>>();
	}
	else if (keyword == "uint8"sv)
	{
		return std::make_unique<TemplateInputHandler<std::uint8_t>>();
	}
	else if (keyword == "uint16"sv)
	{
		return std::make_unique<TemplateInputHandler<std::uint16_t>>();
	}
	else if (keyword == "uint32"sv)
	{
		return std::make_unique<TemplateInputHandler<std::uint32_t>>();
	}
	else if (keyword == "uint64"sv)
	{
		return std::make_unique<TemplateInputHandler<std::uint64_t>>();
	}
	else if (keyword == "int8"sv)
	{
		return std::make_unique<TemplateInputHandler<std::int8_t>>();
	}
	else if (keyword == "int16"sv)
	{
		return std::make_unique<TemplateInputHandler<std::int16_t>>();
	}
	else if (keyword == "int32"sv)
	{
		return std::make_unique<TemplateInputHandler<std::int32_t>>();
	}
	else if (keyword == "int64"sv)
	{
		return std::make_unique<TemplateInputHandler<std::int64_t>>();
	}
	else if (keyword == "float32"sv)
	{
		return std::make_unique<TemplateInputHandler<float>>();
	}
	else if (keyword == "float64"sv)
	{
		return std::make_unique<TemplateInputHandler<double>>();
	}
	else if (keyword == "string"sv)
	{
		return std::make_unique<TemplateInputHandler<std::string>>();
	}

	// The keyword is not known
	else
	{
		/// @todo replace "template input" with a more descriptive name
		utils::json::decoder::throwWithLocation(value, std::runtime_error("unknown data type in template input"));
	}

	return std::unique_ptr<AbstractTemplateInputHandler>();
}

auto TemplateInput::performReadTask(const process::ExecutionContext &context) -> void
{
	// tasks must not be executed before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: \"read\" task of xentara::plugins::templateDriver::TemplateInput executed before configuration has been loaded");
	}

	// Ask the handler to read the data
	_handler->read(context.scheduledTime());
}

auto TemplateInput::invalidateData(std::chrono::system_clock::time_point timeStamp) -> void
{
	// tasks must not be executed before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: \"read\" task of xentara::plugins::templateDriver::TemplateInput executed before configuration has been loaded");
	}

	// Forward the request to the handler
	_handler->invalidateData(timeStamp);
}

auto TemplateInput::dataType() const -> const data::DataType &
{
	// dataType() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: xentara::plugins::templateDriver::TemplateInput::dataType() called before configuration has been loaded");
	}

	// Forward the request to the handler
	return _handler->dataType();
}

auto TemplateInput::directions() const -> io::Directions
{
	return io::Direction::Input;
}

auto TemplateInput::forEachAttribute(const model::ForEachAttributeFunction &function) const -> bool
{
	// forEachAttribute() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: xentara::plugins::templateDriver::TemplateInput::forEachAttribute() called before configuration has been loaded");
	}

	return
		// Handle the handler attributes
		_handler->forEachAttribute(function);

	/// @todo handle any additional attributes this class supports, including attributes inherited from the I/O component
}

auto TemplateInput::forEachEvent(const model::ForEachEventFunction &function) -> bool
{
	// forEachAttribute() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: xentara::plugins::templateDriver::TemplateInput::forEachEvent() called before configuration has been loaded");
	}

	return
		// Handle the handler events
		_handler->forEachEvent(function, sharedFromThis());

	/// @todo handle any additional events this class supports, including events inherited from the I/O component
}

auto TemplateInput::forEachTask(const model::ForEachTaskFunction &function) -> bool
{
	// Handle all the tasks we support
	return
		function(tasks::kRead, sharedFromThis(&_readTask));

	/// @todo handle any additional tasks this class supports
}

auto TemplateInput::makeReadHandle(const model::Attribute &attribute) const noexcept -> std::optional<data::ReadHandle>
{
	// makeReadHandle() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		// Don't throw an exception, because this function is noexcept
		return std::make_error_code(std::errc::invalid_argument);
	}

	// Handle the handler attributes
	if (auto handle = _handler->makeReadHandle(attribute))
	{
		return handle;
	}

	/// @todo handle any additional readable attributes this class supports, including attributes inherited from the I/O component

	return std::nullopt;
}

auto TemplateInput::realize() -> void
{
	// realize() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: xentara::plugins::templateDriver::TemplateInput::realize() called before configuration has been loaded");
	}

	// Realize the handler
	_handler->realize();
}

} // namespace xentara::plugins::templateDriver