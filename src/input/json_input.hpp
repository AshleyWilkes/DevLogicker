#pragma once
#include "boost/dynamic_bitset.hpp"
#include <jsoncpp/json/json.h>
#include <string>
#include "input/input.hpp"
/* Notes 17th october 2019, about a year after the rest of this has been written:
 * Nepamatuju si, proc je zakomentovany include "../input.hpp", mel by byt odkomentovany!
 * Prepsal jsem testy do googletest verze; prepis neni "spravne": interface testy
 * (vsechny krome posledniho) by mely byt udelane jako testy obecneho interface a
 * v json_input.cpp test filu by mely byt jenom instancializovane.*/
/*
 * Implementation of input interface which reads input data
 * from json string.
 *
 * The constructor expects a properly formatted json string
 * and initializes its inner state according to its contents.
 * If the json is not properly formatted the constructor
 * throws invalid_argument whose what() string may contain the
 * explanation of the failure.
 *
 * The interface uses template method get(string) to
 * let users specify their expectations about types
 * contained in the JSON's members. This techinque
 * does not restrict the interface's generality
 * while (having only a single method) it's also
 * as simple as possible. However, to implement such
 * an interface partial sprecialization of that function
 * template is needed. Such a specialization is
 * not directly supported by C++ and therefore it's
 * necessary to use a trick to achieve it. A discussion
 * of the trick used here as well as of some related
 * aspects can be found at fluentcpp.com/2017/08/15/function-templates-partial-specialization-cpp/
 *
 * The get() methods do no format checking; if the underlying
 * library is able to convert the value provided as argument
 * to type provided as template argument, then get() will return
 * the converted value.
 */

//mno jsou zde namitky:
//1) to s tim undefined behaviorem musi byt nejak jinak:
//    ve finale chci postavit knihovnu, ktera bude pracovat
//    s txt vstupem, nad jehoz obsahem tedy nebude zadna
//    kontrola. Umoznit spatne napsanemu txt souboru zpusobit
//    undefined behavior neni dobry napad. 
//Cili resenim je specifikovat, ze v pripade, ze konverze
//    (viz vedle) selze, input vyhodi vyjimku.
//    Pricemz zde tedy jsou jeste porad dve moznosti:
//    bud umoznim jenom "prime" konverze, tj. ty, ktere
//    vlastne ani nic nekonvertuji, antoz proste vraceji
//    napr. bool, kde na vstupu byl bool; anebo umoznim
//    vsechny konverze, jichz je schopen jsoncpp, takze
//    napr. umoznim vratit bool, kde na vstupu je 1.
//
//2) vyhazuju zatim neupravene std::invalid_argument vyjimky,
//    a to je mi pekne. Tohle je spatne uz z toho duvodu,
//    ze takhle zadnou poradnou cestu, jak s vyjimkami
//    pracovat, nikdy nenajdu. Krom toho jakmile zapracuju
//    bod 1), tak uz budu potrebovat dat klientovi snadny
//    zpusob, jak rozlisit mezi vyjimkou vyhozenou proto,
//    ze dany member v jsonu nenu, a vyjimkou vyhozenou,
//    protoze member je jineho typu nez se cekalo.
//Cili resenim je specifikovat momentalne alespon
//    dve vyjimky za ucelem vyhazovani zde v inputu.
//
namespace logicker::core::input {
  class json_input : public input {
    public:
      //json_input( const std::string& document );

      template<typename T>
      const T get( const std::string& name ) const;

      friend json_input build_input( const std::string& document );
    private:
      explicit json_input( const Json::Value& value);
      const Json::Value value_;

      //what follows is a trick allowing partial specialization
      //of function templates; see comment at the beginning
      //of this source file.
      template<typename>
      struct type{};

      template<typename T>
      const T get( const Json::Value& value, type<T> ) const;

      template<typename T>
      const std::vector<T> get( const Json::Value& value, type<std::vector<T>> ) const;
  };

  template<typename T>
  const T json_input::get( const std::string& name ) const {
    if (! value_.isMember( name )) {
      throw std::invalid_argument{ "Member '" + name + "' not found" };
    }
    const Json::Value value = value_[ name ];
    return get( value, type<T>{} );
  }

//these two methods trigger the warning I'm suppressing here
//const has no meaning when returning by value, that is the cause of the warning
//the const is there because it's a part of the primary declaration of the function's template
//clearly the design of this method should be improved
//I have other priorities at the moment and choose to suppress the warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
  template<>
  inline const bool json_input::get<bool>( const Json::Value& value, type<bool> ) const {
    return value.asBool();
  }

  template<>
  inline const int json_input::get<int>( const Json::Value& value, type<int> ) const {
    return value.asInt();
  }
#pragma GCC diagnostic pop

  template<>
  inline const std::string json_input::get<std::string>( const Json::Value& value, type<std::string> ) const {
    return value.asString();
  }

  template<>
  inline const boost::dynamic_bitset<> json_input::get<boost::dynamic_bitset<>>( const Json::Value& value, type<boost::dynamic_bitset<>> ) const {
    auto vec_bool = get( value, type<std::vector<bool>>{} );
    auto result = boost::dynamic_bitset<>( vec_bool.size() );
    for ( std::vector<bool>::size_type i = 0; i < vec_bool.size(); ++i ) {
      if ( vec_bool[i] == true ) {
        result.set( i );
      }
    }
    return result;
  }

  template<>
  inline const json_input json_input::get<json_input>( const Json::Value& value, type<json_input> ) const {
    return json_input{ value }; 
  };

  template<typename T>
  inline const std::vector<T> json_input::get( const Json::Value& value, type<std::vector<T>> ) const {
    std::vector<T> result;
    for ( Json::Value::ArrayIndex i = 0; i < value.size(); ++i ) {
      result.push_back( get( value[i], type<T>{} ) );
    }
    return result;
  }

  json_input build_input( const std::string& document );
}
