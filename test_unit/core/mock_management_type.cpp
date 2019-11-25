#include "gtest/gtest.h"
#include "core/mock_management_type.hpp"

namespace {

using ::testing::NiceMock;

//MockReadOnlyManagementType
//test, ze get vola mockedGet a vrac spravnou hodnotu
TEST(MockReadOnlyManagemenetType, GetWorksAndDelegatesToMockedGet) {
  MockReadOnlyManagementType manTyp{ 28234 };
  EXPECT_CALL( manTyp.getMock(), mockedGet() );
  EXPECT_EQ( manTyp.get(), 28234 );
}

//MockRangeManagementType
//Ok testy
//test, ze zafunguje vytvorit, setnout, getnout (vcetne volani mockedXXX metod)
TEST(MockRangeManagementType, CreateSetGetWorks) {
  MockRangeManagementType<5> manTyp{ 72 };//range [72, 76]
  EXPECT_CALL( manTyp.getMock(), mockedSet( 73 ) );
  EXPECT_CALL( manTyp.getMock(), mockedGet() );
  manTyp.set( 73 );
  EXPECT_EQ( manTyp.get(), 73 );
}

//test, ze zafunguje vytvorit, setnout minimum, getnout (vcetne volani mockedXXX metod)
TEST(MockRangeManagementType, SetMinimumWorks) {
  MockRangeManagementType<165> manTyp{ 69 };
  EXPECT_CALL( manTyp.getMock(), mockedSet( 69 ) );
  EXPECT_CALL( manTyp.getMock(), mockedGet() );
  manTyp.set( 69 );
  EXPECT_EQ( manTyp.get(), 69 );
}

//test, ze zafunguje vytvorit, setnout maximum, getnout (vcetne volani mockedXXX metod)
TEST(MockRangeManagementType, SetMaximumWorks) {
  MockRangeManagementType<48> manTyp{ 101 };
  EXPECT_CALL( manTyp.getMock(), mockedSet( 148 ) );
  EXPECT_CALL( manTyp.getMock(), mockedGet() );
  manTyp.set( 148 );
  EXPECT_EQ( manTyp.get(), 148 );
}

//test, ze zafunguje vytvorit, poerasovat vsechny but one, getnout (vcetne volani mockedXXX metod)
TEST(MockRangeManagementType, SetByErasingWorks) {
  MockRangeManagementType<3> manTyp{ 9 };//range [9, 11]
  EXPECT_CALL( manTyp.getMock(), mockedErase( 9 ) );
  EXPECT_CALL( manTyp.getMock(), mockedErase( 10 ) );
  EXPECT_CALL( manTyp.getMock(), mockedGet() );
  manTyp.erase( 9 );
  manTyp.erase( 10 );
  EXPECT_EQ( manTyp.get(), 11 );
}

//test, ze getValueSet() cerstve vytvoreneho manTypeu vrati vsechny hodnoty
TEST(MockRangeManagementType, GetValueSetOnFreshManagementTypeReturnsAllValues) {
  MockRangeManagementType<4> manTyp{ 6543 };
  EXPECT_CALL( manTyp.getMock(), mockedGetValueSet() );
  EXPECT_EQ( manTyp.getValueSet(), ( std::set{ 6543, 6544, 6545, 6546 } ) );
}

//test, ze getValueSet() po par erasech vraci hodnot min
TEST(MockRangeManagementType, GetValueSetOnPartiallyErasedManagementTypeReturnsOnlyNonErasedValues) {
  MockRangeManagementType<8> manTyp{ 9955 };
  EXPECT_CALL( manTyp.getMock(), mockedErase( 9956 ) );
  EXPECT_CALL( manTyp.getMock(), mockedErase( 9958 ) );
  EXPECT_CALL( manTyp.getMock(), mockedErase( 9960 ) );
  EXPECT_CALL( manTyp.getMock(), mockedErase( 9962 ) );
  EXPECT_CALL( manTyp.getMock(), mockedGetValueSet() );
  manTyp.erase( 9956 );
  manTyp.erase( 9958 );
  manTyp.erase( 9960 );
  manTyp.erase( 9962 );
  EXPECT_EQ( manTyp.getValueSet(), ( std::set{ 9955, 9957, 9959, 9961 } ) );
}

//test, ze getValueSet() po setu vraci hodnotu jednu
TEST(MockRangeManagementType, GetValueSetOnManagementTypeWithSetValueReturnsTheSingleValue) {
  MockRangeManagementType<6> manTyp{ 91 };
  EXPECT_CALL( manTyp.getMock(), mockedSet( 95 ) );
  EXPECT_CALL( manTyp.getMock(), mockedGetValueSet() );
  manTyp.set( 95 );
  EXPECT_EQ( manTyp.getValueSet(), ( std::set{ 95 } ) );
}

//Fail tesy
//test, ze nezafunguje vytvorit, setnout minimum - 1, getnout (out_of_range)
TEST(MockRangeManagementType, FailOnSetLessThanMinimum) {
  NiceMock<MockRangeManagementType<9>> manTyp{ 66 };
  EXPECT_THROW( manTyp.set( 65 ), std::out_of_range );
}

//test, ze nezafunguje vytvorit, setnout maximum + 1, getnout (out_of_range)
TEST(MockRangeManagementType, FailOnSetMorehanMaximum) {
  NiceMock<MockRangeManagementType<65>> manTyp{ 201 };//range [201, 265]
  EXPECT_THROW( manTyp.set( 266 ), std::out_of_range );
}

//test, ze nezafunguje vytvorit, erasnout minimum - 1 (out_of_range)
TEST(MockRangeManagementType, FailOnEraseLessThanMinimum) {
  NiceMock<MockRangeManagementType<9>> manTyp{ 66 };
  EXPECT_THROW( manTyp.erase( 65 ), std::out_of_range );
}

//test, ze nezafunguje vytvorit, erasnout maximum + 1 (out_of_range)
TEST(MockRangeManagementType, FailOnEraseMoreThanMaximum) {
  NiceMock<MockRangeManagementType<65>> manTyp{ 201 };//range [201, 265]
  EXPECT_THROW( manTyp.erase( 266 ), std::out_of_range );
}

//test, ze po vytvoreni a setnuti novej set vyhodi logic_error
TEST(MockRangeManagementType, FailOnSetAfterSet) {
  NiceMock<MockRangeManagementType<64>> manTyp{ 94 };
  manTyp.set( 100 );
  EXPECT_THROW( manTyp.set( 106 ), std::logic_error );
}

//test, ze po vytvoreni a setnuti erase vyhodi logic_error
TEST(MockRangeManagementType, FailOnEraseAfterSet) {
  NiceMock<MockRangeManagementType<64>> manTyp{ 94 };
  manTyp.set( 100 );
  EXPECT_THROW( manTyp.erase( 106 ), std::logic_error );
}

//test, ze po vytvoreni a vyerasovani vsech but one set vyhodi logic_error
TEST(MockRangeManagementType, FailOnSetAfterSetByErasing) {
  NiceMock<MockRangeManagementType<2>> manTyp{ 51 };//range [51, 52]
  manTyp.erase( 51 );
  EXPECT_THROW( manTyp.set( 52 ), std::logic_error );
}

//test, ze po vytvoreni a vyerasovani vsech but one erase vyhodi logic_error
TEST(MockRangeManagementType, FailOnEraseAfterSetByErasing) {
  NiceMock<MockRangeManagementType<2>> manTyp{ 51 };//range [51, 52]
  manTyp.erase( 51 );
  EXPECT_THROW( manTyp.erase( 52 ), std::logic_error );
}

//test, ze set s drive erasnutou hodnotou vyhodi logic_error
TEST(MockRangeManagementType, FailOnSettingPreviouslyErasedValue) {
  NiceMock<MockRangeManagementType<124>> manTyp{ 85 };
  manTyp.erase( 100 );
  EXPECT_THROW( manTyp.set( 100 ), std::logic_error );
}

//test, ze opakovanej erase stejny hodnoty vyhodi logic_error
TEST(MockRangeManagementType, FailOnErasingPreviouslyErasedValue) {
  NiceMock<MockRangeManagementType<124>> manTyp{ 85 };
  manTyp.erase( 100 );
  EXPECT_THROW( manTyp.erase( 100 ), std::logic_error );
}

}
