{
open Parser;;
exception Eof;;
exception LexingError of string;;

let keyword s = function
    | "program" -> PROGRAM
    | "function" -> FUNCTION
    | "procedure" -> PROCEDURE
    | "begin" -> BEGIN
    | "end" -> END
    | "for" -> FOR
    | "while" -> WHILE
    | "repeat" -> REPEAT
    | "integer" -> INTEGER
    | "then" -> THEN
    | "until" -> UNTIL
    | "if" -> IF
    | "else" -> ELSE
    | "inc" -> INC
    | "dec" -> DEC
    | "do" -> DO
    | "ref" -> REF
    | "real" -> REAL
    | "string" -> STRING
    | "boolean" -> BOOLEAN
    | "character" ->CHARACTER
    | "array" -> ARRAY
    | "to" -> TO
    | "downto" -> DOWNTO
    | "var" -> VAR
    | "type" -> TYPE
    | "enumerate" -> ENUMERATE
    | "record" -> RECORD
    | "write" -> WRITE
    | "read" -> READ
    | "case" -> CASE
    | "of" -> OF 
    | "break" -> BREAK
    | "continue" -> CONTINUE
    | "try" -> TRY
    | "except" -> EXCEPT
    | "nil" -> NIL
    | "as" -> AS
    | "raise" -> RAISE
    | "or" -> OR
    | "and" -> AND
    | _ -> ID s

let escape s = function
    | "\\\\" -> '\\'
    | "\\\"" -> '"'
    | "\\a" -> '\007'
    | "\\b" -> '\008'
    | "\\t" -> '\t'
    | "\\n" -> '\n'
    | "\\v" -> '\011'
    | "\\f" -> '\012'
    | "\\r" -> '\r'
    | _ -> raise (LexingError "unknown Pasta escape.")

;;
}

let digits = ['0'-'9']+

rule Token = parse
    | [' ' '\t' '\r']  { Token lexbuf }
    | '\n'             { Lexing.new_line lexbuf; Token lexbuf }
    | "{"              { IgnoreComment lexbuf; Token lexbuf  }
    | ['_' 'A'-'Z' 'a'-'z']['_' 'A'-'Z' 'a'-'z' '0'-'9']* as s 
                       { keyword s }

    | digits as s      { ICONST (string_of_int s) }
    | digits '.' digits ('+'|'-')? digits as s 
                       { RCONST (string_of_float s) }

    | "true"           { BCONST true }
    | "false"          { BCONST false }
    | '"'              { SCONST (MatchString [] lexbuf) }
    | '\''             { CCONST (MatchCharacter lexbuf) }
    | '+'              { ADD }
    | '-'              { SUB }
    | '*'              { MUL }
    | '/'              { DIV }
    | '%'              { MOD }
    | ":="             { ASG }
    | '('              { LPAR }
    | ')'              { RPAR }
    | '['              { LBRA }
    | ']'              { RBRA }
    | '>'              { GT }
    | '<'              { LT }
    | '='              { EQUAL }
    | ("<>" | "!=")    { NOTEQ }
    | ">="             { GTEQ }
    | "<="             { LTEQ }
    | '@'              { GETREF }
    | '!'              { DEREF }
    | ':'              { COLON }
    | ';'              { SCOLON }
    | ','              { COMMA }
    | '.'              { DOT }
    | ".."             { RANGE }
    | _ { raise (LexingError "Unknown token") }

and MatchString charlist = parse
    | '"'
        { String.concat "" (List.map (String.make 1) (List.rev charlist)) }
    | '\\' ['a' 'b' 't' 'n' 'v' 'f' 'r'] as s 
        { (escape s) :: charlist }
    | "\\\""
        { '"' :: charlist }
    | ['\000'-'\031' '\128'-'\255'] 
        { raise (LexingError "Illegal character") }
    | (eof | '\026')  
        { raise (LexingError "Unterminated string") }
    | _ as s 
         { s :: charlist }

and MatchCharacter charlist lexbuf =  parse
    | ('\\' ['a' 'b' 't' 'n' 'v' 'f' 'r']) as s '\'' 
        { escape s }
    | '\\' (['0'-'9']['0'-'9']['0'-'9']) as s '\'' { 
            let code = int_of_string s in
                if code >= 31 && code <= 127 then 
                    Char.chr code
                else 
                    raise (LexiingError "Illegal Character")
        }
    | _ as s '\'' 
        { s.[0] }
    | _ 
        { raise (LexingError "Unknown character") }

and IgnoreComment lexbuf = parse
    | "{" { IgnoreComment lexbuf; IgnoreComment lexbuf }
    | "}" { () }
    | (eof | '\026') { raise (LexingError "Unterminated Comment") }
    | _   { IgnoreComment lexbuf } 
