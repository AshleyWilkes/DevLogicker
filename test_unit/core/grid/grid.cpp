#include "gtest/gtest.h"
#include "core/grid/grid.hpp"
#include "core/grid/mock_grid.hpp"
#include "core/grid/common.hpp"

namespace {

namespace grid = logicker::core::grid;

char valueStr[] = "value";
using MValueId = ManagedValueId<valueStr, int>;

char mapStr[] = "map";
using MMapIdBIF = ManagedMapId<mapStr, bool, int, float>;

char unknownStr[] = "unknown";

using MValuesIds = ManagedIdsSet<MValueId, MMapIdBIF>;

using MValueTypeI = ManagedValueType<int, typename DummyManagementType::type<int>>;
using MValueTypeF = ManagedValueType<float, typename DummyManagementType::type<float>>;

using MValueSlot = ManagedValueSlotFromId<MValueId, MValueTypeI>::type;

using MMapType = ManagedMapType<bool, MValueTypeI, MValueTypeF>;
using MMapSlot = typename ManagedMapSlotFromId<MMapIdBIF, MMapType>::type;

using MSlotsSet = ManagedSlotsSetFromIds<ManagedIdsSet<MValueId, MMapIdBIF>, DummyManagementType>;

//SlotInstance:
//vrati spravny ManagedValue pro value slot
static_assert(std::is_same_v<typename SlotInstance<MValueSlot>::type,
    ManagedValue<int, typename DummyManagementType::type<int>>>);
//vrati spravny ManagedMap pro map slot
static_assert(std::is_same_v<typename SlotInstance<MMapSlot>::type, ManagedMap<MMapType>>);

//grid type jde vytvorit z vyjmenovanych MValueIds a zadaneho ManagementTypu
using Grid1 = Grid<DummyManagementType, MValueId, MMapIdBIF>;
//grid type jde vytvorit ze setu MValuesIds a zadaneho ManagementTypu
using Grid2 = Grid<DummyManagementType, MValuesIds>;
//grid type jde vytvorit ze zadaneho MSlotsSetu
using Grid3 = Grid<MSlotsSet>;

static_assert(grid::is_same_grid_v<Grid1, Grid2>);
static_assert(grid::is_same_grid_v<Grid1, Grid3>);
static_assert(grid::is_same_grid_v<Grid2, Grid3>);

using Grid4 = Grid<DummyManagementType, MValueId>;

//grid typy lze porovnavat na inkluzi -- zde management type nehraje zadnou roli;
//jinymi slovy na urovni typu jsou vsechny management typy vzajemne porovnavatelne,
//napr. proto, ze mohou byt setnute na stejnou hodnotu
static_assert(grid::is_subgrid_v<Grid1, Grid2>);
static_assert(grid::is_subgrid_v<Grid4, Grid1>);
static_assert(! grid::is_subgrid_v<Grid1, Grid4>);

using Grid = Grid1;

//grid instance jde vytvorit, addnout ManagedValue, fetchnout ji, vse za pouziti auto id ("value", resp valueStr)
/*TEST(Grid, SetAndGetWithAutoId) {
  Grid grid;
  grid.get<valueStr>().set( 42 );
  EXPECT_EQ( grid.get<valueStr>().get(), 42 );
  grid.get<mapStr>().set();
  EXPECT_EQ( grid.get<mapStr>().get(), nullptr );
}*/

//grid instance jde vytvorit, addnout ManagedValue, fetchnout ji, vse za pouziti ManagedValueId
TEST(Grid, SetAndGetWithMValueId) {
  Grid grid;
  grid.get<MValueId>().set( 42 );
  EXPECT_EQ( grid.get<MValueId>().get(), 42 );
  grid.get<MMapIdBIF>().add<MValueTypeI>( true, 24 );
  EXPECT_EQ( grid.get<MMapIdBIF>().get<MValueTypeI>( true ).get(), 24 );
  grid.get<MMapIdBIF>().add<MValueTypeF>( false, 21.0 );
  EXPECT_EQ( grid.get<MMapIdBIF>().get<MValueTypeF>( false ).get(), 21.0 );
}

//fetch na v ManagedSlotsSet... neobsazeny typ je compile-time error
TEST(Grid, UnknownMIdType) {
  Grid grid;
  //grid.get<unknownStr>();//this does not compile
}

//!!!!!JE ZDE PROBLEM!!!!!
//aby davalo smysl testovat inkluzi mezi gridy, je treba mit k dispozici slozitejsi
//ManagementType, nez je stavajici verze Dummyho: ten totiz umoznuje max jednu hodnotu
//
//instance gridu lze porovnavat na inkluzi (pomoci operator<=):
//grid1 je subgridem gridu2, prave kdyz
//- typ gridu1 je subtypem gridu2
//- pro kazdy MId instance MValue v gridu1 je podmnozinou instance MValue v gridu2
//
//test, ze grid1 <= grid2 nekompiluje, kdyz Grid1 neni subgrid Gridu2
//test s Gridem s 1 MValueId, 2 ruzne instance, porovnani na obe strany (jeden succ a jeden fail) 
//test s Gridem s 1 MMapId, 2 ruzne instance, porovnani na obe strany (jeden succ a jeden fail)
//test s relativne slozitym Gridem, 2 ruzne instance, porovnani na obe strany (1 succ a 1 fail)

TEST(MockManagedValue, DelegatesGet) {
  //vytvorit InnerManagedValue bez vnitrni tridy
  MockManagedValue<> innerMockMValue;
  //vytvorit OuterManagedValue s InnerManagedValue jako vnitrni tridou
  MockManagedValue<MockManagedValue<>> outerMockMValue{ innerMockMValue };
  //setnout expectation na InnerManagedValue.get()
  EXPECT_CALL( innerMockMValue, mockedGet() );
  //setnout expectation na OuterMValue.get(), aby se predeslo uninteresting call warningu
  EXPECT_CALL( outerMockMValue, mockedGet() );
  //zavolat OuterManagedValue.get()
  outerMockMValue.get();
}

TEST(MockManagedValue, DelegatesSet) {
  //vytvorit InnerManagedValue bez vnitrni tridy
  MockManagedValue<> innerMockMValue;
  //vytvorit OuterManagedValue s InnerManagedValue jako vnitrni tridou
  MockManagedValue<MockManagedValue<>> outerMockMValue{ innerMockMValue };
  //setnout expectation na InnerManagedValue.set()
  EXPECT_CALL( innerMockMValue, mockedSet( 42 ) );
  //setnout expectation na OuterMValue.set(), aby se predeslo uninteresting call warningu
  EXPECT_CALL( outerMockMValue, mockedSet( 42 ) );
  //zavolat OuterManagedValue.set()
  outerMockMValue.set( 42 );
}

TEST(MockGrid, DelegatesGet) {
  //vytvorit InnerMock bez vnitrni tridy
  MockGrid<> innerMockGrid;
  //vytvorit OuterMock s InnerMockem jako vnitrni tridou
  MockGrid<MockGrid<>> outerMockGrid{ innerMockGrid };
  //setnout expectation na InnerMock.get()
  EXPECT_CALL( innerMockGrid, mockedGet() );
  //setnout expectation na OuterMock.get(), aby se predeslo uninteresting call warningu
  EXPECT_CALL( outerMockGrid, mockedGet() );
  //zavolat OuterMock.get()
  outerMockGrid.get<MValueId>();
}

}
