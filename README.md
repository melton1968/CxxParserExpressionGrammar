# Parser Expressionn Grammar
Provides Embedded Domain Specific Language (EDSL) for working with Parser Expression Grammars (PEGs).


## Table of Contents

## Reference

#### `At<P...>` ####
* Succeeds iff `seq<P...>` would succeed.
* Consumes nothing.
* Disables all actions.
* Allows local failure even within `must<>`.

#### `NotAt<P...>` ####
* Succeeds iff `seq<P...>` would not succeed.
* Consumes nothing.
* Disables all actions.
* Allows local failure even within `must<>`.

#### `Character<char C>` ####
Succeeds iff next character is C.

#### `NotCharacter<char C>` ####
Succeeds iff next character is not C.

#### `AnyCharacter<char C>` ####
Succeeds iff there is a next character.

#### `Characters<char C, char... Cs>` ####
Succeeds iff the next characters is C and Characters<Cs...> succeeds.

### Atomic ###

#### `Success` ####
Unconditionally matches.

#### `Failure` ####
Unconditionally fails.

####  `StartOfFile` ####
Matching if input is at beginning of file.

####  `StartOfLine` ####
Matches if input is at beginning of line.

####  `EndOfFile` ####
Matching if input is at end of file.

####  `EndOfLine` ####
Matches if input is at end of line.

####  `EndOfLineOrFile` ####
Matches if input is at end of line.



