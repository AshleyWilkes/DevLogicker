#include "gtest/gtest.h"
#include "input/mock_input.hpp"

namespace {

TEST(MockInput, DelegatesGet) {
  //vytvorit InnerInput bez vnitrni tridy
  MockInput<> innerMockInput;
  //vytvorit OuterInput s InnerInputem jako vnitrni tridou
  MockInput<MockInput<>> outerMockInput{ innerMockInput };
  //setnout expectation na InnerInput.get()
  EXPECT_CALL( innerMockInput, mockedGet() );
  //zavolat OuterInput.get()
  outerMockInput.get<int>( "" );
}

}
