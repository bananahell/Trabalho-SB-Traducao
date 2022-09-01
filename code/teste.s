section .data
acc dd 0

label0 dd 1

section .text

global _start

_start:
push dword label0
push dword 1
call output_call
add esp, 8

push dword label0
push dword 1
call output_call
add esp, 8

push dword label0
push dword 1
call input_call
add esp, 8

push dword label0
push dword 1
call output_call
add esp, 8

mov eax, 1
mov ebx, 0
int 80h

output_call:
  push ebp
  mov ebp, esp
  mov eax, 4
  mov ebx, 1
  mov ecx, [ebp + 12]
  mov edx, [ebp + 8]
  int 80h
  pop ebp
  ret

input_call:
  push ebp
  mov ebp, esp
  mov eax, 3
  mov ebx, 0
  mov ecx, [ebp + 12]
  mov edx, [ebp + 8]
  int 80h
  pop ebp
  ret
