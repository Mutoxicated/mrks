env Main

let global rubric be: 
    EmptyLiteralCheck: $expr == "" 
        * throw "Do $expr.isEmpty() instead"

    ErrorNeglection: ^variable.type is Error and ^variable.name == "_" and ^value.origin isnt self
        * throw "Cannot be ignoring errors"

let function a(int num)-> bool be:
    if num == 0:
        return true
    
    return false

let function b be:
    if true:
        return

print("hi ")

// Comment // TOKEN