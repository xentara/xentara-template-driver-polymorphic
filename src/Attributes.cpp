// Copyright (c) embedded ocean GmbH
#include "Attributes.hpp"

#include <xentara/utils/core/Uuid.hpp>
#include <xentara/data/DataType.hpp>

#include <string_view>

namespace xentara::plugins::templateDriver::attributes
{

using namespace std::literals;

const model::Attribute kError { model::Attribute::kError, model::Attribute::Access::ReadOnly, data::DataType::kErrorCode };

const model::Attribute kWriteError { model::Attribute::kWriteError, model::Attribute::Access::ReadOnly, data::DataType::kErrorCode };

} // namespace xentara::plugins::templateDriver::attributes
