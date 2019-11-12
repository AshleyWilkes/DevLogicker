#pragma once
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
};

}
