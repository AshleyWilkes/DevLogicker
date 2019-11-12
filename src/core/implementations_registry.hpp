#pragma once

//takze co?
//chci poskytovat rozhrani pro kolekce objektu, implementujicich urcitou Operation nebo Relation
//implementace je classa, ktera si sama urcuje, jakou hlavicku ma jeji vypocetni metoda
//podstatne je, ze ma nejakej
//using params_type
//a metodu
//static OutT perform(params_type)
//
//!!!!resp. ta metoda v predchozim radku muze vracet ValueSet<OutT>, neni vyreseno!!!!
//to totiz neni treba pro InitStepy, takze udelejme napred tyto a zkusenejsi pristupme k reseni
//Constraintu. Sounds good.
//
//registry samotna ma template<typename... Args> metodu, ktera bere libovolne parametry,
//prochazi zaregistrovane implementace a pokousi se je pouzit
//tech libovolnych parametru musi byt stejny pocet jako vstupnich parametru implementacni
//metody (zkouseji se jen ty implementacni metody, ktere maji spravny pocet parametru)
//a napred se kazdy z parametru zkusi zkonvertit na typ, ktery implementacni metoda pozaduje
//to se provede volanim konverzni metody get<T>(), je tedy treba, aby vsechny inputy,
//ktere zde chci povolit (MappingType, core::input, ?) mely tuhle metodu a vedely,
//ze maji vyhodit vyjimku, kdyz konverzi nelze provest. Konverzi nelze provadet pri kompilaci,
//musi to byt run-time zalezitost. 
//
//registry musi pri kompilaci znat vsechny implementacni classy, ktere ma brat v potaz
