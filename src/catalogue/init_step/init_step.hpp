#pragma once

#include "core/item/topology/rows_cols.hpp"
#include "core/item/topology/square.hpp"
#include "core/operation/transform/identity.hpp"
#include "core/operation/transform/domain_1_to_n.hpp"
#include "grid_builder/init_step.hpp"

namespace logicker::catalogue::init {

//toto je provizorne umistena operace, kterou bych rad pouzival pro nacitani velmi mnoha uloh
template<typename ValueT, typename Domain>
class InputValue {};

//toto jsou zarodky initstepu!

template<typename ValueT>
using AvailableValuesFromSize = grid_builder::InitStep<mapping_id::AvailableValues<ValueT>, core::operation::transform::Domain1ToN<ValueT>, mapping_id::Size>;

char valueStr[] = "value";
template<typename ValueT, auto inputField = valueStr>
using Centers = grid_builder::InitStep<mapping_id::Centers<ValueT>, InputValue<ValueT, mapping_id::AvailableValues<ValueT>>, grid_builder::InputField<inputField>>;

using RowsCols = grid_builder::InitStep<mapping_id::Regions, core::item::topology::RowsCols, mapping_id::Square>;

char sizeStr[] = "size";
using Size = grid_builder::InitStep<mapping_id::Size, core::operation::transform::Identity<int>, grid_builder::InputField<sizeStr>>;

using Square = grid_builder::InitStep<mapping_id::Square, core::item::topology::Square, mapping_id::Size>;

}
