### using command list


TargetProgram Compile
```
afl-gcc-fast -o afl_file_input afl_file_input.c
```

using Custom Mutator (afl_custom_fuzz_send)
```
cc -O3 -shared -fPIC -o custom_send.so -I /AFLplusplus/include afl_custom_mutator.c
```

```
AFL_CUSTOM_MUTATOR_LIBRARY=/home/jun20/jun/etc/AFLToyDriver/afl_custom_mutator_test/custom_send.so AFL_DEBUG=1 \
afl-fuzz -i in -o out -- ./afl_file_input ./foo
```

using Custom Mutator (afl_custom_post_process & afl_custom_fuzz)

```
cc -O3 -shared -fPIC -o example.so -I /AFLplusplus/include example.c
```
```
AFL_CUSTOM_MUTATOR_LIBRARY=/home/jun20/jun/etc/AFLToyDriver/afl_custom_mutator_test/example.so AFL_DEBUG=1 \
afl-fuzz -i in -o out2 -- ./afl_file_input @@
```