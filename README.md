# MALW-MCYBERS
## Infection
For the infection part we decided to use *New section* technique. This consits in adding a new section to a *.exe* file.

For this, we have the following files:
- main.cpp
- PE.cpp
- PE.h

However, an error is appearing during compilation. **WARNING:** Compilation should be done in Windows environment.

```sh
C:\Users\IEUser\Documents\CodeResizing>g++ -c PE.cpp
PE.cpp: In function 'PE::PE_FILE PE::ParsePE(const char*)':
PE.cpp:50:9: error: 'memcpy_s' was not declared in this scope; did you mean 'memcpy'?
   50 |         memcpy_s(&pefile.ids, sizeof(IMAGE_DOS_HEADER), PE, sizeof(IMAGE_DOS_HEADER));
      |         ^~~~~~~~
      |         memcpy
PE.cpp: In function 'void PE::WriteBinary(PE::PE_FILE, std::string, size_t)':
PE.cpp:96:9: error: 'memcpy_s' was not declared in this scope; did you mean 'memcpy'?
   96 |         memcpy_s(r_ch, pefile.size_ids, &pefile.ids, pefile.size_ids);
      |         ^~~~~~~~
      |         memcpy

```

A possible solution may be change *memcpy_s* by *memcpy*, but if you do it, apper more errors. In order to avoid the new problems when the change is done, probably addapting the code may be enough (visit this [link](https://en.cppreference.com/w/c/string/byte/memcpy) to get more info about it). Feel free to make the changes you want!

## Shell code 
Create process + encrypt(Albert) + ofuscation(Walid)
https://docs.microsoft.com/en-us/windows/win32/procthread/creating-processes
https://github.com/NytroRST/ShellcodeCompiler
