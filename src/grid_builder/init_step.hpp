#pragma once
#include "core/implementations_registry.hpp"
#include "core/grid/managed_id.hpp"
#include <tuple>

namespace logicker::grid_builder {

template<char* id>
struct InputField {};

template<typename OutputsTo_, typename OperationId_, typename... Reads_>
class InitStep {
  public:
    using OutputsTo = OutputsTo_;
    using Reads = std::tuple<Reads_...>;
    using Requires = std::tuple<OutputsTo, Reads>;

    using OperationId = OperationId_;

    template<typename Grid, typename Input>
    static void perform( Grid& grid, const Input& input ) {
      //extractnout veci z Reads
      auto readVals = readValues( grid, input, std::make_index_sequence<sizeof...(Reads_)>{} );
      //zavolat OperationId, OutT
      using namespace logicker::core::implementations_registry;
      using Operation = ImplementationsRegistry<OperationId, typename OutputsTo::type>;
      auto resultValue = Operation::get().perform( readVals );
      //vysledek ulozit do OutputsTo
      grid.template get<OutputsTo>().set( resultValue );
    }
  private:
    template<typename Grid, typename Input, std::size_t... Is>
    static auto readValues( const Grid& grid, const Input& input, std::index_sequence<Is...> ) {
      return std::make_tuple<>( readValue( std::tuple_element_t<Is, Reads>{}, grid, input )... );
    }

    template<typename ValueType, typename Grid, typename Input>
    static auto readValue( const ValueType& vType, const Grid& grid, const Input& input ) {
      using namespace logicker::core::grid;
      if constexpr ( std::is_base_of_v<ManagedId, ValueType> ) {
        return grid.template get<ValueType>().get();
      } else {//tady by to chtelo rozlisovat jeste InputValue a jako treti moznost compile-time error
        return input.template get<ValueType>( ValueType::name );
      }
    }
};

}
