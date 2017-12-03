:SetUp
    @echo off

:Mounting	
echo -{ ...
echo -{ head
	nasm head.s -felf -o head.o

:Compiling	
echo -{ ...
echo -{ Compiling TASKMAN.BIN.
    gcc -c crt0.c -o crt0.o  
	gcc -c main.c -o main.o  
	
:Objects	
set myObjects=head.o ^
crt0.o ^
main.o ^
api.o ^
stdio.o ^
stdlib.o ^
string.o 





:Linking
echo -{ ...
echo -{ Linking objects ... 
    ld -T link.ld -o TASKMAN.BIN %myObjects% -Map app_map.s
   
    rem
    rem :Cleaning   
    rem      N�o pode deletar os objetos.
    rem
   
:Moving   
    move TASKMAN.BIN bin\TASKMAN.BIN
    
:End	
echo End.
	pause
	
	
