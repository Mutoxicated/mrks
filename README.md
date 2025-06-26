# Markus

A general-purpose programming language, with the goal of lending the user unique freedom through setting rules to the compiler, improving code organization and maintainability.

The proof of concept:
```
let global a be 1

let b be "this is a string"

let global rubric be:
  BlankIdentifierCheck:
    - condition:  ^content.scope.Is(ForLoop) && ^context.secondForVariable.name == "_"
    - throw: "Can't be ignoring for loop 'v' value"

let function name(int integer) -> bool be:
  if integer > 0:
    return true
  return false

let c be name
```
