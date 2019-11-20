#include "gtest/gtest.h"
#include "core/grid/grid.hpp"
#include "core/grid/mock_grid.hpp"
#include "core/implementations_registry.hpp"
#include "core/mock_implementations_registry.hpp"
#include "grid_builder/init_step.hpp"
#include "input/mock_input.hpp"

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

namespace logicker::core::implementations_registry {

class DummyOperation{};

template<>
class ImplementationsRegistry<DummyOperation, int> {
  public:
    using Type = MockImplementationsRegistry<>;
    static const Type& get() {
      static const Type& instance{};
      return instance;
    }
};

}

namespace {

using namespace logicker::core::grid;

char valueIdStr[] = "value";
using MValueId = ManagedValueId<valueIdStr, int>;

//toto of course patri uplne nekam jinam
//je trochu zajimava otazka, jestli primo do input v adresari input
//anebo jestli by bylo lepsi udelat mezistupen; ten mezistupen by mohl zaroven
//resit tu zalezitost se subinputem dedikovanym pro assignment/solution
template<auto name_, typename type_>
class InputField  {
  public:
    static constexpr auto name = name_;
    using type = type_;
};

char inputFieldStr[] = "input";
using InputFieldId = InputField<inputFieldStr, int>;

using TestInitStep = logicker::grid_builder::InitStep<
  MValueId,
  InputFieldId,
  logicker::core::implementations_registry::DummyOperation,
  MValueId
>;

//ad registry. Chci, aby InitStep::perform pouzil mockImplementationsRegistry a ne normalni 
//ImplementationsRegistry. To znamena, ze ten init step nemuze prohledavat globalni namespace
//na templatovou promennou Operation, ale rather nejakou instanci nejakeho containera
//na registry. Anebo by se muselo zaridit, aby init step v globalnim namespace nasel toho Mocka.
//Pricemz ale Mock je obal na Registry, zatimco normalni implementace je normalni Registry.
//Takze by se InitStep musel ptat na neco jinyho nez na ImplementationsRegistry<Operation>.
//
//Cili to cele smeruje k tomu, aby pri kompilaci vznikla singleton instance neceho jako 
//collection of ImplementationRegistries, ktera by interne obsahovala SetT jednotlivych
//registries a mela by metodu get<Operation, OutT>(), ktera by vracela instance techto registries
//
//Mno ale ta varianta se singleton instanci ma zasadni omezeni: neni zrejme, jak do ni "dynamicky"
//(tj nejak auto-registracne) pridavat implementace snadno proste tim, ze implementaci napisu.
//Tuto moznost naopak velmi snadno naplnuje stavajici varianta se specializacemi primarni
//templaty. Toto omezeni je natolik zasadni, ze kvuli nemu opoustim (docasne?) opoustim moznost,
//popsanou v predchozim odstavci, a vymyslel jsem, jak pouzit variantu se specializacemi
//primarni templaty i pri testovani.

TEST(InitStep, CallsReadsOperationsAndOutputsTo) {
  MockGrid<> grid;
  MockInput<> input;
  using namespace logicker::core::implementations_registry;
  using ImplReg = ImplementationsRegistry<DummyOperation, int>;
  EXPECT_CALL( grid, mockedGet() ).Times( 2 );
  EXPECT_CALL( input, mockedGet() );
  EXPECT_CALL( ImplReg::get(), mockedPerform() );
  TestInitStep::perform( grid, input );
}

}

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
