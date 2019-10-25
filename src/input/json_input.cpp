#include "json_input.hpp"
#include "boost/dynamic_bitset.hpp"
#include <vector>

namespace logicker::core::input {
  json_input::json_input( const Json::Value& value ) : value_{ value } {}

  json_input build_input( const std::string& document ) {
    Json::Value document_value;
    Json::Reader reader;
    if (! reader.parse( document, document_value, false )) {
      throw std::invalid_argument{ reader.getFormattedErrorMessages() };
    }
    return json_input{ document_value };
  }
}
