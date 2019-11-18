#include "gtest/gtest.h"
#include "core/implementations_registry.hpp"
#include "core/mock_implementations_registry.hpp"

namespace logicker::core::implementations_registry {

class DummyOperation {};

//chci mit 3 implementace jedne operationy; 2 s jednim parametrem, 1 s dvema parametry; kazda necht vraci
//  pro stejne inputy ruzny output.
class Impl1Bool {
  public:
    using Implements = DummyOperation;
    using InTs = std::tuple<bool>;
    using OutT = int;
    static int perform( bool /*b*/ ) { return 1; }
};

class Impl1Int {
  public:
    using Implements = DummyOperation;
    using InTs = std::tuple<int>;
    using OutT = int;
    static int perform( int /*i*/ ) { return 2; }
};

class Impl2 {
  public:
    using Implements = DummyOperation;
    using InTs = std::tuple<int, int>;
    using OutT = int;
    static int perform( const std::tuple<int, int>& ) { return 3; }
};

//???neslo by to napsat nejak pomoci XXX<DummyOperation, int, Impl1Bool, Impl1Int, Impl2>
//nebo jeste lepe tak, aby nebylo ani treba ty prvni dva parametry specifikovat,
//antoz ty musej bejt spolecny pro vsechny implementace, jinak je specifikace invalid a nema
//projit kompilaci???
//
//tim, ze napisu XXX<DummyOperation, int, Impl1Boo, Impl1Int, Impl2> bych chtel zaridit,
//aby zacala byt definovana specializace ImplementationRegistry<DummyOperation, int>,
//zatimco jinak by neexistovala
//to by asi slo, kdyby ImplementationRegistry byla alias templata do XXX, ta by pak
//byla definovana zaroven s touto XXX. Nikoliv, to nejde, protoze XXX musi byt templatovan
//pres vsech 5 parametru, takze ta alias templata nepujde napsat. Zatim necham takto, tj budu
//implementace vyjmenovavat uvnitr definice classy v tuplu.
template<>
class ImplementationsRegistry<DummyOperation, int> :
    public ImplementationsRegistryBase<Impl1Bool, Impl1Int, Impl2> {};

}

namespace {

using namespace logicker::core::implementations_registry;

TEST(ImplementationRegistry, TestImpl1Bool) {
  EXPECT_EQ( (ImplementationsRegistry<DummyOperation, int>::perform( true )), 1 );
}

TEST(ImplementationRegistry, TestImpl1Int) {
  EXPECT_EQ( (ImplementationsRegistry<DummyOperation, int>::perform( 1 )), 2 );
}

TEST(ImplementationRegistry, TestImpl2) {
  EXPECT_EQ( (ImplementationsRegistry<DummyOperation, int>::perform( 1, 1 )), 3 );
}

TEST(ImplementationRegistry, TestNoImpl) {
  EXPECT_THROW( (ImplementationsRegistry<DummyOperation, int>::perform( true, true )), std::domain_error );

}

//chci zaregistrovat ty 3 operations do registry
//  pri registraci chci ocheckovat (jinak compile-time error)
//    ze implementace implementuje prislusnou operaci
//    ze ma usingy InTs a OutT, kde OutT odpovida OutT typu of registry itself
//    ze ma metodu OutT perform( InTs... )
//chci zavolat perform metodu registry na4krat tak, aby
//a) pouzila prvni implementaci s 1 parametrem
//b) pouzila druhou implementaci s 1 parametrem
//c) pouzila implementaci s 2 parametry
//d) nenasla zadnou pouzitelnou implementaci

TEST(MockImplementationRegistry, DelegatesPerform) {
  //vytvorit InnerRegistry bez vnitrni tridy
  MockImplementationsRegistry<> innerMockRegistry;
  //vytvorit OuterRegistry s InnerRegistry jako vnitrni tridou
  MockImplementationsRegistry<MockImplementationsRegistry<>> outerMockRegistry{ innerMockRegistry };
  //setnout epectation na InnerRegistry.perform()
  EXPECT_CALL( innerMockRegistry, mockedPerform() );
  //zavolat OuterRegistry.perform()
  outerMockRegistry.perform<>();
}

}
