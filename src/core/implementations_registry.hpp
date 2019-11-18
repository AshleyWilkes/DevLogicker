#pragma once
#include "core/convertor.hpp"

namespace logicker::core::implementations_registry {

//takze co?
//chci poskytovat rozhrani pro kolekce objektu, implementujicich urcitou Operation nebo Relation
//implementace je classa, ktera si sama urcuje, jakou hlavicku ma jeji vypocetni metoda
//podstatne je, ze ma nejakej
//using params_type
//a metodu
///*static*/ OutT perform(params_type)//rusim static, jelikoz staticke metody nejdou mockovat
//
//!!!!resp. ta metoda v predchozim radku muze vracet ValueSet<OutT>, neni vyreseno!!!!
//to totiz neni treba pro InitStepy, takze udelejme napred tyto a zkusenejsi pristupme k reseni
//Constraintu. Sounds good.
//!!!!typ OutputT si musi urcit bud registry sama nebo template parameter jeji perform() metody!!!!
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
//takze na jedny strane mame /*statickou*/ compile-time definici templatovane tridy, jejiz /*singleton*/
//instance predstavuje interface pro klienty
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

    //zde je tedy treba prochazet jednotlive Implementations
    //  to lze udelat partialnimi specializacemi pro Impl, Impls...
    //  lze to udelat partialni specializaci Impls, int
    //  lze nemit perform metody staticky a v perform prochazet normalne v run-timu
    //    instanci std::tuplu, v niz jsou jednotlive instance implementaci
    template<typename... Args>
    static int perform( const Args&... args ) {
      return performImpl<Implementations...>( args... );
    }
  private:
    //using Impls = std::tuple<Implementations...>;
    
    template<typename fImpl, typename... Impls, typename... Args>
    static int performImpl( Args... args ) {
      //pro implementaci fImpl se pokusim zkonvertit obdrzene parametry na pozadovane parametry
      convertor::Convertor<typename fImpl::InTs> conv( args... );

      if ( conv.is_success()) {//konverze prosla, vracim vysledek z Implementation
        return fImpl::perform( conv.get() );
      } else if constexpr ( sizeof...( Impls ) ) {//konverze neprosla, jdu na dalsi Implementation
        return performImpl<Impls...>( args... );
      } else {//kdyz dosly Implementations, hazu std::domain_error
        throw std::domain_error("");
      }
      return 1;
    }
};

}
