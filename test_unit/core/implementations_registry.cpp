#include "gtest/gtest.h"

//neni jasne, jak ma fungovat moznost, kdy npar. pod jednou Operation mam registrovane jak
//metody vracejici OutT, tak metody vracejici ValueSet<OutT>. To proted neresim, at se pohnu.
namespace {

//chci mit 3 implementace jedne operationy; 2 s jednim parametrem, 1 s dvema parametry; kazda necht vraci
//  pro stejne inputy ruzny output.
//chci zaregistrovat ty 3 operations do registry
//chci zavolat perform metodu registry na4krat tak, aby
//a) pouzila prvni implementaci s 1 parametrem
//b) pouzila druhou implementaci s 1 parametrem
//c) pouzila implementaci s 2 parametry
//d) nenasla zadnou pouzitelnou implementaci

}
