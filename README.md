# Parser Expressionn Grammar
Provides Embedded Domain Specific Language (EDSL) for working with Parser Expression Grammars (PEGs).


## Table of Contents

## Reference

### `at<P...>` ###
Succeeds iff `seq<P...>` would succeed.
Consumes nothing.
Disables all actions.
Allows local failure even within `must<>`.

### `not_at<P...>` ###
Succeeds iff `seq<P...>` would not succeed.
Consumes nothing.
Disables all actions.
Allows local failure even within `must<>`.

### Atomic ###


### Block ###
### Character ###
