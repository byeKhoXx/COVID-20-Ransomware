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

```sh
> g++ -c PE.cpp -std=c++17
> g++ -c infectPE.cpp -std=c++17
> g++ -o infectPE.exe infectPE.o
> .\infectPE.exe .\executable_to_infect.exe .\out.exe

> g++ -c processCreator.cpp
> g++ -o procesCreator.exe processCreator.o
> .\processCreator.exe executable_to_execute.exe

> g++ -c shellcodeExecuter.cpp
> g++ -o shellcodeExecuter.exe shellcodeExecuter.o
> .\shellcodeExecuter.exe
```
