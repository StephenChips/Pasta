# 异常类定义
Pasta的编译期异常有：
1. LexingError
2. ParingError
3. SematicError

他们的定义为：
```ocaml
type LexingError = exception of string;
type ParsingError = exception of string;
```

# LexingError 词法错误
当程序存在非法单词的时候，抛出此错误

## 未知单词
当程序中有未知的单词的时候，抛出此错误。

比如：非法的字符：@，#，由此组成的标识符：3#d等

错误信息： "Illegal Token";

## 未结束的字符串
当字符串漏掉结束的引号 \" 的时候，抛出此错误

比如：
"123
" a34#d

错误信息："Undetermined String"

## 未结束的注释
当注释漏掉结束的符号 \} 的时候，抛出此错误

比如：
{ this is a comment

{ FIXME: a bug

错误信息："Undetermined Comment"

## 非法字符
当字符非法的时候，抛出此错误

非法字符包括：
1. 未结束的字符，如 'a
2. 非法转义字符，如'\i'， '\666'
3. 单引号中出现多余一个字符，且不是i转义字符，如'ee'

错误信息："Illegal Character"

# ParsingError 语法错误
当程序存在语法错误的时候，抛出此错误

由于LALR(1)算法不懂，对yacc的处理步骤不熟悉，这步骤暂且回避掉。
所有语法错误都会抛出一样的错误信息：

错误信息："ParsingError"

# SematicError 语义错误
当程序存在语义错误的时候，抛出此错误

## 需要一个常量表达式
当在声明变量同时初始化的时候一个的时候，用来初始化变量的表达式包含变量或函数调用的时候，抛出此错误

例子：
```
var
    i: integer = foo();
```

```
var
    i: integer = a; { a is a declared variable }
```

错误信息： "RequiredAConstantExpression"
## 表达式类型和变量类型不一致
在抛出错误的时候要给出变量的类型和表达式的类型

### 在声明初始化变量的时候
例子
```
var
    i: integer = 3.0;
```

```
var
    i: integer = Apple; { Apple has a enumerate type }
```

## 一次只能初始化一个变量
在声明初始化变量的时候，尝试一次性初始化多个变量
例子
```
var
    a, b, c: integer = 3;
```

错误信息："Only can initialize a variable every time"

## 变量未声明
使用的变量没有预先声明

错误信息要指明哪一个变量没有声明

例子
```
i := 3; { i didn't be declared yet }
```

错误信息： "variable %s undeclared", $variable_name

## 类型未声明
使用的类型没有声明

错误信息要指明哪个类型没有声明

例子
```
var
    i: Foo; { Type foo undeclared }
```

错误信息："type %s undeclared", $type_name
## 变量重复声明
使用的类型重复声明

错误信息： "variable %s has been declared before", $var_name

## 类型重复定义
使用的类型重复声明

错误信息："type %s has heen defined before", $type_name

## 非法的输入表达式
表达式的类型应当为一个变量，且不能为引用类型

例子：
```
read 3;

read @3;
```

read @3
错误信息： 
1. "Illegal Input Statement: the expression isn't a variable."
2. "Illegal Input Statement: the expression cannot be a reference."

## 非法表达式
在做表达式运算的时候操作数类型非法

例子：
1. 两个类型不一致且不能隐式转换的的表达式
    1. 当执行-，*，/的时候，两边的操作数
        1. 都是real
        2. 都是integer
        3. 一边时integer，一边时real
    2. 当执行 + 的时候，两边操作数：
        1. 都是real
        2. 都是integer
        3. 都是string
        4. 一边时string，一边时character
        5. 一边时integer，一边时real
    3. 当执行关系运算的时候，两边操作数必须是boolean类型
    4. 当执行左值运算（取引用，去引用，数组索引和记录取域）的时候，操作数必须都是左值表达式
    4. 当执行取引用的时候，操作数不能为引用类型
    5. 当执行去引用的时候，操作数必须时引用类型
    6. 当执行数组索引运算的时候，操作数必须是integer list，被操作数必须时array类型
    7. 当执行记录区域的时候，被操作数因该时record类型

当不满足以上的情况的时候，抛出非法表达式错误

错误信息： ”Illegal Expression"

## 过程/函数未定义

当调用函数/过程的时候，被调用的函数/过程没有定义，抛出此错误

错误信息：
1. "Function %s Undefined", $proc_name
2. "Procedure %s Undefined", $proc_name

## 函数/过程参数过多/过少
当函数被调用时，传入的实参数量过多或过少的时候，抛出此错误

该错误应当指出函数/过程需要的参数个数和错误的参数个数

错误信息：
1. "Too Many Arguments: %d Argument(s) Is Required, But %d Is Accepted", correct_args_num, actual_args_num
2. "Too Few Arguments: %d Argument(s) Is Required, But %d Is Accepted", correct_args_num, actual_args_num

/* 渣英语，需要更改说法 */

# 函数/过程参数类型不匹配
当函数被调用是，传入的实参类型和形参类型不一致的时候，抛出此错误

该错误应该指出函数的形参类型表和实参类型表的差异
错误信息：
1. """
    Function Arguments Type Unmatched.
    Expect Arguments Type: 
        $argument-type-list1
    Actual Arguments Type:
        $argument-type-list2
   """
2. """
    Procedure Arguments Type Unmatched.
    Expect Arguments Type: 
        $argument-type-list1
    Actual Arguments Type:
        $argument-type-list2
   """

$argument-type-list ::= ($argument-name ':' $type-name)+ | <empty>

## 函数返回值类型不正确
当函数返回值类型不正确的时候，抛出此错误

错误信息
 1. """
    Function Return Type Unmatched.
    Except: $type_name, Actual: $type_name
    """

## 函数缺乏返回值
当函数缺少返回类型的时候，抛出此错误
1. "Function: $func_name: Expect Return Value"

## 过程拥有返回值
当过程有返回值的时候，抛出此错误

错误信息
1. "Procedure $proc_name Has a Return Value"

## BREAK 和 CONTINUE 出现在循环之外
错误信息：
1. "Illegal Statement: This Break Statement Isn't Inside a Loop"
2. "Illeagal Satement: This Continue Statement Isn't Inside a Loop"


a[10].field

a.field[10]

Access (AccArray 10 (AccField a "field"))
Access (AccField (AccArray 10 a) "field")
## CASE OF 的用于判断的表达式应当是左值且类型为字面量或枚举类型

1. 错误信息 "Illegal Expression In Case Of Statement"
## 安全指针

1. 栈上变量的地址不能作为返回值
2. 如果存在函数存在一个边变量，他的类型是引用的引用( ref ref 'a)，那么栈上变量的地址不能赋值给去引用后的引用

例子：
```
procedure danger(b: ref ref integer);
var
    c: integer = 3;
begin
    !b := @c; { Illegal!!! }
end;

var
    a: ref int;
begin
    danger(@a);
    !a := 10; { danger!! }
end;
```

