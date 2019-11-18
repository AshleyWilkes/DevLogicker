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
class Convertor {
  public:
    Convertor() = delete;

    template<typename... Actual>
    Convertor( const Actual&... args ) {
      if constexpr ( sizeof...(Actual) == sizeof...(Required) ) {
        try {
          value_ = std::make_tuple<>( convert<Required>( args )... );
          success_ = true;
        } catch ( const std::invalid_argument& ) {}
      }
    }

    bool is_success() const { return success_; }
    std::tuple<Required...> get() const {
      if ( success_ ) return value_;
      throw std::domain_error( "" );
    }
  private:
    bool success_{ false };
    std::tuple<Required...> value_;

    template<typename Required_, typename Actual_>
    Required_ convert( const Actual_& arg ) {
      if constexpr ( std::is_same_v<Actual_, Required_> ) {
        return arg;
      } else if constexpr ( std::is_class_v<Actual_> ) {//insufficient, for now good enough
        return arg.template get<Required_>();
      } else {
        throw std::invalid_argument( "" );
      }
    }
};

template<typename... Required>
class Convertor<std::tuple<Required...>> : public Convertor<Required...> {
  public:
    Convertor() = delete;

    template<typename... Actual>
    Convertor( const Actual&... args ) : Convertor<Required...>( args... ) {}
};

template<typename Required>
class Convertor<Required> {
  public:
    Convertor( const Required& arg ) : success_{ true }, value_{ arg } {}

    template<typename Actual>
    Convertor( const Actual& arg ) {
      try {
        if constexpr ( std::is_class_v<Actual> ) {
          value_ = arg.template get<Required>();
          success_ = true;
        }
      } catch ( const std::invalid_argument& ) {}
    }

    //more than 1 argument, i.e. failure
    template<typename... Actual>
    Convertor( const Actual&... args ) {}

    bool is_success() const { return success_; }
    Required get() const {
      if ( success_ ) return value_;
      throw std::domain_error( "" );
    }
  private:
    bool success_{ false };
    Required value_;
};

}
