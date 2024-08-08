This is example for using "snapshot fuzzing mode" in AFL++
Just make simple target for fuzzing using QEMU & snapshot mode in AFL++

Use Command

1. QEMU & AFL++ build

```
# AFL++를 클론하고 빌드
git clone https://github.com/AFLplusplus/AFLplusplus.git
cd AFLplusplus
make distrib

# QEMU 지원 빌드
cd qemu_mode
./build_qemu_support.sh

```

2. Setting Environment Variable

```
# 스냅샷 모드 설정 (예제 주소 사용)
export AFL_QEMU_SNAPSHOT=0x00005555555554b0
export AFL_QEMU_PERSISTENT_GPR=1
export AFL_QEMU_PERSISTENT_MEM=1
export AFL_QEMU_PERSISTENT_EXITS=1
export AFL_DEBUG=1

```

3. compile target source code
```
gcc -o snapshot_fuzzing_example snapshot_fuzzing_example.c

```

4. fuzzing
```
afl-fuzz -Q -i input_dir -o output_dir -- ./snapshot_fuzzing_example
```