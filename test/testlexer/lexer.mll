{
open Parser;;
exception Eof;;
exception LexingError of string;;

let keyword s =
    match s with
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
    | "true" -> BCONST true
    | "false" -> BCONST false
    | _ -> ID s

let escape (s: string): char = 
    match s with
    | "\\\\" -> '\\'
    | "\\\"" -> '"'
    | "\\\'" -> '\''
    | "\\a" -> '\007'
    | "\\b" -> '\008'
    | "\\t" -> '\t'
    | "\\n" -> '\n'
    | "\\v" -> '\011'
    | "\\f" -> '\012'
    | "\\r" -> '\r'
    | _ -> raise (LexingError "unknown Pasta escape.")

;;

let convert_to_char str = 
  if str.[0] = '\\' then
    if str.[1] >= '0' && str.[1] <= '9' then
      let code = int_of_string (String.sub str 1 3) in
      Char.chr code 
    else
      escape str
  else
    str.[0]
;;
}

let digits = ['0'-'9']+

rule token = parse
    | [' ' '\t' '\r']  { token lexbuf }
    | '\n'             { Lexing.new_line lexbuf; token lexbuf }
    | '{'              { ignore_comment lexbuf; token lexbuf  }
    | ['_' 'A'-'Z' 'a'-'z']['_' 'A'-'Z' 'a'-'z' '0'-'9']* as s 
                       { keyword s }

    | ['0'-'9']+ as s      { ICONST (int_of_string s) }
    | (['0'-'9']+ '.' ['0'-'9']+) as s 
                       { RCONST (float_of_string s) }

    | '"'              { SCONST (match_string [] lexbuf) }
    | '\''             { CCONST (match_character lexbuf) }
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
    | _ { raise (LexingError "LexingError: Illegal token") }

and match_string charlist = parse
    | '"'
        { String.concat "" (List.map (String.make 1) (List.rev charlist)) }
    | '\\' ['a' 'b' 't' 'n' 'v' 'f' 'r'] as s 
        { match_string ((escape s) :: charlist) lexbuf }
    | '\\' '"'
        { match_string ('"' :: charlist) lexbuf }
    | "\\\\" { match_string ('\\' :: charlist) lexbuf }
    | (eof | '\026')  
        { raise (LexingError "LexingError: Unterminated String") }
    | _ as s 
         { match_string (s :: charlist) lexbuf }

and match_character = parse
    | ('\\' ['a' 'b' 't' 'n' 'v' 'f' 'r' '\'' '"' '\\']) as s '\'' { escape s }
    | ('\\' ['0'-'9']['0'-'9']['0'-'9']) as s '\'' { Char.chr (int_of_string (String.sub s 1 3)) }
    | _ as ch '\'' { ch }
    | _ { raise (LexingError "LexingError: Illegal Character") }

and ignore_comment = parse
    | "{" { ignore_comment lexbuf; ignore_comment lexbuf }
    | "}" { () }
    | (eof | '\026') { raise (LexingError "LexingError: Undetermined Comment") }
    | _   { ignore_comment lexbuf } 
