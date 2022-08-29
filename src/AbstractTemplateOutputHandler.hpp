// Copyright (c) embedded ocean GmbH
#pragma once

#include <xentara/data/DataType.hpp>
#include <xentara/data/ReadHandle.hpp>
#include <xentara/data/WriteHandle.hpp>
#include <xentara/model/Attribute.hpp>
#include <xentara/process/Event.hpp>

#include <string_view>
#include <memory>
#include <chrono>
#include <optional>

// TODO: rename namespace
namespace xentara::plugins::templateDriver
{

using namespace std::literals;

// Base class for data type specific functionality for TemplateOutput.
// TODO: rename this class to something more descriptive
class AbstractTemplateOutputHandler
{
public:
	// Abstract classes need a virtual destructor
	// Note: This destructor is also abstract, which is not really necessary, but ensures that the class is abstract even
	// if all other abstract functions are removed. 
	virtual ~AbstractTemplateOutputHandler() = 0;

	// Returns the data type
	virtual auto dataType() const -> const data::DataType & = 0;

	// Resolves an attribute that belong to this handler.
	virtual auto resolveAttribute(std::u16string_view name) -> const model::Attribute * = 0;

	// Resolves an event.
	// Note: This function uses the aliasing constructor of std::shared_ptr, which will cause the returned pointer to the control block of the parent.
	// This is why the parent pointer is passed along.
	virtual auto resolveEvent(std::u16string_view name, std::shared_ptr<void> parent) -> std::shared_ptr<process::Event> = 0;

	// Createas a read-handle for an attribute that belong to this handler.
	virtual auto readHandle(const model::Attribute &attribute) const noexcept -> std::optional<data::ReadHandle> = 0;

	// Createas a wrate-handle for an attribute that belong to this handler.
	// Note: This function uses the aliasing constructor of std::shared_ptr, which will cause the returned pointer to the control block of the parent.
	// This is why the parent pointer is passed along.
	virtual auto writeHandle(const model::Attribute &attribute, std::shared_ptr<void> parent) noexcept -> std::optional<data::WriteHandle> = 0;

	// Realizes the handler
	virtual auto realize() -> void = 0;
		
	// Attempts to read the data from the I/O component and updates the handler accordingly.
	virtual auto read(std::chrono::system_clock::time_point timeStamp) -> void = 0;
	// Attempts to write any pending value to the I/O component and updates the state accordingly.
	virtual auto write(std::chrono::system_clock::time_point timeStamp) -> void = 0;	

	// Invalidates any read data
	virtual auto invalidateData(std::chrono::system_clock::time_point timeStamp) -> void = 0;
};

inline AbstractTemplateOutputHandler::~AbstractTemplateOutputHandler() = default;

} // namespace xentara::plugins::templateDriver