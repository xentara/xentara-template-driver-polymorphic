// Copyright (c) embedded ocean GmbH
#pragma once

#include "ReadTask.hpp"
#include "AbstractTemplateInputHandler.hpp"

#include <xentara/io/Io.hpp>
#include <xentara/io/IoClass.hpp>
#include <xentara/plugin/EnableSharedFromThis.hpp>
#include <xentara/process/Task.hpp>
#include <xentara/utils/json/decoder/Value.hpp>

#include <functional>
#include <string_view>

// TODO: rename namespace
namespace xentara::plugins::templateDriver
{

using namespace std::literals;

class TemplateIoComponent;

// A class representing a specific type of input.
// TODO: rename this class to something more descriptive
class TemplateInput final : public io::Io, public plugin::EnableSharedFromThis<TemplateInput>
{
private:
	// A structure used to store the class specific attributes within an element's configuration
	struct Config final
	{
		// TODO: Add custom config attributes
	};
	
public:
	// The class object containing meta-information about this element type
	class Class final : public io::IoClass
	{
	public:
		// Gets the global object
		static auto instance() -> Class&
		{
			return _instance;
		}

	    // Returns the array handle for the class specific attributes within an element's configuration
	    auto configHandle() const -> const auto &
        {
            return _configHandle;
        }

		auto name() const -> std::u16string_view final
		{
			// TODO: change class name
			return u"TemplateInput"sv;
		}
	
		auto uuid() const -> utils::core::Uuid final
		{
			// TODO: assign a unique UUID
			return "cccccccc-cccc-cccc-cccc-cccccccccccc"_uuid;
		}

	private:
	    // The array handle for the class specific attributes within an element's configuration
		memory::Array::ObjectHandle<Config> _configHandle { config().appendObject<Config>() };

		// The global object that represents the class
		static Class _instance;
	};

	// This constructor attaches the input to its I/O component
	TemplateInput(std::reference_wrapper<TemplateIoComponent> ioComponent) :
		_ioComponent(ioComponent)
	{
	}

	auto dataType() const -> const data::DataType &;

	auto directions() const -> io::Directions;

	auto resolveAttribute(std::u16string_view name) -> const model::Attribute * final;
	
	auto resolveTask(std::u16string_view name) -> std::shared_ptr<process::Task> final;

	auto resolveEvent(std::u16string_view name) -> std::shared_ptr<process::Event> final;

	auto readHandle(const model::Attribute &attribute) const noexcept -> data::ReadHandle final;

	auto realize() -> void final;

protected:
	auto loadConfig(const ConfigIntializer &initializer,
		utils::json::decoder::Object &jsonObject,
		config::Resolver &resolver,
		const FallbackConfigHandler &fallbackHandler) -> void final;

private:
	// The read task needs access to out private member functions
	friend class ReadTask<TemplateInput>;

	// Creates an input handler based on a configuration value
	auto createHandler(utils::json::decoder::Value &value) -> std::unique_ptr<AbstractTemplateInputHandler>;

	// This function is called by the "read" task. It attempts to read the value if the I/O component is up.
	auto performReadTask(const process::ExecutionContext &context) -> void;
	// Invalidates any read data
	auto invalidateData(std::chrono::system_clock::time_point timeStamp) -> void;

	// The I/O component this input belongs to
	// TODO: give this a more descriptive name, e.g. "_device"
	std::reference_wrapper<TemplateIoComponent> _ioComponent;

	// The handler for data type specific functionality, or nullptr, if the data type hans not been loaded yet
	std::unique_ptr<AbstractTemplateInputHandler> _handler;

	// The "read" task
	ReadTask<TemplateInput> _readTask { *this };
};

} // namespace xentara::plugins::templateDriver