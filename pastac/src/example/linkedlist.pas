program StackExample;


type 
    StackItem = record
        item: integer;
        next: ref LinkedListItem;
    end;

    StackException = exception
        msg: string;
    end;

procedure Push(StackRef: ref StackItem; number: integer);
var
    NewItemRef: ref StackItem;
begin
    newItemRef := new(NewItem);
    !NewItemRef.next := StackRef;
    NewItem.item := number;
    StackRef := newItem ;
end;


procedure Pop(StackRef: ref StackItem);
begin
    if StackRef <> nil then
	StackRef := !StackRef.next
    else
        raise StackExcpetion("empty stack");
end;

begin
end.
