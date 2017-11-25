# myscript syntax notes

## myscript type
integer
real
boolean

## operations

operation for real type:
1. addition
2. subtraction
3. multiplication
4. divison
5. comparing operators
    1. equal
    2. greater
    3. less
    4. greater or equal
    5. less or equal

addition
symbol: +
usage: real + real
result: real
scolon

subtraction
symbol: -
usage: real - real
result: real

multiplication:
symbol: *
usage: real * real
result: real

division:
symbol: /
usage: real / real
result: real

comparing operators:
equal
symbol: =
usage: float = float
result: bool

greater
symbol: >
usage: float > float
result: bool

less
symbol: <
usage: float < float
result: bool

greater or equal
symbol: <=
usage: float <= float
result: bool


less or equal
symbol: >=
usage: float >= float
result: bool

not equal:
symbol: <>
usage: float <> float
result: bool

1. not
2. and
3. or

not
symbol: not
usage: not bool
result: bool

and
symbol: and
usage: bool and bool
result: bool

or:
syambol: or
usage: bool or bool
result: bool

## the evaluating order of operators
```
1. not
2. and, *, /
3. or, + -
4. =, <>, <, >, <=, >=
```


# syntax
## lexical element
### real
**syntax**
\d+.\d+((e|E)(+|-)\d+)

represent a real number

### boolean
a boolean value is either true or false

### conversion
If you assign a expression with integer type to a real variable, the value will convert to real atomatically, but you cannot assign a real expression to a integer expression.

## program structure
1. program declaration
2. global variable declaration
2. procedures
3. main procedure body.

## program declaration
Every myscript code should start with a program description block, consisting with a 'program' keyword, a program name, and a semicolon. for example:
```
program MyProgramscolon

``` 
This declaration means that this file has a main procedure body, and can be executed or become a entry point as a program.

### grobal variable declaration
this block is to declarer global variable, which is visible for main block and all procedures, the syntax see below:
```
global
    {variable declarations}

```

For example:
```
global
    x: floatscolon
    y, z: boolscolon
``` 
### procedure and procedure body
In myscript, we are not going to divide subprogram into function, those will return a value and procedure, those make effects but won't return value. We all respect them as 'procedure'.

**declaration**
```
{procedure without return value}
procedure proc_name(var1: type, var2: type, ...)scolon

{procedure with return value}
function proc_name(var1:type, var2: type, ...) return_typescolon

```

following a declaration, there should be a procedure body. there are two forms of procedure body, which is shown below

the first one has a keyword `var`, you can declare variables between it and keyword begin, and those declared variables has scope in following begin/end block.


for example:
```
var 
    a: boolscolon
    b, c: float
begin
    a := 3.0scolon
endscolon
```

if you do not need to declare any variable, you can just omit the var block.
for example:

```
begin
    gb := 1.3scolon {gb declared as global variable}
end
```

### conditional statement
if {condition} then
    {statement}

if {condition} then
    {statement}
else
    {statement}

if {condition} then
    {statement}
else if {condition} then
    {statement}

### loop statement
while {condition} do
    {statement}

repeat
    {statement}
until {condition}


#### break and continue
use break statement to jump out the nearest looping statement
use continue statement to start next loop.
#### statement and expression

{statement} ::=
    | {assignment-statement}
    | {conditional-statement}
    | {loop-statement}
    | {compound-statement}
    | break
    | continue

{compound-statement} ::=
    | begin {statement-sequence} end

{statement-sequence} ::=
    | {statement} {statement-sequence}
    | epsilon

{assignment-statement} ::= 
    | identifier asg {expression}

{expression} ::= 
    | {additional-expression} gt   {expression}
    | {additional-expression} lt   {expression}
    | {additional-expression} eq   {expression}
    | {additional-expression} gteq {expression}
    | {additional-expression} lteq {expression}
    | {additional-expression} noeq {expression}
    | {additional-expression}

{additional-expression} ::= 
    | {multiplicative-expression} add {additional-expression} |
    | {multiplicative-expression} sub {additional-expression} |
    | {multiplicative-expression} or  {additional-expression} |
    | {multiplicative-expression}

{multiplicative-expression} ::= 
    | {not-expression} mul {multiplicative-expression}
    | {not-expression} div {multiplicative-expression}
    | {not-expression} and {multiplicative-expression}
    | {not-expression}

{not-expression} ::=
    | not {primitive-expression}
    | {primitive-expression}

{primitive-expression} ::= 
    | true
    | false
    | real
    | integer
    | id
    | lp {expression} rp 


{conditional-statement} ::=
    | keyword(if) {expression} keyword(then) {statements} {rest-conditinal-statement}

{rest-conditional-statement} ::=
    | keyword(else) {statement}
    | keyword(else) {conditional-statement}
    | epsilon

{loop-statement} ::= 
    | {while-loop-statement}
    | {for-loop-statement}
    | {repeat-loop-statement}

{while-loop-statement} ::= 
    | keyword(while) {expression} keyword(do) {statement}

{for-loop-statement} ::=
    | keyword(for) id asg {expression} keyword(to) {expression} keyword(do) {statement}
    | keyword(for) id asg {expression} keyword(downto) {expression} keyword(do) {statement}

{repeat-loop-statement} ::= 
    | keyword(repeat) {statements} keyword(until) {expression}

{variable-declaration-block} ::=
    | var {variable-declaration-sequence}

{variable-declaration-sequence} ::=
    | {variable-declaration} scolon {variable-declaration-sequence}
    | {variable-initializer} scolon {variable-declaration-sequence}
    | {variable-declaration} scolon
    | {variable-initializer} scolon

{variable-declaration} ::=
    | {identifier-sequence} : variable-type

{identifier-sequence} ::= 
    | identifier , {identifier-sequence}
    | identifier

{variable-initializer} ::= 
    | identifer colon variable-type = {expression} // should only a constant expression, no variables and function calls will involved!

{program} ::= 
    | {program-declaration} {global-variable-declaration} {procedure-and-function-definition-sequence} {main-block} dot

{program-declaration} ::= 
    | program identifier scolon

{global-variable-declaration} ::= 
    | {variable-declaration-block}

{procedure-and-function-definition-sequence} ::=
    | {procedure-and-function-definition} {procedure-and-function-definition-sequence}
    | {procedure-and-function-definition}

{procedre-and-function-definition} ::=
    | {procedure-definition}
    | {function-definition}

{procedure-definition} ::=
    | procedure identifier lp {parameter-sequence} rp scolon {variable-declaration-block} {compound-statement} scolon

{function-definition} ::= 
    | function identifier lp {parameter-sequence} rp colon variable-type scolon {vaiable-declaration-block} {compund-statement} scolon

{parameter-sequence} ::=
    | {variable-declaration} scolon {rest-parameter-sequence}
    | {variable-declaration}
    | epsilon

{rest-parameter-sequence} ::=
    | {variable-declaration} scolon {variable-declaration}
    | {variable-declaration}
