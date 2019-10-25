#pragma once

namespace logicker_old::puzzle {

class Elements {
  public:
    //Mappings
    using HeightM = Mapping<"height", int>;
    using WidthM = Mapping<"width", int>;
    using RegionsM = Mapping<"region", Rectangle::Center, Region>;
    using ValueM = Mapping<"value", Region, ElementT>;

    using GridMappings = std::tuple<HeightM, WidthM, RegionsM, ValueM>;

    //Helpers
    using NeighborCentersM = Mapping<"neighbor_centers", Set<Pair<Rectangle::Center>>>;
    using NeighborCentersH = Helper<NeighborCentersM, Rectangle::PairCoords<Rectangle::UAdjacent>>;

    using GridHelpers = std::tuple<NeighborCentersH>;

    //Constraints
    using NeighborsPairP = Parameter<"neighbors_pair_coords_param", PairCoords<Rectangle::Center>>;
    using CoordsValue = Extracted<ValueM, Extracted<RegionsM>>;
    using NeighborValues = PairValues<Extracted<NeighborsPairP>, CoordsValue>;

    using Constraints = AllOf<
      Extracted<NeighborCentersM>, NeighborsPairP,
      ValuesDiffer<NeighborValues>>;
};

}


//!!nasleduje (zmateny) popis problemu, ale napred je zde reseni!!
//!!ale ja proste muzu chtit, aby se mi operace chovala jako strom s danym poradim uzlu, a aby neuplne specifikovana slozita operace poslouchala stejnej interface jako zcela nespecifikovana jednoducha operace!!
//!!to bylo to reseni, ted ten popis!!
//
//extractory se vyskytuji v ruznych formach:
//1) extractor mappingu bez parametru; vzdy uveden se vsemi parametry, ehm
//2) extractor mappingu s 1 parametrem; uvedeny i se svym parametrem
//3) extractor mappingu s 1 parametrem; uvedeny bez sveho parametru
//
//totez se, zda se, tyka Operations, a zdehle debata o extractorech je jen specialni pripad
//uplne stejneho problemu s Operations; u Operations avsak neni pochyb o tom, ze muzou mit
//vice nez jeden vstup??
//
//jde teda o ruzne formy zapisu toho, jak se ma Operation pouzit ke konverzi Inputs->Outputs
//kdyz vyloucim moznost, ze bych chtel zapsat napul-bindovanou operaci (napr. mela by 2 parametry
//a jeden bych nastavil a druhy ne), zbyvaji jen ty dve moznosti: zadny nebo vsechny
//
//pokud neni parametr stanoven zadny, pak mam efektivne misto operace pouze jeji Id, jedna
//se o jeji dobre specifikovanou templatu; nazev stale musi byt plne specifikovan
//
//pokud jsou parametry specifikovany vsechny, mam cele volani
//
//zkusme prozatim cestu, kde extractor je validni, pokud obsahuje jako prvni parametr svuj mapping, a pokud pripadne nasledujici parametry odpovidaji operacim, vracejicim hodnoty jednotlivych operandu extractoru ve spravnem poradi; takto zadane operandy nemuseji byt vsechny
//
//tento plan, zda se, je funckni, pouze mozna narazi na technicke problemy s C++ sablonami
//
//a jeste je jeden problem, a to, kdyz chci specifikovat extractor, jehoz parametrem je jiny extractor, a je to tento jiny extractor, ktery nema zadany parametr
