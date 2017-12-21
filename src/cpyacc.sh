ocamllex lexer.mll
ocamlyacc -v parser.mly
ocamlc -c lexer.ml
ocamlc -c parser.ml
ocamlc -c pastac.ml
ocamlc -o pastac lexer.cmo parser.cmo pastac.cmo

echo done
