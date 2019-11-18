/*#include "grid_builder/init_step.hpp"
#include "core/grid/grid.hpp"
#include "core/grid/common.hpp"

//co ma fungovat?
//InitStep ma mit metodu perform(grid, input)
//ta ma extractnout Reads veci, vybrat vhodnou implementaci
//OperationId, zavolat implementaci a vysledek ulozit do OutputsTo
//
//jina moznost: NEW VERSION, TO IMPLEMENT
//chci mock grid, mock input a mock implementations_registry
//overit, ze cteni gridu zpusobi volani metody gridu
//overit, ze cteni inputu zpusobi volani inputu
//overit, ze volani perform() stepu zpusobi volani perform() impl_registry s prectenymi hodnotami
//overit, ze zapis do gridu zpusobi volani metody gridu s hodnotou vracenou z operation
//
//tedy postup testu by byl:
//vytvorit mock grid a mock input
//udelat nejakej DummyOperation a vytvorit mock implementations_registry
//udelat InitStep, kterej cte neco z gridu a neco z inputu, vola DummyOperation a zapisuje do gridu
//setnout expect call na cteni inputu, gridu, volani operation a zapis do gridu
//zavolat InitStep::perform*/

/*namespace logicker::core::implementations_registry {

class DummyOperation{};

template<>
class ImplementationsRegistry<DummyOperation, int> :
    public ImplementationsRegistryVase<> {};
}

namespace {

char valueIdStr[] = "value";
using MValueId = ManagedValueId<valueIdStr, int>;

char inputFieldStr[] = "input";
using InputFieldId = InputField<inputFieldStr>;

using TestInitStep = InitStep<
  MValueId,
  InputFieldId,
  DummyOperation,
  MValueId
>;

TEST(InitStep, CallsReadsOperationAndOutputsTo) {
  MockGrid<> grid;
  MockInput<> input;
  EXPECT_CALL( grid, get() ).Times( 2 );
  EXPECT_CALL( input, get() );
  EXPECT_CALL( implRegistry, perform() );
  TestInitStep::perform( grid, input );
}

}*/

/*namespace {

using namespace logicker::grid_builder;

namespace grid = logicker::core::grid;

char valueFromInputStr[] = "valueFormInput";
using MValueFromInputId = ManagedValueId<valueFromInputStr, int>;

char valueFromGridStr[] = "valueFromGrid";
using MValueFromGridId = ManagedValueId<valueFromGridStr, int>;

using Grid = Grid<DummyManagementType, MValueFromInputId, MValueFromGridId>;

char inputNameStr[] = "input";

using InitStepFromInput = InitStep<
  MValueFromInputId,
  DummyOperation,
  InputField<inputNameStr>>;

using InitStepFromGrid = InitStep<
  MValueFromGridId,
  DummyOperation,
  MValueFromInputId>;

TEST(InitStep, InitStepFromInput) {
  Grid grid;
  //vytvor input se vstupni hodnotou
  InitStepFromInput::perform( grid, input );
  //tady by to chtelo, aby DummyManagementType zde byl mock, kteryho se muzu zeptat, jestli
  //  doslo k zavolani jeho metody setValue( 42 )
  //  MValueFromInputId volani ma mit
  //  MValueFromGridId volani nema mit
}

TEST(InitStep, InitStepFromGrid) {
  Grid grid;
  //nastav do promenne MValueFromInputId hodnotu
  InitStepFromGrid::perform( grid );
  //tady by to chtelo, aby DummyManagementType zde byl mock, kteryho se muzu zeptat, jestli
  //  doslo k zavolani jeho metody setValue( 42 )
  //  MValueFromInputId volani nema mit
  //  MValueFromGridId volani ma mit
}

}

//co chci testovat? OLD VERSION TO DELETE
//chci pouzit nejaky dummy grid s dvema values typu int
//chci pouzit nejakou dummy operation, asi identitu
//chci overit, ze se hodnota z inputu ulozi do prvni value
//chci overit, ze se hodnota z prvni value ulozi do druhe value*/
