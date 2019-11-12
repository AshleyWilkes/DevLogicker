#pragma once

namespace logicker::core::operation {

//definovana nad hodnotami, tj. treba nad dvema inty, relace pojmenovava
//nejaky vztah mezi hodnotami, o nemz lze pro dane konkretni hodnoty urcit
//bool hodnotu, zda konkretni hodnoty relaci splnuji ci-li nic
template<typename... Is>
class Relation{};

//definovana nad input hodnotami, operace pojmenovava nejaky vypocet, jimz
//lze pro konkretni hodnoty vstupu urcit jednoznacne hodnotu vystupu
//zvlastnim pripadem Operation je dvojice <Relation, int>, ktera umoznuje
//Relacim, tam kde je to mozne, vystupovat jako Operace, tj. jako zpusob
//urceni nejake hodnoty
template<typename O, typename... Is>
class Operation;

}

namespace logicker::core::operation_impl {

//functor s metodou
//typename Operation::O
//perform( typename std::tuple<const typename Operation::I>& is );
template<typename Operation, typename... Args>
class OperationImpl {
  public:
    using Is = std::tuple<Args...>;
    using Os = int;
    Os perform( const Args&... args ) {
    }
};

//mam dve sady argumentu:
//  a) Wrapu -- itemy a inputy -- specifikovany klientem pri instancializaci Wrapu
//  b) OperationImpl -- itemy only -- specifikovany by OperationImpl itself
//
//dal jsem si do predpokladu, ze pocet argumentu v a) a b) bude stejny. tj Wrapy,
//zda se, ve skutecnosti nejsou templatovany jen by Operation, ale navic jeste
//by NumberOfArguments
//
//v tom pripade bych tedy ve wrapu mel tuple aktualnich argumentu a informaci
//o tuplu pozadovanych typu. efektivni by asi bylo, kdybych se tedy pokusil zkonvertit
//tuple aktualnich na tuple pozadovanych, a v pripade uspechu konverze s tim tuple
//pozadovanych zavolal perform() implementace. Sounds goood!
//
//Jak bude vypadat Wrap nad Constraintem? Constraint je predpis, jehoz pomoci lze
//o vyplnenem Puzzlu rozhodnout, jestli je vyplnen v souladu s pravidly; v tomto
//smyslu je Constraint slozita Operation, pocitajici bool hodnotu. Takto je taky
//Constraint v definici Puzzlu (nebo kdekoli jinde) zadan. Z pohledu solvera
//ale je nutno Constraint chapat jako Relation, protoze solver nezna vsechny vyplnene
//hodnoty vysledku, ale zato vi, ze vysledek, prohnan Constraintem, ma vest na hodnotu
//true. 
//
//Mezi relacemi a operacemi existuje velmi uzky vztah: Operace je Relace, v niz byla 
//jedna promenna prohlasena za vylucnou a Operace jeji hodnotu urcuje podle pravidel 
//Relace z ostatnich promennych. Na druhou stranu Operaci lze snadno rozsirit na Relaci:
//k promennym Operace staci pridat jednu dodatecnou promennou, predstavujici vysledek
//Operace, a jednu bool promennou obsahujici informaci o tom, zda obsah te dodatecne
//promenne odpovida spravne hodnote. (Nutnost posledne zminene bool promenne je velmi
//zajimave tema. Vidim dva duvody, proc ji zavadim: 1) hodnota teto promenne je,
//chapu-li Constraint jako Operationu, vysledkem teto Operationy, je to to, co v dusledku
//probubla az nahoru a da celemu Constraintu pravdivostni hodnotu. 2) je to prostredek
//k tomu, abych mohl do ostatnich promennych Relace dat zcela libovolny obsah, a zaroven
//mel prostredek, jak Relaci uchovat v konzistentnim stavu, ktery umozni nekonzistenci
//rovnou povazovat za error, kdyz ji odhalim. No, jak rikam, zajimave a mozna ne zcela
//zpracovane). S ohledem na tuto blizkou spriznenost Operaci a Relaci prestavam rozlisovani
//mezi nimi povazovat za dulezite.
//
//Dulezite naopak je, jak bude vypadat run-time struktura Constraintu uvnitr Solvera a
//jak se tato struktura bude vytvaret.
//Kazda Relace, ktera je soucasti Constraintu, musi mit k dispozici interni sloty pro
//vsechny sve promenne; tyto musi byt navic provazany tak, aby vystupy z jedne byly
//slinkovany se vstupy druhe. Na jedne strane (uplne dole) jsou extractory, ktere
//nemaji zadne vstupy (nektere extractory maji vstupy, ty maji take svou platnost,
//pouze nejsou uplne dole); na druhe strane (uplne nahore) je jedna bool promenna,
//jejiz hodnota neni predem znama, pokud delam check, a naopak je zadana jako true,
//pokud delam solve.
//
//Run-time Constraint Wrap stavim odshora: zacinam top-level Relaci, jejiz vysledek
//ztotoznim s tou uplne nahore stojici bool promennou. Pokracuju Connective Relacemi,
//ktere vytvori stromek propojenych bool promennych, plynule pomoci Predicate Relaci
//dale propojeny s promennymi jineho typu, mezi nimi v nizsich patrech Transform operace
//tvori nejake vztahy. No tak tohle je vysoce informativni odstavec, wtz.
//
//Kazda promenna zminena v predchozim odstavci je managed value vhodneho typu, a management
//type ??muze byt asi vsude domain-based, jinymi slovy read-only mapping vubec k nicemu
//nepotrebuju??. Inu proc ne, v solveru ho asi opravdu na nic nepotrebuju, naopak v
//grid_builderu se asi hodi. Cili kdyz chci instancializovat solver nad Constraintem,
//potrebuju vedet, jaky vsechny interni mappingy budu potrebovat pouzit. Na zaklade teto
//informace vytvorim solveruv interni Grid. Pouzit i pro interni potreby solveru datovou
//strukturu Grid se jevi jako dobry napad, protoze beztak potrebuju pracovat s ruzne indexovanyma
//polema promennych ruznych typu, a to je presne Grid. Pote jako druhy krok projdu ve smeru
//top-down celej Constraint a vytvorim si wrapy na jednotlive soucastky Constraintu;
//kazdej wrap reprezentuje jednu Relation ma linky na ??klice/managedValues?? v Gridu,
//ktery spravuje. Ve treti fazi pak prochazim ??pole?? wrapu a v nejakem poradi zkousim,
//zda urcity wrap dokaze vymyslet dedukci. Pokud ano, zapracuju dedukci a jedu dal tak dlouho...
//
//Nu a v tomto okamziku resim ten krucinalni problem toho, jak ma vypadat takovy wrap, resp.
//jeho spoluprace s Relationou a zpusoby hledani dedukci. Zde mame 1. rozdil oproti wrapu
//pro initstep: wrap pro solver vraci dedukci, nikoli vyslednou hodnotu.

template<typename Operation, typename...Args>
class Wrap {
};

}
