#pragma once

namespace logicker::core::convertor {

//(co nasleduje jsou drevorubecky popsane vyhody Convertoru oproti konverznim metodam)
//konverzni metody nelze partially specializovat, coz potrebuju, abych mohl
//  v pripade Required = Actual pouzit bezpodminecnou identitu. Proto se priklanim k moznosti mit
//  misto toho konverzni classu. Ta je z principu veci schopna vracet informace uzivatelsky
//  prijatelnejsim zpusobem, nez jsou vyjimky, coz je skvele
//(konec drevorubecky popsanych vyhod)
//
//multi verze ma umet i neuspesnou konverzi v pripade nestejneho poctu
//  poskytnutych a pozadovanych hodnot

template<typename... Required>
class Convertor;

template<typename... Required_>
class Convertor<std::tuple<Required_...>> {
  public:
    using Required = std::tuple<Required_...>;

    Convertor() = delete;

    template<typename... Actual>
    Convertor( const std::tuple<Actual...>& args ) {
      if constexpr ( sizeof...(Actual) == sizeof...(Required_) ) {
        try {
          value_ = convert( args, std::make_index_sequence<sizeof...(Actual)>{} );
          success_ = true;
        } catch ( const std::invalid_argument& ) {}
      }
    }

    bool is_success() const { return success_; }
    Required get() const {
      if ( success_ ) return value_;
      throw std::domain_error( "" );
    }
  private:
    bool success_{ false };
    Required value_;

    template<typename... Actual, std::size_t... Is>
    auto convert( const std::tuple<Actual...>& args, std::index_sequence<Is...> ) {
      return std::make_tuple<>(
          convert<typename std::tuple_element_t<Is, Required>>( std::get<Is>( args ) )... );
    }

    template<typename Req_, typename Act_>
    Req_ convert( const Act_& arg ) {
      if constexpr ( std::is_same_v<Act_, Req_> ) {
        return arg;
      } else if constexpr ( std::is_class_v<Act_> ) {//insufficient, for now good enough
        return arg.template get<Req_>();
      } else {
        throw std::invalid_argument( "" );
      }
    }
};

}
