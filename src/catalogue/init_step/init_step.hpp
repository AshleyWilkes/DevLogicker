#pragma once

#include "core/item/topology/rows_cols.hpp"
#include "core/item/topology/square.hpp"
#include "core/operation/transform/domain_1_to_n.hpp"
#include "core/operation/transform/domain_any_value.hpp"
#include "core/operation/transform/identity.hpp"
#include "grid_builder/init_step.hpp"

namespace logicker::catalogue::init {

//toto je provizorne umistena operace, kterou bych rad pouzival pro nacitani velmi mnoha uloh
//Domain by, zda se, mohl byt template template parametr, protoze to vypada, ze by se tim
//  usetrilo opakovani ValueT v deklaracich
//ma znamenat cti hodnoty typu ValueT z domeny Domain
//tento zapis neni dostacujici, takze proted znamena operace na vypocet hodnot,
//  kterou neumim poradne zapsat. Az bude seznam uloh uplnejsi, projdu je a vypracuju
//  lepsi syntaxi.
//zapis se pouzive ke cteni dat pro proper (non-single-value) mappingy; chybi v nem avsak
//  informace o typu Key te mapy, nemluve o tom, kde hodnoty Key brat, pokud nejsou primo
//  soucasti vstupu, coz jak vime nemuseji byt.
template<typename ValueT, typename Domain>
class InputValue {};

//toto jsou zarodky initstepu!

template<typename ValueT>
using AvailableValuesFromSize =
  grid_builder::InitStep<
    mapping_id::AvailableValues<ValueT>,
    core::operation::transform::Domain1ToN<ValueT>,
    mapping_id::Size>;

char valueStr[] = "value";
template<typename ValueT, auto inputField = valueStr>
using Centers =
  grid_builder::InitStep<
    mapping_id::Centers<ValueT>,
    InputValue<ValueT, mapping_id::AvailableValues<ValueT>>,
    grid_builder::InputField<inputField>>;

char dominoesStr[] = "dominoes";
template<auto inputField = dominoesStr>
using Dominoes =
  grid_builder::InitStep<
    mapping_id::Regions,
    InputValue<int, int>,//tohle je nesmysl, ale to je ted cely pristup k nacitani mapujicich mappingu
    grid_builder::InputField<inputField>>;

char heightStr[] = "height";
using Height =
  grid_builder::InitStep<
    mapping_id::Height,
    core::operation::transform::Identity<int>,
    grid_builder::InputField<heightStr>>;

using Rectangle =
  grid_builder::InitStep<
    mapping_id::Rectangle,
    core::item::topology::Rectangle,
    mapping_id::Width,
    mapping_id::Height>;

char regionsStr[] = "regions";
template<auto inputField = regionsStr>
using Regions =
  grid_builder::InitStep<
    mapping_id::Regions,
    InputValue<core::item::topology::RegionId, core::operation::transform::DomainAnyValue<core::item::topology::RegionId>>,
    grid_builder::InputField<inputField>>;

template<typename ValueT, auto inputField = valueStr>
using RegionValues =
  grid_builder::InitStep<
    mapping_id::RegionValues<ValueT>,
    InputValue<ValueT, mapping_id::AvailableValues<ValueT>>,//Available Values je tady spatne, bude fungovat jenom kdyz nepotrebuju ruzny availably pro regiony a pro jinou oblast; tato vlastnost nacitani si zaslouzi podrobnou uvahu, na kterou ted nemam cas
    grid_builder::InputField<inputField>>;

using RowsCols =
  grid_builder::InitStep<
    mapping_id::Regions,
    core::item::topology::RowsCols,
    mapping_id::Square>;

template<typename Type, auto id, auto inputField = id>
using Scalar =
  grid_builder::InitStep<
    mapping_id::Scalar<id, Type>,
    core::operation::transform::Identity<Type>,
    grid_builder::InputField<inputField>>;

char sizeStr[] = "size";
using Size =
  grid_builder::InitStep<
    mapping_id::Size,
    core::operation::transform::Identity<int>,
    grid_builder::InputField<sizeStr>>;

using Square =
  grid_builder::InitStep<
    mapping_id::Square,
    core::item::topology::Square,
    mapping_id::Size>;

char uDirectionStr[] = "directionClues";
template<typename ClueType, auto inputField = uDirectionStr>
using UDirectionClues =
  grid_builder::InitStep<
    mapping_id::UDirectionClues<ClueType>,
    InputValue<int, int>,//jde jen o to, projit kompilaci
    grid_builder::InputField<inputField>>;

char widthStr[] = "width";
using Width = 
  grid_builder::InitStep<
    mapping_id::Width,
    core::operation::transform::Identity<int>,
    grid_builder::InputField<widthStr>>;
}
