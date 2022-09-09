// Copyright (c) embedded ocean GmbH
#include "TemplateOutput.hpp"

#include "AbstractTemplateOutputHandler.hpp"
#include "TemplateOutputHandler.hpp"

#include <xentara/process/ExecutionContext.hpp>
#include <xentara/utils/json/decoder/Object.hpp>
#include <xentara/utils/json/decoder/Errors.hpp>

namespace xentara::plugins::templateDriver
{
	
using namespace std::literals;

TemplateOutput::Class TemplateOutput::Class::_instance;

auto TemplateOutput::loadConfig(const ConfigIntializer &initializer,
		utils::json::decoder::Object &jsonObject,
		config::Resolver &resolver,
		const FallbackConfigHandler &fallbackHandler) -> void
{
	// Get a reference that allows us to modify our own config attributes
    auto &&configAttributes = initializer[Class::instance().configHandle()];

	// Go through all the members of the JSON object that represents this object
	for (auto && [name, value] : jsonObject)
    {
		if (name == u8"dataType"sv)
		{
			// Create the handler
			_handler = createHandler(value);
		}
		/// @todo load custom configuration parameters
		else if (name == u8"TODO"sv)
		{
			/// @todo parse the value correctly
			auto todo = value.asNumber<std::uint64_t>();

			/// @todo check that the value is valid
			if (!"TODO")
			{
				/// @todo use an error message that tells the user exactly what is wrong
				utils::json::decoder::throwWithLocation(value, std::runtime_error("TODO is wrong with TODO parameter of template output"));
			}

			/// @todo set the appropriate member variables, and update configAttributes accordingly (if necessary) 
		}
		else
		{
			// Pass any unknown parameters on to the fallback handler, which will load the built-in parameters ("id" and "uuid"),
			// and throw an exception if the key is unknown
            fallbackHandler(name, value);
		}
    }

	// Make sure that a data type was specified
	if (!_handler)
	{
		/// @todo replace "template output" with a more descriptive name
		utils::json::decoder::throwWithLocation(jsonObject, std::runtime_error("Missing data type in template output"));
	}
	/// @todo perform consistency and completeness checks
	if (!"TODO")
	{
		/// @todo use an error message that tells the user exactly what is wrong
		utils::json::decoder::throwWithLocation(jsonObject, std::runtime_error("TODO is wrong with template output"));
	}
}

auto TemplateOutput::createHandler(utils::json::decoder::Value &value) -> std::unique_ptr<AbstractTemplateOutputHandler>
{
	// Get the keyword from the value
	auto keyword = value.asString<std::u8string>();
	
	/// @todo use keywords that are appropriate to the I/O component
	if (keyword == u8"bool"sv)
	{
		return std::make_unique<TemplateOutputHandler<bool>>();
	}
	else if (keyword == u8"uint8"sv)
	{
		return std::make_unique<TemplateOutputHandler<std::uint8_t>>();
	}
	else if (keyword == u8"uint16"sv)
	{
		return std::make_unique<TemplateOutputHandler<std::uint16_t>>();
	}
	else if (keyword == u8"uint32"sv)
	{
		return std::make_unique<TemplateOutputHandler<std::uint32_t>>();
	}
	else if (keyword == u8"uint64"sv)
	{
		return std::make_unique<TemplateOutputHandler<std::uint64_t>>();
	}
	else if (keyword == u8"int8"sv)
	{
		return std::make_unique<TemplateOutputHandler<std::int8_t>>();
	}
	else if (keyword == u8"int16"sv)
	{
		return std::make_unique<TemplateOutputHandler<std::int16_t>>();
	}
	else if (keyword == u8"int32"sv)
	{
		return std::make_unique<TemplateOutputHandler<std::int32_t>>();
	}
	else if (keyword == u8"int64"sv)
	{
		return std::make_unique<TemplateOutputHandler<std::int64_t>>();
	}
	else if (keyword == u8"float32"sv)
	{
		return std::make_unique<TemplateOutputHandler<float>>();
	}
	else if (keyword == u8"float64"sv)
	{
		return std::make_unique<TemplateOutputHandler<double>>();
	}

	// The keyword is not known
	else
	{
		/// @todo replace "template output" with a more descriptive name
		utils::json::decoder::throwWithLocation(value, std::runtime_error("unknown data type in template output"));
	}

	return std::unique_ptr<AbstractTemplateOutputHandler>();
}

auto TemplateOutput::performReadTask(const process::ExecutionContext &context) -> void
{
	// tasks must not be executed before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: \"read\" task of xentara::plugins::templateDriver::TemplateOutput executed before configuration has been loaded");
	}

	// Ask the handler to read the data
	_handler->read(context.scheduledTime());
}

auto TemplateOutput::performWriteTask(const process::ExecutionContext &context) -> void
{
	// tasks must not be executed before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: \"write\" task of xentara::plugins::templateDriver::TemplateOutput executed before configuration has been loaded");
	}

	// Ask the handler to write the data
	_handler->write(context.scheduledTime());
}

auto TemplateOutput::invalidateData(std::chrono::system_clock::time_point timeStamp) -> void
{
	// tasks must not be executed before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: \"read\" task of xentara::plugins::templateDriver::TemplateOutput executed before configuration has been loaded");
	}

	// Forward the request to the handler
	_handler->invalidateData(timeStamp);
}

auto TemplateOutput::dataType() const -> const data::DataType &
{
	// dataType() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: xentara::plugins::templateDriver::TemplateOutput::dataType() called before configuration has been loaded");
	}

	// Forward the request to the handler
	return _handler->dataType();
}

auto TemplateOutput::directions() const -> io::Directions
{
	return io::Direction::Input | io::Direction::Output;
}

auto TemplateOutput::resolveAttribute(std::u16string_view name) -> const model::Attribute *
{
	// resolveAttribute() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: xentara::plugins::templateDriver::TemplateOutput::resolveAttribute() called before configuration has been loaded");
	}

	// Check the handler attributes
	if (auto attribute = _handler->resolveAttribute(name))
	{
		return attribute;
	}

	/// @todo add any additional attributes this class supports, including attributes inherited from the I/O component

	return nullptr;
}

auto TemplateOutput::resolveTask(std::u16string_view name) -> std::shared_ptr<process::Task>
{
	if (name == u"read"sv)
	{
		return std::shared_ptr<process::Task>(sharedFromThis(), &_readTask);
	}
	else if (name == u"write"sv)
	{
		return std::shared_ptr<process::Task>(sharedFromThis(), &_writeTask);
	}

	/// @todo add any additional tasks this class supports

	return nullptr;
}

auto TemplateOutput::resolveEvent(std::u16string_view name) -> std::shared_ptr<process::Event>
{
	// resolveAttribute() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: xentara::plugins::templateDriver::TemplateOutput::resolveEvent() called before configuration has been loaded");
	}

	// Check the handler events
	if (auto event = _handler->resolveEvent(name, sharedFromThis()))
	{
		return event;
	}

	/// @todo add any additional events this class supports, including events inherited from the I/O component

	return nullptr;
}

auto TemplateOutput::readHandle(const model::Attribute &attribute) const noexcept -> data::ReadHandle
{
	// readHandle() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		// Don't throw an exception, because this function is noexcept
		return std::make_error_code(std::errc::invalid_argument);
	}

	// Check the handler attributes
	if (auto handle = _handler->readHandle(attribute))
	{
		return *handle;
	}

	/// @todo add any additional readable attributes this class supports, including attributes inherited from the I/O component

	return data::ReadHandle::Error::Unknown;
}

auto TemplateOutput::writeHandle(const model::Attribute &attribute) noexcept -> data::WriteHandle
{
	// writeHandle() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		// Don't throw an exception, because this function is noexcept
		return std::make_error_code(std::errc::invalid_argument);
	}

	// Check the handler attributes
	if (auto handle = _handler->writeHandle(attribute, sharedFromThis()))
	{
		return *handle;
	}

	/// @todo add any additional writable attributes this class supports, including attributes inherited from the I/O component

	return data::WriteHandle::Error::Unknown;
}

auto TemplateOutput::realize() -> void
{
	// realize() must not be called before the configuration was loaded, so the handler should have been
	// created already.
	if (!_handler) [[unlikely]]
	{
		throw std::logic_error("internal error: xentara::plugins::templateDriver::TemplateOutput::realize() called before configuration has been loaded");
	}

	// Realize the handler
	_handler->realize();
}

} // namespace xentara::plugins::templateDriver