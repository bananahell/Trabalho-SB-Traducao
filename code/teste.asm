section .data

acc dd 0
ZERO_ASCII equ 48
CR_ASCII equ 13
LF_ASCII equ 10
MINUS_ASCII equ 45
label0 dd 0

section .text

global _start

_start:

push dword label0
push dword 13
call input_call
add esp, 8

push dword label0
push dword 13
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
  mov eax, 0 ; value
  mov ebx, 0 ; counter
  mov edx, [ecx] ; temp
  cmp edx, MINUS_ASCII
  je neg_input_loop
input_loop:
  mov edx, ecx
  add edx, ebx
  mov edx, [edx]
  cmp edx, LF_ASCII
  je bye_input
  mov edx, 10
  mul edx
  mov edx, ecx
  add edx, ebx
  mov edx, [edx]
  sub edx, ZERO_ASCII
  add eax, edx
  add ebx, 1
  jmp input_loop
neg_input_loop:
  mov edx, ecx
  add edx, ebx
  mov edx, [edx]
  cmp edx, LF_ASCII
  je bye_input
  mov edx, 10
  mul edx
  mov edx, ecx
  add edx, ebx
  mov edx, [edx]
  sub edx, ZERO_ASCII
  sub eax, edx
  add ebx, 1
  jmp neg_input_loop
bye_input:
  mov [ecx], eax
  pop ebp
  ret
