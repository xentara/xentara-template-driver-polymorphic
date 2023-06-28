// Copyright (c) embedded ocean GmbH
#pragma once

#include "Attributes.hpp"
#include "CustomError.hpp"

#include <xentara/data/Quality.hpp>
#include <xentara/data/ReadHandle.hpp>
#include <xentara/memory/ObjectBlock.hpp>
#include <xentara/model/ForEachAttributeFunction.hpp>
#include <xentara/model/ForEachEventFunction.hpp>
#include <xentara/process/Event.hpp>
#include <xentara/utils/eh/expected.hpp>

#include <chrono>
#include <concepts>
#include <optional>
#include <memory>

namespace xentara::plugins::templateDriver
{

/// @brief State information for a read operation.
template <std::regular DataType>
class ReadState final
{
public:
	/// @brief Iterates over all the attributes that belong to this state.
	/// @param function The function that should be called for each attribute
	/// @return The return value of the last function call
	auto forEachAttribute(const model::ForEachAttributeFunction &function) const -> bool;

	/// @brief Iterates over all the events that belong to this state.
	/// @param function The function that should be called for each events
	/// @param parent
	/// @parblock
	/// A shared pointer to the containing object.
	/// 
	/// The pointer is used in the aliasing constructor of std::shared_ptr when constructing the event pointers,
	/// so that they will share ownership information with pointers to the parent object.
	/// @endparblock
	/// @return The return value of the last function call
	auto forEachEvent(const model::ForEachEventFunction &function, std::shared_ptr<void> parent) -> bool;

	/// @brief Creates a read-handle for an attribute that belong to this state.
	/// @note The value attribute is not handled, it must be gotten separately using valueReadHandle().
	/// @param attribute The attribute to create the handle for
	/// @return A read handle for the attribute, or std::nullopt if the attribute is unknown (including the value attribute)
	auto makeReadHandle(const model::Attribute &attribute) const noexcept -> std::optional<data::ReadHandle>;

	/// @brief Creates a read-handle for the value attribute
	/// @return A read handle to the value attribute
	auto valueReadHandle() const noexcept -> data::ReadHandle;

	/// @brief Realizes the state
	auto realize() -> void;

	/// @brief Updates the data and sends events
	/// @param timeStamp The update time stamp
	/// @param valueOrError This is a variant-like type that will hold either the new value, or an std::error_code object
	/// containing an read error
	auto update(std::chrono::system_clock::time_point timeStamp, const utils::eh::expected<DataType, std::error_code> &valueOrError) -> void;

private:
	/// @brief This structure is used to represent the state inside the memory block
	struct State final
	{
		/// @brief The update time stamp
		std::chrono::system_clock::time_point _updateTime { std::chrono::system_clock::time_point::min() };
		/// @brief The current value
		DataType _value {};
		/// @brief The change time stamp
		std::chrono::system_clock::time_point _changeTime { std::chrono::system_clock::time_point::min() };
		/// @brief The quality of the value
		data::Quality _quality { data::Quality::Bad };
		/// @brief The error code when reading the value, or a default constructed std::error_code object for none.
		std::error_code _error { CustomError::NoData };
	};

	/// @brief A summary event that is fired when anything changes
	process::Event _changedEvent { io::Direction::Input };

	/// @brief The data block that contains the state
	memory::ObjectBlock<State> _dataBlock;
};

/// @class xentara::plugins::templateDriver::ReadState
/// @todo change list of extern template statements to the supported types
extern template class ReadState<bool>;
extern template class ReadState<std::uint8_t>;
extern template class ReadState<std::uint16_t>;
extern template class ReadState<std::uint32_t>;
extern template class ReadState<std::uint64_t>;
extern template class ReadState<std::int8_t>;
extern template class ReadState<std::int16_t>;
extern template class ReadState<std::int32_t>;
extern template class ReadState<std::int64_t>;
extern template class ReadState<float>;
extern template class ReadState<double>;
extern template class ReadState<std::string>;

} // namespace xentara::plugins::templateDriver