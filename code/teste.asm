section .data

msg dd 0a0d6141h
msg_size dd 4

section .text

global _start

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

_start:
  push dword msg
  push dword [msg_size]
  call output_call
  add esp, 8

  push dword msg
  push dword [msg_size]
  call output_call
  add esp, 8

  push dword msg
  push dword [msg_size]
  call input_call
  add esp, 8

  push dword msg
  push dword [msg_size]
  call output_call
  add esp, 8

  mov eax, 1
  mov ebx, 0
  int 80h
