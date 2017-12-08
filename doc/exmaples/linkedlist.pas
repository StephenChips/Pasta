program StackExample;
type StackItem = record
    item: integer;
    next: ref LinkedListItem;
end;

type StackException = excpection
    msg: string;
end.


procedure Push(StackRef: ref StackItem; number: integer);
var
    NewItem: StackItem;
begin
    NewItem.next := @StackRef;
    NewItem.item := number;
    StackRef := @newItem; 
end;

procedure Pop(StackRef: ref StackItem);
var
    e: StackExcpetion;
begin
    if StackRef <> nil then
        StackRef := !stackRef.next;
    else
        begin
            e.msg = "empty.stack";
            raise e;
        end;
end.
