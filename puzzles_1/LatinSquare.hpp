#pragma once

namespace logicker_old::puzzle {

template<typename ValueT = IntItem, typename ValueD /*??= IntDomainMinMax<1, ExtractedValue<"size">>*/
class LatinSquare {
  public:
    //Mappings
    using SizeM = Mapping<"size", int>;
    using ValueM = Mapping<"value", Square::Center, ValueT>;

    using GridMappings = std::tuple<SizeM, ValueM>;

    //Helpers
    using AvailValuesM = Mapping<"avail_vals", Set<ValueT>>;
    using AvailValuesH = Helper<AvailValuesM, IntDomainMinMax<1, Extracted<SizeM>>>;

    using RegionCoordsM = Mapping<"region_coords", Set<Set<Square::Center>>>;
    using RegionCoordsH = Helper<RegionCoordsH, Square::RegionCoords<Square::URows, Square::UCols>>;//U stands for unordered
    
    using GridHelpers = std::tuple<AvailValuesH, RegionCoordsH>;

    //Constraints
    using RegionCoordsP = Parameter<"region_coords_param", Set<Square::Center>>;
    using FieldCoordsP = Parameter<"field_coords_param", Square::Center>;

    using RegionValuesSet = SetValues<Extracted<RegionCoordsP>, FieldCoordsP, Extracted<ValueM, Extracted<FieldCoordsP>>>;

    using Constraints = AllOf<
      Extracted<RegionCoordsM>, RegionCoordsP,
      SetsEqual<RegionValuesSet, Extracted<AvailValuesM>>;

};

}
