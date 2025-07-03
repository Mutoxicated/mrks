# Markus

A general-purpose programming language, with the goal of lending the user unique freedom through setting rules to the compiler, improving code organization and maintainability.

The proof of concept:
```
env Main [ Mode: Prototype ] // Prototype mode uses the VM, making the language interpreted
// There's also other modes, like 'Strict' or 'Normal', both using the compiler instead

let global a be 1

let b be "this is a string"

let global rubric be:
  BlankIdentifierCheck:
    - condition:  ^context.scope.Is(ForLoop) && ^context.secondForVariable.name == "_"
    - throw: "Can't be ignoring for loop 'v' value"

let function name(int integer) -> bool be:
  if integer > 0:
    return true
  return false

let c be name

let rubric be:
  NonPascalCaseMethod:
    - condition: !^context.method.name.caseType.Is(Pascal)
    - throw: "Only PascalCase names are allowed for methods"

// Structs
struct Rectangle:
- int id
- int width
- int height

let method Rectangle.area() -> int be: // 'NonPascalCaseMethod' error!
  return self.width, self.height


```

# Notice

Markus is very early in development, currently only the lexer system is done. That being said, I am simultaneously working on other systems that are extensions of the language, like Polus (the lsp) and soon the Virtual Machine. This is so that we don't have to make compromises later down the line for a new system that has to be developed.
