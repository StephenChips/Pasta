program Try;

type 
    myrecord = record 
        case b: boolean of
	    true: (
	        car: integer;
		wheel: integer;
		)
	    false: (
	        has: real;
		)
    end;

begin

end.
