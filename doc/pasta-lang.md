# Language Manual of Pasta Programming Language

## Acknowledge
Thanks for my teacher Ming Guo.

## Backgound
This is a final assignment of my compiling course. Making a programming language is quite a challenging and exciting tasts, it can improve and enhance the knowledges and parsing techniques learned from the course, while transfer some ideas in my mind into a true artifact. Owing to it is my first trail on making a programmig language, my main purpose is to get fimilar with concepcts learned from course, so I will not add some fancy feature (coroutine, lambda expression, OOP, etc.) into this language. It will be a very simple toy programming language, but it has basic programming elements.

## Features and Limitations
Pasta is a Pascal like programming language. You can consider it as a subset of Pascal. It primerely has following features and limitations: 

## Features
1. You can declare and assign value to a variable. 
2. Procedures and functions is supported, you can define and invoke them like other pascal program. 
3. Controll flow is supported, you can you while, for, repeat and if statement in pascal style.
4. Pasta compiler is provided, but I am not going to build a read-eval-print-loop intepretor.

### Limitations
1. There are only *two data* types in current version of Pasta Language: **real  and integer**. the *real type* represents a float number, which is equilenuivalent to double type in C. The *boolean type* represent two logical value, true and false. 
2. You cannot define you own data type. 
3. Pasta does not support module in current version, you have to write all functions, procedures, definitions and declarations in one file.
4. We don't support compound data like C's structure or array.

## Code examples
### Fib number
```pascal
Program Fib;
function Fib(n: real): real;
var
    a0, a1, i: real;
begin
    a1 := 0;
    a2 := 1;
    if n = 0 or n = 1 then
        Fib := 1.0;
    else if n > 1 then
        for i := 1 to n do
        begin
            a1 := a2;
            a2 := a1 + a2;
        end;
        Fib := a1; 
    else
        Fib := -1.0;
end;

begin
    writeln(Fib(10));
end.
``` 

Example of recursive function
```pascal
Program FibRecursive;
function Fib(n: real): real;
begin
    if n < 0 then
        Fib := -1;
    else if n = 0 or n = 1 then
        Fib := n;
    else
        Fib := Fib(n - 1) + Fib(n - 2);
end;

begin
    writeln(Fib(10));
end.
```

## Language Reference Manual
### Types and Literals
#### Primitive Types
As we have mentioned, in current version, pasta has two data types: boolean and real.
| Name | Prefix   | Description   |
|---   |: ---    :|: ---         :|
| Boolean type | boolean | true or false, example: ```true false``` |
| Real Number | real | Possible value: A IEEE 754 double floating point, <br /> example ```2.34 0.334 1.2e-3``` |

### Operators and Expression

#### Comments
Only one type of the comment is accepted, enclosed by two brace:`{...}`

#### Identifier

Identifier are sequence combination of letters, digits and underscore. The first character must be a letter or underscore.

**valid variables**
apple
i
_count
_4re

**invalid variables**
4e
e-3
1q4

#### Arthmetic Operators
the arthmetic operators are +, -, *, /.
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

** valide example **
`(a == 1) and (b <= 2)`

means a is equal to one and b smaller or equal than 2.

Among `and`,`or`, `not`, not has highest precedence, then `and`, and `or` has lowest precedence.

between two side of logical operator `and`, `or` and `not` should be expressions that have boolean type.

** invalid input **
3.0 and 5.1
2.3 and false

#### precedence list
1. not
2. and, *, /
3. or, + -
4. =, <>, <, >, <=, >=

### Control Flow
#### Loops

**while loops**
```
Program WhileLoops;
Var
    i: real;
Begin
    i := 0.0;
    while i < 10.0 do
        begin
            i = i * 2.0;
            WriteLn(i);
        end;
End.
```

**for loops**
Program ForLoops;
Var
    i: real;
Begin
    for i := 1.0 to 10.0 do
        WriteLn(i);
End.

**repeat loops**
```
Program RepeatLoops;
Var
    i: real;
Begin
    i := 0.0
    Repeat
        WriteLn(10);
        i := 1.0;
    Until i >= 10;
End;
```

#### Conditionals
There are 3 kinds of conditional statement.

**if... statement**
```
Var
    x: real;
Begin
    x := 3.4;
    if x < 3.9 then
        WriteLn(x);
End;
```

**if...else... statement**
```
Var
    x, y: real;
Begin
    x := 3.4;
    y := 1.0;
    if x < 3.9 then
        WriteLn(x);
    else
        WriteLn(y);
End;
```

**if...else if...else.. statement**
```
Var
    x, y: real;
Begin
    x := 3.4;
    y := 1.0;
    if x < 3.9 then
        WriteLn(x);
    else if y < 3.9 then
        WriteLn(y);
    else
        Begin
            WriteLn(x);
            WriteLn(y);
        End;
End;
```

### Program Structure
#### Functions And Procedures
Both functions and procedures are subproblems that may take some argments, or does not take any, then execute some code. The difference between two of them is that function will return a value after it finished.

You can declare local variables at 'var' section. You will see example below.

We define a function like this:

**example**
```pascal
function SumThreeNumber(x, y, z: real): real;
Var
    result: real;
Begin
    result := x + y + z;
    SumTreeNumber := result;
End;
```
Two return and end a function, just assign a expression to function name.

We define a procedure like this:

**example**
```pascal
procedure WriteRealNumber(x: real);
Begin
    writeln(x);
End;
```
#### Class declaration
A class depicts a group of objects that have same attributes and can do same action. To declaration a class, use 'class' keyword below.
```
type Counter = class
private
    count: integer;
public
    constructor initialize();
    begin
        count := 0;
    end;

    procedure inc();
    begin
        count = count + 1;
    end;
end;
```

To create a object through a class, first you should declare a reference.
```
...
var
    counter: Counter
...
```

then invoke a object construtor 'initialize'.
```
    counter := Counter.initialize();
```

You needn't to worry about when you should destory the object and free the memery, the Pasta GC will do this job atomatically for you.

We also support the generic type in the language. You have to place the generic type's name embrace by a pair of parenthnese, after the class keyword.
for example:

```
...
type Stack = class<Item>
...
```
This will  define a LIFO stack data type, which is used frequencely in programming.

A class can have more than one generic types. You should seperate then with commas, like:

```
...
type Map = class<Key, Item
```
To create a object through class with generic type, first you should declare a reference, then you should write down the actual type inside the parenthnese, for example
```
...
var
    s: Stack of integer;
begin
    s := Stack<integer>.initialize();
...
```

```
var
    m: Map of integer, string;
begin
    s := Map<integer, string>.initialize();
...
```

Every class inherit from class object implicitly, unless you write down the parent class when declaring it, like below

```
    type Retailer = class inherit Staff
```

Of course a class can have more than one parent class, for example:
````
...
type Listener = class inherit Component, Container
...

type Component = class
...

type Container = class
...
```
The code below will declare a class names Listener, which inherits classes Component and container, and classes Component and Container inherits class Object implictly.


the class Object is vary simple:
type Object = class
public
    function ToString(): string;
#### Program
A Pasta program consist of a program declaration, global variable declaration, class declaration, procedure and function definition and main body. Here is a simple example:
```
Program Example;
type StackError = class inherit Error
    constructor initialize(msg: string);
    begin
        super.msg := msg;
    end;
     
    function message(): string;
    begin
        message := this.message;
    end;
end;
 

var
     buffer: array of integer;
begin
    buffer := integer[0..10];

    buffer.hi;
    buffer.low;
    buffer.length;


type Stack = class<Item> inherit Object
private
    length: integer;
    list: ListItem;
    type StackItem = class
        item: T;
        next: ListItem;
    End;
public
    constructor initialize();
    begin
        length := 0;
        list := Null;
    end;
      
    procedure pop(): Item;
    var
        result: Item;
    begin
        if this.length < 0 then
            raise StackError("empty stack")
        else
        begin
            result := this.list.item;
            this.list := this.list.next;
            pop := result;
        end;
    end;
    
    procedure push(item: Item);
    var
        newItem: StackItem ;
    begin
        newItem := StackItem.initialize();
        newItem.item := item;
        newItem.next := this.list;
        this.list := newItem;
    end;

    procedure ToString();
    var
        currentItem: StackItem;
        result: string;
    begin
        result := "";
        currentItem := this.list.item;
        
        while currentItem <> Null do
            result := result + currentItem.ToString() + " ";

        ToString := "[" + result + "]";
    end;
end;

procedure Main();
var
    stack: Stack of integer;
begin
    stack := Stack<integer>.initialize();
    stack.push(3);
    stack.push(10);
    
    out.writeln(stack);

    stack.pop();
    out.writeln(stack);
end;

begin
    Main();
end.
```
