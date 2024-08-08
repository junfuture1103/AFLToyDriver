Use Command

```
afl-clang-fast -o fuzz_target target.c driver.c
afl-fuzz -i in -o out -s 123 ./fuzz_target @@
```