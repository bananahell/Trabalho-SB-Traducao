section .data

num_aux db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
acc dd 0
label0 dd 0
label1 dd 0
label2 dd 1

section .text

global _start

_start:
  ; INPUT label0
  push dword label0
  call input_call
  add esp, 4

  ; COPY label0, label1
  mov eax, [label0]
  mov [label1], eax

  ; S_OUTPUT label1, label2
  push dword label1
  push dword [label2]
  call s_output_call
  add esp, 8

  ; STOP
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
  mov ecx, num_aux
  mov edx, 13
  int 80h
  mov ebx, dword [ebp + 8]
  mov ebp, 0
  mov ecx, num_aux
  mov edx, 10
 input_loop:
  cmp byte [ecx], 10
  je bye_input
  mov eax, ebp
  imul edx
  mov ebp, eax
  mov edx, [ecx]
  and edx, 255
  add ebp, edx
  mov edx, 10
  sub ebp, 48
  add ecx, 1
  jmp input_loop
 bye_input:
  mov dword [ebx], ebp
  pop ebp
  ret

s_output_call:
  push ebp
  mov ebp, esp
  mov eax, 4
  mov ebx, 1
  mov ecx, [ebp + 12]
  mov edx, [ebp + 8]
  int 80h
  pop ebp
  ret

s_input_call:
  push ebp
  mov ebp, esp
  mov eax, 3
  mov ebx, 0
  mov ecx, [ebp + 12]
  mov edx, [ebp + 8]
  int 80h
  pop ebp
  ret
