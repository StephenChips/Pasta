%token <int> ICONST
%token <float> RCONST
%token <bool> BCONST
%token <char> CCONST
%token <string> SCONST
%token <string> ID
%token ADD SUB MUL DIV MOD
%token ASG
%token LPAR RPAR LBRA RBRA
%token GT LT EQUAL NOTEQ GTEQ LTEQ
%token GETREF DEREF
%token COLON SCOLON COMMA
%token DOT
%token RANGE

%token PROGRAM FUNCTION PROCEDURE BEGIN END FOR WHILE REPEAT
%token INTEGER THEN UNTIL IF ELSE INC DEC DO REF REAL STRING
%token BOOLEAN CHARACTER ARRAY TO DOWNTO VAR TYPE ENUMERATE 
%token RECORD WRITE READ CASE OF BREAK CONTINUE TRY EXCEPT 
%token NIL AS RAISE EXCEPTION FINALLY FALSE TRUE

%left EQUAL NOTEQ GT LT GTEQ LTEQ
%left OR SUB ADD
%left AND MUL DIV MOD
%nonassoc GETREF NOT UMINUS UADD
%right DEREF
%left DOT INDEXING FUNCCALL

%start program 
%type <unit> program
%%

/* PROGRAM STRUCTURE */

program:
    program_declaration global_declaration_list compound_statement DOT { print_endline "Parse Successfully.";; }
;

global_declaration_list:
      global_declaration SCOLON global_declaration_list {}
    | global_declaration SCOLON {}
    ;

global_declaration :
      variable_declaration {}
    | procedure_definition {}
    | function_definition {}
    | type_definition {}
    ;

program_declaration : 
    PROGRAM ID SCOLON {}
    ;

/* DECLARATIOINS AND DEFINITIONS */

simple_type:
      INTEGER {}
    | REAL   {}
    | STRING {}
    | CHARACTER {}
    | BOOLEAN {}
    ;

any_named_type:
      ID {}
    | simple_type {}
    ;

ordinal_type:
      INTEGER {}
    | REAL {}
    | CHARACTER {}
    | BOOLEAN {}
    ;

any_type:
      ID {} /* any type except simple type */
    | array_type {}
    | simple_type {}
    ;
array_type: 
    ARRAY LBRA range_list RBRA OF any_named_type {}
    ;

range_list:
      range COMMA range_list {}
    | range {}
    ;

range:
      ICONST RANGE ICONST {}
    | {}/* empty range */
    ;

variable_declaration: 
    VAR var_declrs_or_inits {}
    ;

var_declrs_or_inits:
    | declr_or_init SCOLON var_declrs_or_inits {}
    | declr_or_init {}
    ;

declr_or_init:
    | var_declr {}
    | var_init {}
    ;
var_declr_list: 
      var_declr_list SCOLON var_declr {}
    | var_declr {}
    ;

var_declr: 
    id_list ID COLON any_type {}
    ;

var_init_list: 
      var_init_list SCOLON var_init{}
    | var_init {}
    ;

/* the result of expression should be a constant */
/* the variable should has simple type */

/* use attribute gramma to limit the number of variable to be init. */
var_init: 
      id_list COLON simple_type EQUAL expression  {}
    | id_list COLON ID EQUAL ID {} /* enumerate type */
    ;

procedure_definition: 
    PROCEDURE ID LPAR parameter_list RPAR SCOLON local_var_declrs compound_statement SCOLON {}
    ;

function_definition: 
    FUNCTION ID LPAR parameter_list RPAR COLON any_type SCOLON local_var_declrs compound_statement SCOLON {}
    ;

local_var_declrs:
      var_declrs_or_inits {}
    | {}  /* no variable declared */
    ;

parameter_list:
      var_declr_list SCOLON var_init_list {}
    | var_declr_list {}
    | var_init_list {}
    | /* empty list */ {}
    ;

type_definition: 
    TYPE type_definition_list {}
    ;

type_definition_list: 
      ID EQUAL type_body SCOLON type_definition_list {}
    | ID EQUAL type_body SCOLON {}
    ;

type_body:
      any_type {}  /* alias a type */
    | record_body {}
    | enumerate_body {}
    | exception_body {}
    | reference_body {}
    ;

record_body:
      RECORD fixed_field_list variant_part END {}
    | RECORD variant_part END {}
    | RECORD fixed_field_list END {}
    ;

fixed_field_list:
      fixed_field SCOLON fixed_field_list {}
    | fixed_field {}
    ;

fixed_field: 
    id_list COLON any_type {}
    ;

variant_part:
      CASE literal_or_enum_list COLON ordinal_type OF fixed_field_list variant_else_part END {}
    | CASE literal_or_enum_list COLON ordinal_type OF fixed_field_list END {}
    ;

variant_else_part: 
    ELSE fixed_field_list  {}
    ;

enumerate_body: 
    ENUMERATE id_list END {}
    ;

exception_body:
      EXCEPTION {}
    | EXCEPTION fixed_field_list END {}
    ;

reference_body:
    REF any_type {}
    ;


/* STATEMENTS */
statement: 
      assignment_statement {}
    | procedure_statement {}
    | if_then_else_statement {}
    | case_of_statement {}
    | while_do_statement {}
    | for_statement {}
    | repeat_until_statement {}
    | try_except_statement {}
    | compound_statement {}
    | raise_statement {}
    | write_statement {}
    | read_statement {}
    | inc_statement {}
    | dec_statement {}
    | BREAK {}
    | CONTINUE {}
    | {} /* empty statement */
    ;

assignment_statement:
    ID ASG expression {}
    ;

procedure_statement: 
    ID LPAR expression_list RPAR {}
    ;

compound_statement: 
    BEGIN statement_list END {}
    ;

case_of_statement: 
      CASE expression OF case_part ELSE statement END {}
    | CASE expression OF case_part END {}
    ;

/* sematic check: literals in list should have same type */
case_part:  
      literal_or_enum_list COLON statement SCOLON case_part {}
    | literal_or_enum_list COLON statement SCOLON {}
    ;

literal_or_enum:
      literal {}
    | ID {}
    ;

literal: 
      ICONST {}
    | RCONST {}
    | BCONST {}
    | SCONST {}
    | CCONST {}
    ;

literal_or_enum_list: 
      literal_or_enum literal_or_enum_list {}
    | literal_or_enum {}
    ;

if_then_else_statement: 
      IF expression THEN statement {}
    | IF expression THEN statement ELSE statement {}
    ;

while_do_statement:  
    WHILE expression DO statement {}
    ;

for_statement: 
      FOR ID ASG expression TO expression DO statement {}
    | FOR ID ASG expression DOWNTO expression DO statement {}
    ;

repeat_until_statement:  
      REPEAT statement UNTIL expression {}
    ;

write_statement: 
    WRITE expression_list {}
    ;

read_statement: 
    READ expression_list {}
    ;

inc_statement: 
    INC expression {}
    ;

dec_statement: 
    DEC expression {}
    ;

raise_statement: 
    RAISE ID LPAR expression_list RPAR  {}
    ;

try_except_statement: 
      TRY statement_list EXCEPT exception_part DO statement_list END {}
    | TRY statement_list FINALLY statement_list END {}
    | TRY statement_list EXCEPT exception_part DO statement_list FINALLY statement_list END {}
    ;

exception_part: 
      id_list {}
    | id_list AS ID {}
    ;

id_list: 
      id_list COMMA ID {}
    | ID {}
    ;

statement_list: 
      statement {}
    | statement_list SCOLON statement {}
    ;

/* EXPRESSION */

expression:  
      expression GT   expression {}
    | expression LT   expression {}
    | expression EQUAL expression {}
    | expression GTEQ expression {}
    | expression LTEQ expression {}
    | expression NOTEQ expression {}
    | expression ADD expression   {}
    | expression SUB expression   {}
    | expression OR  expression   {}
    | expression MUL expression {}
    | expression DIV expression {}
    | expression MOD expression {}
    | expression AND expression {}
    | NOT expression {}
    | GETREF expression {}   /* get object's address */
    | DEREF expression  {}   /* access refered object's value */
    | TRUE {}
    | FALSE {}
    | REAL {}
    | INTEGER {}
    | ID {}
    | NIL {}
    | LPAR expression RPAR {}
    | ID LBRA expression_list RBRA %prec INDEXING {} /* array indexing, should has one expression in the list at less */
    | ID LPAR expression_list RPAR %prec FUNCCALL {} /* function call */
    | ID DOT field {}                 /* access record or exception instance's field */
    ;

field:  
      ID {}
    | ID DOT field {}
    ;

expression_list:  
    expression {}
    | expression COMMA expression_list {}
    | {} /* empty list */
    ;
