env Main

let global rubric be: 
	EmptyLiteralCheck: $expr == ""
		* throw "Do $expr.isEmpty() instead"

	ErrorNeglection: ^variable.type is Error and ^variable.name == "_" and ^value.origin isnt self
		* throw "Cannot be ignoring errors"

// params: int a, int b
// returns: bool
// desc: Blahblahblah
let function a be:
	if num == 0: 
		return true
	ASDA
	return false
ASDASD
let function b be:
	if true:
		return

print("hi ")

// Comment // TOKEN

/*
multi
line
*/

wow