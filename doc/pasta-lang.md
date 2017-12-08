# Language Manual of Pasta Programming Language

## Acknowledge
Thanks for my teacher Ming Guo.

## Backgound
This is a final assignment of my compiling course. Making a programming language is quite a challenging and exciting tasts, it can improve and enhance the knowledges and parsing techniques learned from the course, while transfer some ideas in my mind into a true artifact. Owing to it is my first trail on making a programmig language, my main purpose is to get fimilar with concepcts learned from course, so I will not add some fancy feature (coroutine, lambda expression, OOP, etc.) into this language. It will be a very simple toy programming language, but it has basic programming elements.

## Features and Limitations
Pasta is a Pascal like programming language. You can consider it as a subset of Pascal, but some grammar is slightly difference than origin Pascal. Basically, it has following features.

1. Types in Pasta include *real*, *integer*, *bool*, *character*, *array*, *string*, *record* and *enumerate*. I will explain each types below.
2. Declaring variables of each type above.
3. Declaring self-defined compound structured type through type *record*
4. Defining function and procedure.
5. Basic arithematic operation, for example, addition, subtraction, multiplication and division. Some other operations are supported and will be memtioned below.
6. Atomic statement and Compound statement. 
5. Basic Controll flow that you can find in many other language, including *conditional statement*, *loop statement* and *compound statement*.

### Limitations
1. Pasta doesn't support Object-Oriented features.
3. Pasta does not support module in current version, you have to write all functions, procedures, definitions and declarations in one file.

## Code examples
Let's take fibonacci sequence as an example and have a better feel with this small language.

```pascal
Program Fib;
function Fib(n: integer): integer;
var
    a0, a1, i: integer;
begin
    a1 := 0;
    a2 := 1;
    if n = 0 or n = 1 then
        Fib := 1;
    else if n > 1 then
        for i := 1 to n do
        begin
            a1 := a2;
            a2 := a1 + a2;
        end;
        Fib := a1; 
    else
        Fib := -1;
end;

begin
    write Fib(10);
end.
```

**Example of recursive function**
~~~pascal
Program FibRecursive;
function Fib(n: integer): integer;
begin
    if n < 0 then
        Fib := -1;
    else if n = 0 or n = 1 then
        Fib := n;
    else
        Fib := Fib(n - 1) + Fib(n - 2);
end;

begin
    write Fib(10);
end.
~~~

## Language Reference Manual
### Types and Literals
#### Primitive Types
| Name          | Prefix    | Examples          |
|---            |: ---     :|: ---             :|
| Boolean type  | boolean   | true or false     |
| Real Number   | real      | 2.34 0.334 1.2e-3 |
| Integer       | integer   | 10, -10, 0        |
| String        | string    | "value", "", "s"  |
| Characater    | character | 'a', 'b' , '\n'   |

##### implicit convertion
Pasta allow convert a variable to other type atomatically when evaluating a expression if it meet follow situation.
1. Integer to Real
    1. When evaluating a addition, subtraction, multiplication and division expression, if one of the operant is a integer and another is a real number, that integer will be converted to real number implictly.
    2. When assign a integer value to a variable that has real type, that integer value will be converted to a real number implictly.
3. Character to String
    1. When join a charater value to a string value, the character value will be converted to a string value implictly.

According to rules above, following expression is valid.

1. `2 + 3.4`, result is a real number 5.4
2. `a := 2 {a has a real type}`, equals to assign 2.0 to variable a
3. `'H' + "ello"`, result is a string `"Hello" `

There are some invalid expression
1. `b := 3.5 {b has a integer type}`, real literal constant should convert to integer explicitly.
2. `'H' + 2`, cannot plus a integer to a character.
3. `3.2 mod 3`, mod required two integer operants.

##### Convertion functions
Sometime we need to convert a value to another type mandatory, in this case, we should call convertion functions. There a number of convertion functions in Pasta. 
1. integer
    1. `real(integer: num): real;` convert a integer to real number  explicitly.
    2. `istr(integer: num): string;` convert to string
    3. `ch(integer: num): char;`

2. real
    1.`floor(real: num): integer;`
    2.` trunc(real: num): integer;`
    3.` rstr(integer: num): integer;`

3. character:
    1. `cstr(char: ch): string;`
    2. `code(char: ch): integer`

4. string:
    1. `ParseToInt(str: string): int;`
    2. `ParseToReal(str: string): real;`

#### Compund Types
| Name          | Prefix         | Description                                           |
|---            |: ---          :|: ---                                                 :|
| Record        | record         | A bunch of variables that has a unique identifier     |
| Array         | array          | A array of variables with fixed length                |
| Emumerate     | enumerate      | A set of identifier                                   |
| Reference     | ref            | a referece to a object                                |

All of variables has compound type will allocated on the heap. The programmer need not to care about when we should release the memory, since pasta gc will do these for us.

##### Record
A Record is a structure contains a number of field. Each field inside a record has a unique identifier and a definite type, you can consider it as a special vaiable. You can define a record by following grammar.

~~~Pascal
record
    FieldOne: integer;
    FieldTwo: real;
end
~~~

The example above define a record that has two field. The first field is a integer and another has a type of real.

If there are some fields have same type in a record, you can write the identifiers together, then seperate them by commas, and specified their type at the end of the identifier sequence, like below:

~~~Pascal
record
    Weight, Height: real;
    Name: striing;
end
~~~

A field can not only be a primitive type,  but also can be a compound type, for example, a the field *personal* contains a employee's personal infomation is a record.

~~~pascal

type PersonalInfo= record
    name, address: string;
    sex: boolean;
end;

type Employee= record
    personal: PersonalInfo;
    CompanyName: string;
end; 
~~~

**Variant record and record tag**
A record can be variant, to define a variant record, use case...of syntax and record tag.

record
    case tag: integer of
        1: (Foo: integer;);
        2: (Bar: real; Buzz: string;);
end.


When the tag's value is one, the record meant to store a integer value in the field `Foo`, otherwise, when the tag's value equals two, it should store a real type and a string type variable. Since pasta doesn't check the value if validate in running time, so you can assign a real variable to the Bar field when the tag's value equals one, but this is a dangerous action and may cause your program halt.

**notice**
A record type should bind to a identifier using`type <identifier> = ...` syntax. Pasta don't support anoynomous record type, for example:

~~~Pascal
{ example one }
type Bar = record
    foo: record
        buz: int;
        raz: real;
    end;
    plu: real;
end;

{ example two }
type foo = array [0..10] of record
    bar: real;
    buz: integer;
end;

{  example three }
function bar(bar: integer): record
    bull: integer;
    sed: real;
 end;
~~~
To solve the conflict, associate those anoynomous record with a identifer.

The reason why I prefer to do this is that pasta determine if two types is same by checking if they have same type identifier, rather than check out the type's structure.


##### Array
A array is a array of variable that have same type. The length of array is fixed at compiling time. You can access the item of array through indexing. To define a array, use following syntax.


array [<low>..<hi>] of <type>

Mention that both low and hi is integer literal constant.

Multi-demensional array is supported


array [<low1>..<hi1>, <low2>..<hi2>] of <type>


in fact, the definition above equals


array [<low1>..<hi1>] of array [<low2>, <hi2>] of <type>

I suggest to use the first kind of definition instead of the second one, it is a syntacitc sugar that can make your code briefer.


Var
    Arr : array [1..10] of integer;
Begin
    Arr[1] := 1;
    write Arr[1]; {display 1}
End.
  

Undetermined array is also supported in pasta. To use it, just omit the array range part when you declare a array, for example:

Var
    Arr: array of integer;


Multi-demesional array also can be undetermined, for example:

Var
    Arr: array of array of integer;


and it can be partial undetermined, for example:

Var
    Arr: array of array [1..5] of integer;

{ or }
Var
    Arr: array [1..5] of array of integer;


This mechinisum provide a elastic way on dealing with data, because sometimes we just wan't a array of data, and don't care about the array's length.

##### Operations on array
1. Indexing
2. assignment


**constrains**

Owing to unspecified index range of undetermined array, the compiler cannot know how many memory it should be allocated to, so the undetermined array declared may not existed on the memroy. Probably we can say that undetermined array isn't a real array. Operating on undetermined array may cause some unhapply experience, and may raise error to your program. To avoid risks, the array operations follow rules below:

1. Premise: two array's item should have same type. 

2. A undetermined array should be initialized before using (e.g. indexing, assigns). To initialize a undetermined array, Assign a determined array to it. Otherwise, the compiler should complain give a compliing warnning.

3. A undetermined array only can be assigned through
    1. A Determined array
    2. A initialized undetermined array

4. You cannot assigns a undetermined array to a determined array, No matter it has initialized or not.

5. Only two determined array that have same upper bound and lower bound can do assignment.

6. If you assigns an undetermined array that does not been initialized yet, to another array, the program should be terminated and raises a runtime error.

7. If the index to access an array's item is out of range, the program should be termiated and raises a runtime error.

##### Reference
Reference is something like the pointer in C language, it store the address of certain object, and can access the object through dereferencing. But unlike dengerous and free pointer in C, the reference is save and restricted. The only operation allowed is assignment. Common arithmethic operation, such as addition , subtraction is prohibited.

Reference is a compound type, because it should have a object to refer. To define a reference, use folowing syntax:

ref <type>


for example `ref integer` will defined a reference point to a integer.

the object to be refer can either on the stack or on the heap, both situation are allowed.

With reference type and record, we can define a linked-list data structure.

```pascal
type LinkedListItem of E = record
    item: E;
    next: ref LinkedListItem;
end;

type LinkedList of E = record
    list: LinkedListItem;
    length: int;
end;
```

**operations about reference**
1. @ get the address of a object.
2. ! access the object that the reference points to.
3. := assignment, assign a reference value to another reference.

We should notice that no convertion existed to convert a non-reference type to a reference type. This means that you cannot assign a integer value to a reference, like many programmer do often when working with C language.

example:
Say we declared two variable, IntRef and IntVar. IntRef is a reference of integer, and it points to the variable IntVar, which has a type of integer.

to refer the integer variable, you should get its address first, then assign the address to the reference, so the code would be:

(* previous declarations *)
IntRef := @IntVar;

After variable Intvar is refered, its value can be alter indirectly. Rather than evaluates a integer expression and assigns it to the variable, you can de-refer the reference first, get the left value of the integer variable, then assign the value to the left value. It provide a more flexible way to mulnipulate variable, and extends the capacity of programming language on express complex data structure or data flow, for example, you can design a recursive abstract data structure such as linked-list  easily.

###### string
String is quite a special type, because we can regard its variable as a array of character, but since it is very frequently used when programming, and operating a string as a array is inconvience, so in Pasta language, we wrap this 'array of character' into a object and regard it as a elementary type, and provide a serial of function on operating a string.

The function to manipulate strings are provided below:
**functions for conversion**
1. ParseToInt(str: string): int;
2. ParsetoReal(str: string): real;
3. GetArray(str: string): array of char;

**functions about characters**
4. IndexOf(ch: char): integer;
5. GetChar(index: index): char;

**functions on operating string itself**
6. StrLength(str: string): integer
7. SubString(start, end: integer): string;

### Operators and Expression

#### Comments
Only one type of the comment is accepted, enclosed by two brace:`{...}`

#### Identifiers and Keywords
Identifier are sequence combination of letters, digits and underscore. The first character must be a letter or underscore, and is case-insensitive.

**valid variables**
apple
i
_count
_4re

**invalid variables**
4e
e-3
1q4

**keywords**
the following word is keywords of Pasta, you cannot use them as identifier in porgram

**Program** /**Function** /**Begin** /**End** /
**For** /**While**/**Repeat**/**Integer** /
**Until** /**If** /**Else** /**Inc**/**Dec**  /
**Ref** /  **Real** /**String** /**Boolean** /
**Character** /**Array** /
**Enumerate** / **Record**/ **Write**/ **Read**/ **Case**/ **Of**

#### Arthmetic Operators
the arthmetic operators are +, -, \*, /.
The + and - operators have lower precedence than * and /, by convention, + means addition, - means subtraction, * means multiplication and / means division. all operands shoule be have a real type, and the result is a real number too.

**valid input**
1.0 + 2.0
1.3 - 3.2
2.3 * 2.1
1.2 + 3.2 * 1.3 - 2.9 / 4.2

**invalid input**
1.0 + true
false - false

#### Logical and Relational Operators
Relational Operator >, < ,<=, >=, = are in the same precedence, in fact they has lowest precedence among all operators.

** invalid example **
`a == 1 and b <= 2` 

** valid example **
`(a == 1) and (b <= 2)` means a is equal to one and b smaller or equal than 2.  Among `and`,`or`, `not`, not has highest precedence, then `and`, and `or` has lowest precedence.

between two side of logical operator `and`, `or` and `not` should be expressions that have boolean type.

** invalid input **
3.0 and 5.1
2.3 and false

#### precedence list
1. @ !
2. not
3 and, \*, /, mod
4 or, + -
5 =, <>, <, >, <=, >=

### Statements
There are six kinds of statement in Pasta, discriping below.
#### Assignments
Assignment in Pasta followed by syntax bellow:
*variable* := *expression* ;

When we assign a expression to a variable, we change its value.

#### Increase and decrease statements
Increase and decrease statement will increase or decrease a integer variable by one, it is equals to a := a + 1.

**syntax**
inc **i** ;
dec **i** ;

for example:

inc i;
dec i;

is equals to

i := i + 1;
i := i - 1;

and the value of i remain unchanged at last.

#### output and input statements

**syntax**
write **x** ;
read **x** ;

output statement in pasta is `write x`, and the input statement in pasta is `read x`, which x is a right-value expression that has type of integer, string, real, boolean and character. Write statement cannot output a entire array or record, so you have to define a function to deal with each fields inside a record. For the output statement, the x should be a variable having a type that output statement accepts.

The output statement will output the value of expression to the standard output channel, often displays it at the terminal, and the input statement accepts a value from standard input channel, and assign it to the variable.

Let me explain the usage by giving some example:

~~~pascal
write 3; { print constant 3} 
write false; { print constant false }
write "hello, world"; { print string "hello, world" }
write a[3];  { print the third item of array a. }
write e.bar;  { print the field 'bar' of record b }
write result; {print the variable result, which has valid type }

read a; { read a value to variable a, which has valid type. }
read !b; { read a value to variable the b refer to }
~~~

the following usage is invalid:

~~~pascal
write b; { b is a array or a record}
write @a; { cannot print the address }

read 3; { cannot read constant }
read b; { b is a array }
~~~

By convension, the output statement will end with a newline, if you don't want to return a carriage after printing, append a comma a the end of the statement, for example:

~~~pascal
write "Hello, John.", ;
~~~
If you want to output many expression at once, you can write then after the keyword one by one, and seperate them with comma, for example:
~~~pascal
write "3 + 2 = ", 5; 
~~~

#### Compound Statement
Compound statement is a list of statement, enclosed by a begin...end keyword. For example:
~~~Pascal
Begin
    temp := a;
    a := b;
    b := temp;
End;
~~~
The statements' type inside a compound statement does not limit. For example, you can place a compound statement, conditional statement or loop statement ( I will mention them below) inside a compund statement.
#### Loop statements

**while loops**
~~~pascal
Program WhileLoops;
Var
    i: integer;
Begin
    i := 0;
    while i < 10.0 do
        begin
            i = i * 2.0;
            Write i;
        end;
End.
~~~

**for loops**

**syntax**
~~~pascal
for *assignment* to *expression* do
    *statement* ;

or

for *assignment* downto *expression* do
    *statement* ;
~~~
Example code:

~~~Pascal
Program ForLoops;
Var
    i: integer;
Begin
    for i := 1 to 10 do
        Write i;
End.

Program ForLoopCountDown;
Var
    i: integer;
Begin
    for i = 10 downto 10 do
        Write i;
End.
~~~


**repeat loops**
**syntax**
repeat
    *statement*
until *expression*;

Example code:

Program RepeatLoops;
Var
    i: integer;
Begin
    i := 0;
    Repeat
        Write i;
        inc i;
    Until i >= 10;
End;


#### Conditional statements
There are 4 kinds of conditional statement.

**if... statement**

**syntax**

if *expr* then 
    *statement* ;


Var
    x: integer;
Begin
    x := 3.
    if x < 10 then
        Write x;
End;


**if...else... statement**
**syntax**
if *expression* then 
    *statement* 
else *expression* then 
*statement* ;


Var
    x, y: real;
Begin
    x := 3;
    y := 1;
    if x < 3 then
        Write x;
    else
        Write y;
End;


**if...else if...else.. statement**
**syntax**
if *expression* then 
    *statement* 
else if  *expression* then 
    *statement*
...
else
    *statement*;


Example code:

Var
    x, y: integer;
Begin
    x := 3;
    y := 1;
    if x < 3 then
        Write x;
    else if y < 3 then
        Write y;
    else
        Begin
            Write x;
            Write y;
        End;
End;


**case...of..else...end statement**

**syatax**
Case <identity> of
     <Label>: <Statement>;
     <Label>: <Statement>;
    ...
    else
```pascal
Var
    op: string;
    x, y: integer; 
Begin
    read x, op, y;
    case op of
        "+": write x + y;
        "-": write x - y;
        "*": write x * y;
        "/": write x / y;
    else
        write "unknown operator";
    end;
end.
```

The else ... end is optional, and the label should be capatible to the id.
### Program Structure
#### Functions And Procedures
Both functions and procedures are subproblems that may take some argments, or does not take any, then execute some code. The difference between two of them is that function will return a value after it finished.

You can declare local variables at 'var' section. You will see example below.

We define a function like this:

**example**
pascal
function SumThreeNumber(x, y, z: real): real;
Var
    result: real;
Begin
    result := x + y + z;
    SumTreeNumber := result;
End;

Two return and end a function, just assign a expression to function name.

We define a procedure like this:

**example**
pascal
procedure WriteRealNumber(x: real);
Begin
    write x;
End;

#### Program
A Pasta program consist of a program declaration, global variable declaration, type declaration, procedure and function definition and main body.
