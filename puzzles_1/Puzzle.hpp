#pragma once

//v souboru s Puzzlem maji byt tyto casti:
//
//Grid:
//seznam MappingIds, ktera jsou potreba k popisu aktualniho stavu gridu tohoto Puzzlu
//typicky proste seznam standardnich, catalogueovanych MappingIds, coz ale neni podminka
//tento seznam je autoritou pri pripadnych sporech o spravnosti ci nespravnosti popisu jinych casti puzzlu
//
//hmm, standardni typy Gridu by asi taky mely byt catalogueovane, preci jen jimi chci
//  parametrizovat nacitaci templatu
//to by asi znamenalo, ze bude zalezet na poradi, v nemz jsou jednotlive MappingIds uvedeny, coz nechceme
//tomu by asi slo zabranit, kdyz by grid nebyl ten seznam, ale Grid<seznam>; ted nevim, jak si c++ poradi
//  se situaci Grid<A, B> vs Grid<B, A> pri stavajici implementaci meho Gridu (ktera basically vede na
//  Grid<A,B>:public A,B vs Grid<B,A>:public B,A
//
//GridRecipe:
//seznam kroku, jak prevest input na odpovidajicim zpusobem vyplneny Grid.
//-- musi vest na stejny typ, jako je vyse uvedeny Grid
//-- neni prima vlastnost Puzzlu, protoze muze byt ruzny pro ruzne formaty vstupu, coz chceme
//-- ergo kazdy krok popisuje nejaky vztah mezi drive vytvorenymi Mappingy a vstupy na jedne strane
//    a mezi novym Mappingem na druhe strane; konkretni implementace a tedy detailne presny semanticky\
//    vyznam existuje pouze pro konkretni MappingTypy
//-- krok nicmene ma semanticky vyznam i bez specializace pro konkretni MappingType; kroky
//    jsou catalogueovane a dokumentovane
//-- v tomto smyslu je krok dvojice <MappingId, Operation>, kde MappingId je timto krokem inicializovany
//    Mapping a Operation popisuje zpusob, jak urcit hodnotu/y, ktere ma novy Mapping obsahovat; prave
//    interpretace teto Operationy je bod, v nemz se implementace pro ruzne MappingTypy mohou lisit
//
//GridBuilder si vystaci s GridRecipe, libovolnym, je mu jedno, ze to patri k nejakemu Puzzlu
//
//??GridRecipe je taky forma Constraintu, pouze s rozdilem, ze zna Extractory z inputu?? Ano,
//ale pouziva se vzdy jako seznam Operations, nikoli Relations, takze na tom nezalezi
//
//rozlisujeme dva typy "nacitanych objektu": assignment a solution, presneji polovyplneny a 
//    zcela vyplneny obrazec. Toto avsak jednotlive Recipies nema zajimat; jejich design
//    ma byt takovy, ze umozni uzivateli pomoci volby MappingTypu a parametru Recipe dosahnout
//    takoveho efektu, jaky chce (coz casto muze byt prave assignment nebo solution).
