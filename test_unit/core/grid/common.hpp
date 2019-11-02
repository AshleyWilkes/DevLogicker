#pragma once

namespace {

using namespace logicker::core::grid;
namespace type = logicker::type;

template<typename valueT>
class DummyManagementType {
  public:
    DummyManagementType( const valueT& value ) : value_{ value } {}
    valueT get() const { return value_; }
  private:
    valueT value_;
};

}
