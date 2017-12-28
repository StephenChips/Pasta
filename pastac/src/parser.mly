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
%token NIL AS RAISE EXCEPTION FINALLY FALSE TRUE OR AND

%left EQUAL NOTEQ GT LT GTEQ LTEQ
%left OR SUB ADD
%left AND MUL DIV MOD
%nonassoc NOT UMINUS UADD
%left DOT LBRA 
%nonassoc GETREF %right DEREF

%start program 
%type <unit> program
%%

/* PROGRAM STRUCTURE */

program:
      program_declaration global_declaration_list compound_statement DOT {}
    | program_declaration compound_statement DOT {}
;

global_declaration_list:
      global_declaration global_declaration_list {}
    | global_declaration {}
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

array_item_type:
      ID {}
    | simple_type {}
    | reference_type {}
    ;

any_type:
      ID {} /* any type except simple type */
    | array_type {}
    | simple_type {}
    | reference_type {}
    ;
array_type: 
      ARRAY LBRA range_list RBRA OF array_item_type {}
    | ARRAY OF array_item_type {}
    ;

range_list:
      range COMMA range_list {}
    | range {}
    ;

range:
      range_value RANGE range_value {}
    | {}/* empty range */
    ;

range_value:
      ICONST {}
    | SUB ICONST {}
    ;

variable_declaration: 
    VAR var_declrs_or_inits {}
    ;

var_declrs_or_inits:
      declr_or_init SCOLON var_declrs_or_inits {}
    | declr_or_init SCOLON {}
    ;

declr_or_init:
    | var_declr {}
    | var_init {}
    ;

var_declr: 
    id_list COLON any_type {}
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
      variable_declaration {}
    | {}  /* no variable declared */
    ;

parameter_list:
      var_declr_list SCOLON var_init_list {}
    | var_declr_list {}
    | var_init_list {}
    | /* empty list */ {}
    ;

var_declr_list:
        var_declr_list SCOLON var_declr {}
    | var_declr  {}
    ;

var_init_list:
      var_init_list SCOLON var_init {}
    | var_init {}
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
    ;

exception_body:
      EXCEPTION {}
    | EXCEPTION fixed_field_list END {}
    ;

record_body:
      RECORD fixed_field_list END {}
    | RECORD fixed_field_list variant_field END {}
    | RECORD variant_field END {}
    ;

fixed_field_list:
      fixed_field_list fixed_field {}
    | fixed_field {}
    ;

fixed_field: 
    id_list COLON any_type SCOLON {}
    ;

variant_field:
      CASE ID COLON ordinal_type OF variant_case_part variant_else_part {}
    | CASE ID COLON ordinal_type OF variant_case_part {}
    ;

variant_case_part:
      variant_case {}
    | variant_case variant_case_part {}
    ;

variant_case:
      literal_or_enum_list COLON LPAR fixed_field_list RPAR {}
    ;

variant_else_part:
      ELSE fixed_field_list {}

enumerate_body: 
    ENUMERATE id_list END {}
    ;

reference_type: REF any_type {};

ordinal_type:
      INTEGER {}
    | STRING {}
    | CHARACTER {}
    | BOOLEAN {}
    | REAL {}
    | ID  {} /* enumerate type */
    ;

/* STATEMENTS */
statement: 
      stmt_m {}
    | stmt_u {}
    ;


stmt_m: /* balanced if-else-then */
      IF expression THEN stmt_m ELSE stmt_m {}
    | CASE expression OF case_part ELSE stmt_m {}
    | expression ASG expression {} /* assignments */
    | WHILE expression DO stmt_m {} 
    | BEGIN statement_list END {}
    | FOR ID ASG expression TO expression DO stmt_m {}
    | FOR ID ASG expression DOWNTO expression DO stmt_m {}
    | REPEAT stmt_m UNTIL expression {}
    | TRY stmt_m EXCEPT exception_part stmt_m {}
    | TRY stmt_m FINALLY stmt_m {}
    | TRY stmt_m EXCEPT exception_part DO stmt_m FINALLY stmt_m {}
    | RAISE ID LPAR expression_list RPAR  {}
    | RAISE ID {}
    | WRITE expression_list {}
    | READ expression_list {}
    | DEC expression {}
    | INC expression {}
    | BREAK {}
    | CONTINUE {}
    | ID LPAR expression_list RPAR {} /* function call */
    | /* empty statement */ {}
    ;

stmt_u:
      IF expression THEN statement {}
    | IF expression THEN stmt_m ELSE stmt_u {}
    | WHILE expression DO stmt_u {}
    | FOR ID ASG expression TO expression DO stmt_u {}
    | FOR ID ASG expression DOWNTO expression DO stmt_u {}
    | CASE expression OF case_part {}
    | CASE expression OF case_part ELSE stmt_u {}
    | REPEAT stmt_u UNTIL expression {}
    | TRY stmt_u EXCEPT exception_part DO stmt_u {}
    | TRY stmt_u FINALLY stmt_u END {}
    | TRY stmt_u EXCEPT exception_part stmt_u FINALLY stmt_u {}
    ;

/* sematic check: literals in list should have same type */
case_part:
      literal_or_enum_list COLON statement SCOLON case_part {}
    | literal_or_enum_list COLON statement SCOLON {}
    ;

/* Two rules don't currently use in gramma */
/* 
stmt_m_list: 
      stmt_m {}
    | stmt_m_list SCOLON stmt_m {}
    ;

stmt_u_list:
      stmt_u {}
    | stmt_u_list SCOLON stmt_u {};
    ;
*/

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

exception_part: 
      id_list {}
    | id_list AS ID {}
    ;

id_list: 
      id_list COMMA ID {}
    | ID {}
    ;

compound_statement: BEGIN statement_list END {};

statement_list: 
      statement {}
    | statement_list SCOLON statement {}
    ;

/* EXPRESSION */

expression:
    | access {}
    | value  {}
    ;

value:  
      value_not_access {}
    | expression GT   expression {}
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
    | literal {}
    | NIL {}
    | ID LPAR expression_list RPAR {} /* function call */
    ;

value_not_access:
    | LPAR value_not_access RPAR {}
    | GETREF access {}
    ;

access:
      ID {}
    | LPAR access RPAR {}
    | DEREF access {}
    | DEREF value_not_access {}
    | access LBRA expression_list RBRA {}
    | access DOT ID {}
    ;

expression_list:  
      expression {}
    | expression COMMA expression_list {}
    | {} /* empty list */
    ;
