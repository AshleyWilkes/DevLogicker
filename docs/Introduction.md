# Logicker

Logicker is planned as a collection of modules and specifications aimed at describing and working with logic puzzles. The puzzles come in a large variety of types differing by the rules they obey and to create a method to describe these rules is the first goal of Logicker. Other goals include actually creating descriptions of known puzzle types and writing libraries that will allow to easily write programs to check validity of a solution to a puzzle, find a solution of a puzzle, display a puzzle in a GUI for human users to solve and more.

Logicker will be developed inside DevLogicker git repository intended as a sandbox where no rules about coding discipline need to be required. At appropriate moments other repositories will be created, one for each Logicker module, and these will be used to create a properly software engineered modules using TDD with good test coverage and comprehensive documentation.

The structure of this documentation is as follows:
Part 1 Introduction
1) -- About the goals and general properties of the Logicker system
2) -- Puzzle solving as finding Mappings between Places (small areas) on a paper and Symbols written in these places
3) -- Value as a base for modelling Puzzles using Mappings
4) -- Bag and Sequence as grouping Values
5) -- Topology and Coords as a base for modelling Places
6) -- Item as a way to assign Meaning to Symbols in Places
Part 2 Describing puzzles or static properties of Logicker
1) -- Grid as a collection of MappingTypes (i.e. of pairs of ValueTypes)
2) -- Operation as a base for modelling computation over Values
3) -- ??Basic?? Operations over grouping Values (e.g. AllOf, Filter)
3) -- ValueExtractor as an Operation for reading Values from GridStatus
4) -- ItemExtractor as an Operation for reading Items from GridStatus using ValueExtractors
5) -- Constraint as a base for modelling Meaning
Part 3 Using puzzle descriptions or the dynamic stuff
1) -- Domains, their different ??(3)?? interfaces
2) -- Mapping, it's dynamic forms, creation, I/O
3) -- Workers, general idea
4) -- DumbChecker, the simplest Worker
5) -- AssignmentChecker
6) -- RulesChecker
7) -- Reader
8) -- Writer
9-?) -- The rest: Solver, Analyzer, Displayer, Creator
Part 4 Putting it all together to write programs

# Part 1 Introduction

*There are many puzzle types; we want to write programs to work with them, therefore we need a description. It's not clear whether general workers loading puzzle rules at runtime is plausible, this version goes another way.*

*Generally a Puzzle assignment consists of a set of rules and of an incompletely filled Grid. The task is to complete the Grid in a way that satisfies all the rules. To complete the Grid is to fill Values into or in-between the Grid's fields **or rather cells** (i.e. areas that form the Grid).*
*The concept of a field used to be central to my concept of a Puzzle; upon closer inspection it's just a special case of Mapping: field (as the old notion) is a Mapping between Coords (that are needed to describe a Topology, which is needed to describe a Grid) and an arbitrary set of Values. In a newer notion, field is an arbitrary set of Mappings between Coords and Value. Because not all fields in a Grid need to support the same set of Values (think Tapa), Mappings allow fore more fine-grained description. I mean, fields are just a visual representation of their Coords. Seen as such it is just an abstraction useful for thinking about Puzzles, but not so much for describing them.* 

*Puzzle can be thought of as a collection of variables; when correct values are assigned to these, some Constraint holds. We use Operations to describe the Constraint of a puzzle as a compile-time type and template parameterize classes generally called Workers with these types.*

# Part 2 Describing puzzles

## Chapter 1 Grid, Cells, Coords and Values

**Description**
*Most Puzzles that inspired this work are presented as an (half) empty Grid and a set of rules, which specify how to fill-in the Grid to obtain a solution. That usually means either: a) enter one or more values into the cells, b) connect centers of the cells, c) divide the cells into separate regions. Where option a) explicitly states the task is to fill in some value somewhere, options b) and c) implicitly require the same thing, the difference is just that the value (denoting whether there is a connection/division or not) is not entered into the cells themselves, but rather in between them. The clues (which can be seen as values of a kind) given in some Puzzles are even placed in corners between four (in square grids) or six (in hexa grids) cells. To acoomodate all these options we introduce a more general notion of Coords to allow us to describe the possible value placemenets.*
*Our notion of what a Puzzle is is therefore transformed into assigning appropriate values to appropriate coords to fulfill the Puzzle's rules.*
*The notion of a Grid is replaced with a notion of Topology, which is the source of the coords we are to consider in a Puzzle.*
*The other basic notion we use to describe Puzzles is a Value. A Value is anything that can be readily written/read to/from a stream and that can represent a part of an assignment/solution of a Puzzle. The basic Values, provided by Logicker's core, are Boolean (which can also represent shaded vs unshaded, connected vs unconnected), Int and Letter.*
*Once we have the Boolean and Int types, I believe we are better off (i.e. can get a simpler framework) without bools and ints, so e.g. Constraint should be an Operation that returns a Boolean, not a bool.*

## Chapter 2 Item

## Chapter 3 Operation

**Description**
*Operation is a static (compile-time) semantic unit, which is a symbol for a specific algorithm to count a value of some type.*
*To specify an operation there exist named SimpleOperation and CompositeOperation; SimpleOperation symbolizes conjuring the result out of thin air (i.e. from runtime objects), CompositeOperation uses other Operations as input and thus provides a means to specify more complicated operations by putting together simpler ones.*
*Operation should cause some kind of compile error if it's arguments don't fit together -- e.g. AllOf<S,P> should have a Set<E> and Predicate<E>; both Es must be the same otherwise the declaration is invalid*

**To be moved to Part 3 Chapter 0) Using operations**
*Using an operation to obtain a runtime value is called **evaluation**. CompositeOperations use 2-part evaluation: first their inputs are **evaluated**, then an **implementation** of the Composite is performed and it's result used. Implementation of a Composite does only depend on return types of it's input operations, while evaluation of each input depends on the complete type of that input.*
*The Operation defines its static semantic meaning, it does not specify any means of using runtime information (e.g. Grid); specifying that is the responsibility of Extractors' evaluations.*
*Sometimes, however, (think ForEach) an Operation needs to send information (typically an Item) to its child!!! Where does this belong??*

### Operation interface
*using typename OutT*
*using std::tuple<typename> InTs*

**What kinds of Operations can I think of:**
*- Extractors: produce Items, are SimpleOperations
- Combiners: produce Items, are CompositeOperations
    - CollectSet...

- Predicates: take Item, produce bool
    - Equality

- Logical connectives:
    - AllOf...
    - And, Not, ...
- Constraints: produce bool values, they are Logically connected Predicates*

**What do those kinds actually mean?**
*Extractor extracts a Value from a Mapping*
*Combiner creates Items from Values and/or Items*
*Predicates determine whether an Item matches some characteristic*

## Chapter 2 Item

**Description**
*In the static description any type can serve as an item; there it enables sanity syntactic checking.*
*At runtime instances of the Item type are needed; in the end every predicate leads to equality between two instances.*
*Item classes are declared in something I'll call module for the lack of a better word; perhaps it could be header only library or a separately compiled and linked library. Item module should follow some basic principles: have its own namespace, have no dependencies on other parts of Logicker (apart from when it's built on top of other item modules).*
*Item module may also declare Operations to operate on the Item, namely Extractors, Combiners and Predicates.*
*Simple Items are Values, Composite Items consist of Values or Items.*

//****** Poznamky k Extractoru ******//
//!!nasleduje (zmateny) popis problemu, ale napred je zde reseni!!
//!!ale ja proste muzu chtit, aby se mi operace chovala jako strom s danym poradim uzlu, a aby neuplne specifikovana slozita operace poslouchala stejnej interface jako zcela nespecifikovana jednoducha operace!!
//!!to bylo to reseni, ted ten popis!!
//
//extractory se vyskytuji v ruznych formach:
//1) extractor mappingu bez parametru; vzdy uveden se vsemi parametry, ehm
//2) extractor mappingu s 1 parametrem; uvedeny i se svym parametrem
//3) extractor mappingu s 1 parametrem; uvedeny bez sveho parametru
//
//totez se, zda se, tyka Operations, a zdehle debata o extractorech je jen specialni pripad
//uplne stejneho problemu s Operations; u Operations avsak neni pochyb o tom, ze muzou mit
//vice nez jeden vstup??
//
//jde teda o ruzne formy zapisu toho, jak se ma Operation pouzit ke konverzi Inputs->Outputs
//kdyz vyloucim moznost, ze bych chtel zapsat napul-bindovanou operaci (napr. mela by 2 parametry
//a jeden bych nastavil a druhy ne), zbyvaji jen ty dve moznosti: zadny nebo vsechny
//
//pokud neni parametr stanoven zadny, pak mam efektivne misto operace pouze jeji Id, jedna
//se o jeji dobre specifikovanou templatu; nazev stale musi byt plne specifikovan
//
//pokud jsou parametry specifikovany vsechny, mam cele volani
//
//zkusme prozatim cestu, kde extractor je validni, pokud obsahuje jako prvni parametr svuj mapping, a pokud pripadne nasledujici parametry odpovidaji operacim, vracejicim hodnoty jednotlivych operandu extractoru ve spravnem poradi; takto zadane operandy nemuseji byt vsechny
//
//tento plan, zda se, je funckni, pouze mozna narazi na technicke problemy s C++ sablonami
//
//a jeste je jeden problem, a to, kdyz chci specifikovat extractor, jehoz parametrem je jiny extractor, a je to tento jiny extractor, ktery nema zadany parametr
//****** Poznamky k Extractoru ******//

//****** Poznamky k Mappingu ********//
//z teoretickeho uhlu pohledu je mapping funkce (anebo konstanta, kde
//veci fungujou stejne, jako kdyby se jednalo o funkci, jejiz definicni
//obor obsahuje prave jednu hodnotu); u vyreseneho Puzzlu je funkce
//presne znama: zname presny definicni obor a pro kazdou hodnotu
//v nem zname i funkcni hodnotu. Naopak u nevyreseneho Puzzlu funkce
//plne znama byt nemusi. Basically jedina vec, kterou o ni vzdy vime,
//je, ze je to funkce jednoho parametru a zname typ parametru a typ
//funkcni hodnoty; ??mozna lze pripustit, aby slo o funkci vice parametru,
//coz je totez jako kdyz jediny parametr je tuple, je to potreba??
//Nezridka, prakticky vzdycky, ale mame nejake informace o omezenich
//na definicni obor a/nebo na obor hodnot, a tyto informace chceme
//zadat jako soucast popisu Puzzlu, abychom
//a) usnadnili Solveru jeho praci -- needs SetBased
//b) umoznili Displayeru, aby poskytoval pohodlny user-interface
//    naplnujici uzivatelska ocekavani -- needs GeneratorBased
//c) umoznili odhalovat pri nacitani sanity errory (treba cislo 7
//    v Sudoku 6x6) -- needs CriterionBased
//
//Jake moznosti Domain si umim predstavit:
//1) SetBased -- seznam hodnot, ktere pripadaji v uvahu
//2) CriterionBased -- unarni predikat nad hodnotami: true -- hodnota je pripustna
//3) GeneratorBased -- metoda, kterou lze opakovane volat a dostavat pripustne hodnoty
//
//Nova hlavicka
//Maps<string, Domain<Key>, Domain<Value>>
//prip.
//Maps<string, Domain<Value>>
//****** Poznamky k Mappingu ********//

//********* Poznamky k SlotId **********//
//!!Nasledujici je asi ku smazani; koncept SlotId byl nejspis zamergovan
//do Mappingu ve forme jmena Mappingu, ktere puvodne nebylo nedilnou
//soucasti jeho typu jako ted!!
//
//je treba zvazit defaultni slotIds; oc jde: chci, aby pri popisu puzzlu
//existovala moznost mapovat na stejne "opravdove" klice vice hodnot
//nezavisle na sobe; za tim ucelem jsem si vymyslel slotId, ktere spolu
//s "opravdovymi" klici umoznuji vytvorit unikatni klic do kazde mapy
//jako std::pair<realKey, slotId> (zatim pod slovem klic myslim dusledne
//typ, slouzici jako leva strana v definici Maps). Mit slotId jako
//compile-time string by melo usnadit implementaci Mapperu, jinak by
//v principu slotId mohla byt az runtime zalezitost; i v tom pripade by
//ale bylo zadouci, aby hodnotu slotId specifikoval jiz popis Puzzle.
//
//je ovsem jiste, ze existence slotIds zbytecne komplikuje zapis v situaci,
//kdy mi staci mit k dispozici pro urcity "opravdovy" klic jediny slot.
//Nedava smysl delat pro tento pripad specialni implementaci bez slotId,
//lepe pouzit defaultni hodnotu slotId.
//********* Poznamky k SlotId **********//
