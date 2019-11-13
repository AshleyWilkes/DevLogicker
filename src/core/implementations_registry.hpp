#pragma once

namespace logicker::core::implementations_registry {

//takze co?
//chci poskytovat rozhrani pro kolekce objektu, implementujicich urcitou Operation nebo Relation
//implementace je classa, ktera si sama urcuje, jakou hlavicku ma jeji vypocetni metoda
//podstatne je, ze ma nejakej
//using params_type
//a metodu
//static OutT perform(params_type)
//
//!!!!resp. ta metoda v predchozim radku muze vracet ValueSet<OutT>, neni vyreseno!!!!
//to totiz neni treba pro InitStepy, takze udelejme napred tyto a zkusenejsi pristupme k reseni
//Constraintu. Sounds good.
//!!!!typ OutputT si musi urcit bud registry sama nebo template paramter jeji perform() metody!!!!
//neni to vlastnost Operationy, jelikoz ValueSet<OutT> viz vyse
//zakladni implementace podporuje jednoznacne (by registry) specifikovany OutT, usoudim-li pro potreby
//  Constraint solvera, ze je uzitecne mit jeden objekt, podporujici vice OutT (pomoci templatovane)
//  perform() metody), udelam wrapper nad nekolika temito zakladnimi implementacemi
//
//registry samotna ma template<typename... Args> metodu, ktera bere libovolne parametry,
//prochazi zaregistrovane implementace a pokousi se je pouzit
//tech libovolnych parametru musi byt stejny pocet jako vstupnich parametru implementacni
//metody (zkouseji se jen ty implementacni metody, ktere maji spravny pocet parametru)
//a napred se kazdy z parametru zkusi zkonvertit na typ, ktery implementacni metoda pozaduje
//to se provede volanim konverzni metody get<T>(), je tedy treba, aby vsechny inputy,
//ktere zde chci povolit (MappingType, core::input, ?) mely tuhle metodu a vedely,
//ze maji vyhodit vyjimku, kdyz konverzi nelze provest. Konverzi nelze provadet pri kompilaci,
//musi to byt run-time zalezitost. 
//
//registry musi pri kompilaci znat vsechny implementacni classy, ktere ma brat v potaz
//
//takze na jedny strane mame statickou compile-time definici templatovane tridy, jejiz singletom
//predstavuje interface pro klienty
//a na druhy strane je vnitrek metody perform ty templatovany tridy, kterej dela ten vypocet
//  tenhle vnitrek by nejspis mel mit precizneji definovana pravidla fungovani, coz by umoznilo
//  dost mozna i relativne snadno navrhnout sirsi sadu testu
//  pravidla jsou na druhou stranu dost easy: musi byt stejne parametru tam i tam a musi pro kazdy
//    z nich existovat konverze zleva doprava. Pak lze vstupy registrovy perform() metody pouzit
//    jako vstupy implovy perform() metody.

template<typename Operation_, typename OutT_>
class ImplementationsRegistry;

template<typename... Implementations>
class ImplementationsRegistryBase {
  private:
    ImplementationsRegistryBase() = default;
  public:
    ImplementationsRegistryBase(const ImplementationsRegistryBase&) = delete;
    void operator=(const ImplementationsRegistryBase&) = delete;

    template<typename... Args>
    static int perform( const Args&... );
};

}
