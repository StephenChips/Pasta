# TODO LIST

1. rename variables whose meaning is inaccruate. 
    e.g. `cst_pool_num` should remane to `cst_num` or `cst_count`, since it represent the count of variables in RawcodeGen Object.

2. Decouple the "load config file and rawcode file" part from VM initializing functions to a independent object. 
   To initialize a VM, pass a config struct to loading function instead of opening and reading config file directly in the loadin function.

c
