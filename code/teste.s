section .data

acc dd 0
label0 dd 0
label2 dd 0
label1 dd 1

section .text

global _start

_start:
  ; S_INPUT label0, label1
  push dword label0
  push dword [label1]
  call s_input_call
  add esp, 8

  ; LOAD label0
  mov eax, [label0]
  mov [acc], eax

  ; ADD label1
  mov eax, [label1]
  add [acc], eax

  ; STORE label2
  mov eax, [acc]
  mov [label2], eax

  ; COPY label2, label0
  mov eax, [label2]
  mov [label0], eax

  ; S_OUTPUT label0, label1
  push dword label0
  push dword [label1]
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
  mov ecx, [ebp + 12]
  mov edx, [ebp + 8]
  int 80h
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
