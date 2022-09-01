section .data
acc dd 0

label1 dd 1
label0 dd 0, 0

section .text

global _start

_start:

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
