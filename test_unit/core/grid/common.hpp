#pragma once

namespace {

using namespace logicker::core::grid;

class DummyManagementType {
  public:
    template<typename valueT>
    class Impl {
      public:
        Impl() = default;
        explicit Impl( const valueT& value ) : value_{ value } {}
        valueT get() const { return value_; }
        void set( const valueT& value ) const { value_ = value; }
      private:
        mutable valueT value_;
    };

    template<typename valueT>
    using type = Impl<valueT>;
};

/*template<typename valueT>
class DummyManagementType {
  public:
    DummyManagementType() = default;
    explicit DummyManagementType( const valueT& value ) : value_{ value } {}
    valueT get() const { return value_; }
    void set( const valueT& value ) { value_ = value; }
  private:
    valueT value_;
};*/

}
