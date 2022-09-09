section .data

acc dd 0
num_aux db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
label0 dd 0
label2 dd 0
label1 dd 1

section .text

global _start

_start:
  ; INPUT label0
  push dword label0
  push dword num_aux
  call input_call
  add esp, 8

  ; OUTPUT label0
  push dword label0
  push dword [label1]
  call s_output_call
  add esp, 8

  ; STOP
  mov eax, 1
  mov ebx, 0
  int 80h

; TODO tentei a opcao nuclear e coloquei mov edx, 0 em todo canto
output_call:
  push ebp
  mov ebp, 10
  mov eax, [esp + 8]
  mov ebx, 0
  mov esi, num_aux
 get_num_size:
  mov edx, 0
  idiv ebp
  mov edx, 0
  add ebx, 1
  cmp eax, 0
  jne get_num_size
 get_num_string:
  cmp ebx, 0
  je bye_output
  mov eax, [esp + 8]
  mov ecx, ebx
 get_one_char:
  cmp ecx, 1
  je resume_one_char
  mov edx, 0
  idiv ebp
  mov edx, 0
  sub ecx, 1
  jmp get_one_char
 resume_one_char:
  mov ecx, eax
  mov edx, 0
  idiv ebp
  mov edx, 0
  imul ebp
  mov edx, 0
  sub ecx, eax
  add ecx, 48
  mov byte [esi], cl
  add esi, 1
  sub ebx, 1
  jmp get_num_string
 bye_output:
  ;TODO se eu tiro as proximas 3 linhas, da um ruim estranho
  mov byte [esi + 2], 10
  mov byte [esi + 1], 13
  add esi, 2
  sub esi, num_aux
  mov eax, 4
  mov ebx, 1
  mov ecx, num_aux
  mov edx, esi
  int 80h
  pop ebp
  ret

input_call:
  push ebp
  mov ebp, esp
  mov eax, 3
  mov ebx, 0
  mov ecx, [ebp + 8]
  mov edx, 13
  int 80h
  mov ebx, dword [ebp + 12]
  mov ebp, 0
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
