#pragma once

namespace logicker_old::puzzle {

class Magnets {
  public:
    //Mappings
    using HeightM = Mapping<"height", int>;
    using WidthM = Mapping<"width", int>;
    using DominoesM = Mapping<"dominoes", Rectangle::Center, Domino>;//Domino necht je par sousednich poli (centers souradnic)
    using GivensPlusM = Mapping<"givens_plus", Rectangle::RegionId, int>;
    using GivensMinusM = Mapping<"givens_minus", Rectangle::RegionId, int>;
    using ValuesM = Mapping<"values", Rectangle::Center, MagnetsT>;

    using GridMappings = std::tuple<HeightM, WidthM, DominoesM, DominoCoordsM, GivensM>;
    //Helpers
    using NeighborCentersM = Mapping<"neighbor_centers", Set<UPair<Rectangle::Center>>>;
    using NeighborCentersH = Helper<NeighborCentersM, Rectangle::UPairCoords<Rectangle::UAdjacent>>;

    using GridHelpers = std::tuple<NeighborCentersH, DominoesH>;

    //Constraints
    using CoordsValue = Extracted<ValueM, Extracted<RegionsM>>;

    //v sousednich polich nesmeji byt dva plusy/minusy
    using NeighborsUPairP = Parameter<"neighbors_pair_coords_param", UPair<Rectangle::Center>>;
    using NeighborValues = UPairValues<Extracted<NeighborsUPairP>, CoordsValue>;

    using NeighborValuesPredicate = Or<
      ValuesDiffer<NeighborValues>,
      ValuesEqual<NeighborValues::First, MagnetsT::Isolator>>;

    using NeighborsConstraint = AllOf<
      Extracted<NeighborCentersM>, NeighborsUPairP,
      NeighborValuesPredicate>;

    //v ramci domina musi byt plus/minus anebo dva izolanty
    using DominoP = Parameter<"domino_param", UPair<Rectangle::Center>>;
    using DominoValues = UPairValues<Extracted<DominoP>, CoordsValue>;

    using DominoPredicate = Or<
      UPairValuesEqual<DominoValues, UPairValues<MagnetsT::Isolator, MagnetsT::Isolator>>,
      UPairValuesEqual<DominoValues, UPairValues<MagnetsT::Plus, MagnetsT::Minus>>>;

    using DominoConstraint = AllOf<
      ExtractedInv<DominoesM>, DominoP,
      DominoPredicate>;

    //v radcich a sloupcich musi byt spravny pocet plusu/minusu, je-li zadan
    using PlusCountConstraint = AllOf<
      
};

}
