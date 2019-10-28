#pragma once

namespace logicker::puzzle {

template<typename ValueT = IntItem>
class LatinSquare {
  public:
    using GridMappings = std::tuple<
      catalogue::mapping::Size,//Mapping<"size", int>
      catalogue::mapping::Square,//Mapping<"topology", Topology> (for internal use when constructing the Grid -- cili to by tady nemuselo byt, musi to pouze byt soucasti Recipe, pokud chci topologii pouzit pri konstrukci centers a regions (coz asi budu chtit:-) )
      catalogue::mapping::Centers<ValueT>,//Mapping<"value", Coords->ValueT>
      catalogue::mapping::AvailableValues<ValueT>,//Mapping<"available_values", Set<ValueT>>
      catalogue::mapping::Regions//Mapping<"regions", Set<Region>>
    >;

    using GridRecipe = std::tuple<
      catalogue::init::integer::Size,//mapping::Size <= input.get("size")
      catalogue::init::topology::Square,//mapping::Topology <= mapping::Size (via operation::Square(int))

      //tady ted ma bejt krok, kterej rika:
      //-- vyplnujeme 2d mapping (vsechny ostatni zde jsou konstanty, ne funkce), potrebujeme std::map
      //-- pokud mam na vstupu stream of ValueT, prochazej zaroven Topology::Centers a delej dvojice
      //-- pokud mam na vstupu array of [coords, value] pairs, cti a konvertuj
      //-- pokud vyplnuju ReadOnlyMapping, vyzaduj kompletni pokryti Topology::Centers
      //-- pokud vyplnuju ReadWriteMapping, dopln nepritomne Centers defaultni Domainou
      //
      //ergo misi se nam zde veci, ktere se u ostatnich InitStepu v tomto Puzzlu neprojevily
      //a ktere jsou na sebe ortogonalni:
      //-- rozhodovani podle formatu vstupu
      //-- rozhodovani podle typu vystupu (vyplnovaneho MappingTypu)
      //
      //idealne chci psat pouze implementacni metody, ktere transformuji specificke vstupy
      //na vystupy; pouze jednou chci napsat kod, ktery nacita vstupy a na zaklade jejich
      //vlastnosti vybira a vola spravnou implementacni metodu
      //
      //??cili jadro pudla je v tom "vybira" spravnou implementacni metodu??
      //implementace musi obsahovat informaci o pozadavcich na jednotlive inputy
      //vsechny implementace maji mit jednotny interface, jehoz prostrednictvim s nimi
      //GridBuilder pracuje
      //
      //mozna bych umel vymyslet komplikovanejsi verzi, ale pro zacatek predpokladejme,
      //ze kazda implementace ma prave tolik vstupnich parametru jako apply() prislusneho InitStepu
      //a ze check pouzitelnosti implementace a konverze vstupu probiha 1:1 a obnasi tolik co zjistit,
      //zda urcity vstup InitStepu lze zkonvertovat na pozadovany vstup implementace. Pokud tento
      //postup projde pro vsechny parametry, implementace se zavola a jeji vysledek se pouzije.
      //
      //Takze implementace necht obsahuje std::tuple s typama, ktere ocekava, a mappingy
      //a inputy necht obsahuji metody contains<type> a get<type>
      catalogue::init::mapping::Centers<ValueT, 
              catalogue::init::transform::InputOrDomain>,
      catalogue::init::AvailableValuesFromSize<ValueT>,//mapping::AvailableValues <= 1..mapping::Size (via operation::ValueT(int))

      catalogue::init::regions::RowsCols//mapping::Regions <= mapping::Topology (via operation::RowsCols(Topology))
    >;
};

}
