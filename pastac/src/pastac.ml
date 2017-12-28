(* print_endline (string_of_int (Array.length Sys.argv));; *)
if (Array.length Sys.argv) = 2 then
  let ic = open_in Sys.argv.(1) in
  try
    let lexbuf = Lexing.from_channel ic in
    let ast = Parser.program Lexer.token lexbuf in
    begin 
      print_endline "Parsing Finished Successfully";
      ast
    end
  with 
  | Lexer.LexingError msg -> print_endline msg
else
  print_endline "Usage: pastac <file>"
;;

