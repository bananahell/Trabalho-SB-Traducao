#include "translator.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void translateCode(char* fileNameIn) {
  string line;
  vector<string> lineVector;
  lineVector.clear();
  map<int, string> labelTable;
  map<int, string> jmpLabelTable;
  int isInstruction = 0;
  bool isInSectionText = true;
  bool isJump = false;

  ifstream inputFile(fileNameIn, ifstream::in);
  if (inputFile.good() == false) {
    cout << "Arquivo de entrada " << fileNameIn << " nao pode ser lido!"
         << endl;
    return;
  }

  while (getline(inputFile, line)) {
    istringstream stringStream(line);
    string token;
    while (stringStream >> token) {
      lineVector.push_back(token);
      if (isInSectionText == true) {
        if (isInstruction == 0) {
          int instruction = getOpcode(token);
          if (instruction == COPY_OP || instruction == S_INPUT_OP ||
              instruction == S_OUTPUT_OP) {
            isInstruction += 2;
            isJump = false;
          } else if (instruction == STOP_OP) {
            isInSectionText = false;
            isJump = false;
          } else if (instruction > 0) {
            isInstruction += 1;
            if (instruction == JMP_OP || instruction == JMPN_OP ||
                instruction == JMPP_OP || instruction == JMPZ_OP) {
              isJump = true;
            } else {
              isJump = false;
            }
          } else {
            cout << "????????" << endl;
          }
        } else {
          isInstruction--;
          if (isJump == true) {
            jmpLabelTable =
                insertLabelInTable(jmpLabelTable, stoi(token),
                                   labelTable.size() + jmpLabelTable.size());
          } else {
            labelTable =
                insertLabelInTable(labelTable, stoi(token),
                                   labelTable.size() + jmpLabelTable.size());
          }
        }
      }
    }
  }

  inputFile.close();

  map<int, string>::iterator it;
  cout << "Label Table" << endl;
  for (it = labelTable.begin(); it != labelTable.end(); ++it) {
    std::cout << it->first << ':' << it->second << std::endl;
  }
  cout << "Jump Label Table" << endl;
  for (it = jmpLabelTable.begin(); it != jmpLabelTable.end(); ++it) {
    std::cout << it->first << ':' << it->second << std::endl;
  }

  string extension = fileNameIn;
  extension = extension.substr(extension.find("."));
  string fileNameOut = fileNameIn;
  fileNameOut.replace(fileNameOut.find(extension), extension.length(), ".s");
  ofstream outputFile(fileNameOut, ifstream::out);

  map<int, string>::iterator tokenFound;

  outputFile << "section .data" << endl;
  outputFile << endl;
  outputFile << "num_aux db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0" << endl;
  outputFile << "acc dd 0";
  for (unsigned i = labelTable.begin()->first; i < lineVector.size(); i++) {
    tokenFound = labelTable.find(i);
    if (tokenFound != labelTable.end()) {
      outputFile << endl;
      outputFile << tokenFound->second << " dd " << lineVector.at(i);
    } else {
      outputFile << ", " << lineVector.at(i);
    }
  }

  outputFile << endl;
  outputFile << endl;
  outputFile << "section .text" << endl;
  outputFile << endl;
  outputFile << "global _start" << endl;
  outputFile << endl;
  outputFile << "_start:" << endl;

  isInstruction = 0;
  isInSectionText = true;
  string temp;
  for (unsigned i = 0; i < lineVector.size(); i++) {
    if (isInSectionText == true) {
      tokenFound = jmpLabelTable.find(i);
      if (tokenFound != jmpLabelTable.end()) {
        outputFile << " " << tokenFound->second << ":" << endl;
      }
      switch (stoi(lineVector.at(i))) {
        case ADD_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find an add's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; ADD " << tokenFound->second << endl;
          outputFile << "  mov eax, [" << tokenFound->second << "]" << endl;
          outputFile << "  add [acc], eax" << endl;
          outputFile << endl;
          i++;
          break;
        case SUB_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a sub's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; SUB " << tokenFound->second << endl;
          outputFile << "  mov eax, [" << tokenFound->second << "]" << endl;
          outputFile << "  sub [acc], eax" << endl;
          outputFile << endl;
          i++;
          break;
        case MULT_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a mult's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; MULT " << tokenFound->second << endl;
          outputFile << "  mov eax, [acc]" << endl;
          outputFile << "  mov ebx, [" << tokenFound->second << "]" << endl;
          outputFile << "  imul ebx" << endl;
          outputFile << "  mov [acc], eax" << endl;
          outputFile << endl;
          i++;
          break;
        case DIV_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a div's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; DIV " << tokenFound->second << endl;
          outputFile << "  mov eax, [acc]" << endl;
          outputFile << "  mov edx, 0" << endl;
          outputFile << "  mov ebx, [" << tokenFound->second << "]" << endl;
          outputFile << "  idiv ebx" << endl;
          outputFile << "  mov [acc], eax" << endl;
          outputFile << endl;
          i++;
          break;
        case JMP_OP:
          tokenFound = jmpLabelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == jmpLabelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a jmp's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; JMP " << tokenFound->second << endl;
          outputFile << "  jmp " << tokenFound->second << endl;
          outputFile << endl;
          i++;
          break;
        case JMPN_OP:
          tokenFound = jmpLabelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == jmpLabelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a jmpn's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; JMPN " << tokenFound->second << endl;
          outputFile << "  cmp dword [acc], 0" << endl;
          outputFile << "  jl " << tokenFound->second << endl;
          outputFile << endl;
          i++;
          break;
        case JMPP_OP:
          tokenFound = jmpLabelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == jmpLabelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a jmpp's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; JMPP " << tokenFound->second << endl;
          outputFile << "  cmp dword [acc], 0" << endl;
          outputFile << "  jg " << tokenFound->second << endl;
          outputFile << endl;
          i++;
          break;
        case JMPZ_OP:
          tokenFound = jmpLabelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == jmpLabelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a jmpz's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; JMPZ " << tokenFound->second << endl;
          outputFile << "  cmp dword [acc], 0" << endl;
          outputFile << "  je " << tokenFound->second << endl;
          outputFile << endl;
          i++;
          break;
        case COPY_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a copy's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; COPY " << tokenFound->second;
          temp = tokenFound->second;
          tokenFound = labelTable.find(stoi(lineVector.at(i + 2)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a copy's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << ", " << tokenFound->second << endl;
          outputFile << "  mov eax, [" << temp << "]" << endl;
          outputFile << "  mov [" << tokenFound->second << "], eax" << endl;
          outputFile << endl;
          i += 2;
          break;
        case LOAD_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a load's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; LOAD " << tokenFound->second << endl;
          outputFile << "  mov eax, [" << tokenFound->second << "]" << endl;
          outputFile << "  mov [acc], eax" << endl;
          outputFile << endl;
          i++;
          break;
        case STORE_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a store's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; STORE " << tokenFound->second << endl;
          outputFile << "  mov eax, [acc]" << endl;
          outputFile << "  mov [" << tokenFound->second << "], eax" << endl;
          outputFile << endl;
          i++;
          break;
        case INPUT_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find an input's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; INPUT " << tokenFound->second << endl;
          outputFile << "  push dword " << tokenFound->second << endl;
          outputFile << "  call input_call" << endl;
          outputFile << "  add esp, 4" << endl;
          outputFile << endl;
          i++;
          break;
        case OUTPUT_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find an output's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; OUTPUT " << tokenFound->second << endl;
          outputFile << "  push dword " << tokenFound->second << endl;
          outputFile << "  push dword 13" << endl;
          outputFile << "  call output_call" << endl;
          outputFile << "  add esp, 8" << endl;
          outputFile << endl;
          i++;
          break;
        case STOP_OP:
          outputFile << "  ; STOP" << endl;
          outputFile << "  mov eax, 1" << endl;
          outputFile << "  mov ebx, 0" << endl;
          outputFile << "  int 80h" << endl;
          outputFile << endl;
          isInSectionText = false;
          break;
        case S_INPUT_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a s_input's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; S_INPUT " << tokenFound->second;
          temp = tokenFound->second;
          tokenFound = labelTable.find(stoi(lineVector.at(i + 2)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a s_input's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << ", " << tokenFound->second << endl;
          outputFile << "  push dword " << temp << endl;
          outputFile << "  push dword [" << tokenFound->second << "]" << endl;
          outputFile << "  call s_input_call" << endl;
          outputFile << "  add esp, 8" << endl;
          outputFile << endl;
          i += 2;
          break;
        case S_OUTPUT_OP:
          tokenFound = labelTable.find(stoi(lineVector.at(i + 1)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a s_output's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << "  ; S_OUTPUT " << tokenFound->second;
          temp = tokenFound->second;
          tokenFound = labelTable.find(stoi(lineVector.at(i + 2)));
          if (tokenFound == labelTable.end()) {
            cout << "Something went reaaaally wrong..." << endl;
            cout << "Error trying to find a s_input's variable" << endl;
            outputFile.close();
            return;
          }
          outputFile << ", " << tokenFound->second << endl;
          outputFile << "  push dword " << temp << endl;
          outputFile << "  push dword [" << tokenFound->second << "]" << endl;
          outputFile << "  call s_output_call" << endl;
          outputFile << "  add esp, 8" << endl;
          outputFile << endl;
          i += 2;
          break;
        default:
          break;
      }
    }
  }

  outputFile << "output_call:" << endl;
  outputFile << "  push ebp" << endl;
  outputFile << "  mov ebp, esp" << endl;
  outputFile << "  mov eax, 4" << endl;
  outputFile << "  mov ebx, 1" << endl;
  outputFile << "  mov ecx, [ebp + 12]" << endl;
  outputFile << "  mov edx, [ebp + 8]" << endl;
  outputFile << "  int 80h" << endl;
  outputFile << "  pop ebp" << endl;
  outputFile << "  ret" << endl;
  outputFile << endl;

  outputFile << "input_call:" << endl;
  outputFile << "  push ebp" << endl;
  outputFile << "  mov ebp, esp" << endl;
  outputFile << "  mov eax, 3" << endl;
  outputFile << "  mov ebx, 0" << endl;
  outputFile << "  mov ecx, num_aux" << endl;
  outputFile << "  mov edx, 13" << endl;
  outputFile << "  int 80h" << endl;
  outputFile << "  mov ebx, dword [ebp + 8]" << endl;
  outputFile << "  mov ebp, 0" << endl;
  outputFile << "  mov ecx, num_aux" << endl;
  outputFile << "  mov edx, 10" << endl;
  outputFile << " input_loop:" << endl;
  outputFile << "  cmp byte [ecx], 10" << endl;
  outputFile << "  je bye_input" << endl;
  outputFile << "  mov eax, ebp" << endl;
  outputFile << "  imul edx" << endl;
  outputFile << "  mov ebp, eax" << endl;
  outputFile << "  mov edx, [ecx]" << endl;
  outputFile << "  and edx, 255" << endl;
  outputFile << "  add ebp, edx" << endl;
  outputFile << "  mov edx, 10" << endl;
  outputFile << "  sub ebp, 48" << endl;
  outputFile << "  add ecx, 1" << endl;
  outputFile << "  jmp input_loop" << endl;
  outputFile << " bye_input:" << endl;
  outputFile << "  mov dword [ebx], ebp" << endl;
  outputFile << "  pop ebp" << endl;
  outputFile << "  ret" << endl;
  outputFile << endl;

  outputFile << "s_output_call:" << endl;
  outputFile << "  push ebp" << endl;
  outputFile << "  mov ebp, esp" << endl;
  outputFile << "  mov eax, 4" << endl;
  outputFile << "  mov ebx, 1" << endl;
  outputFile << "  mov ecx, [ebp + 12]" << endl;
  outputFile << "  mov edx, [ebp + 8]" << endl;
  outputFile << "  int 80h" << endl;
  outputFile << "  pop ebp" << endl;
  outputFile << "  ret" << endl;
  outputFile << endl;
  outputFile << "s_input_call:" << endl;
  outputFile << "  push ebp" << endl;
  outputFile << "  mov ebp, esp" << endl;
  outputFile << "  mov eax, 3" << endl;
  outputFile << "  mov ebx, 0" << endl;
  outputFile << "  mov ecx, [ebp + 12]" << endl;
  outputFile << "  mov edx, [ebp + 8]" << endl;
  outputFile << "  int 80h" << endl;
  outputFile << "  pop ebp" << endl;
  outputFile << "  ret" << endl;

  outputFile.close();
  cout << "gg!" << endl;
  return;
}

map<int, string> insertLabelInTable(map<int, string> labelTable, int address,
                                    int labelTotal) {
  map<int, string>::iterator addressFound;
  string labelName = "label" + to_string(labelTotal);
  addressFound = labelTable.find(address);
  if (addressFound == labelTable.end()) {
    labelTable.insert(pair<int, string>(address, labelName));
  }
  return labelTable;
}

int getOpcode(const string& instruction) {
  if (instruction == ADD_STR) {
    return ADD_OP;
  } else if (instruction == SUB_STR) {
    return SUB_OP;
  } else if (instruction == MUL_STR) {
    return MULT_OP;
  } else if (instruction == DIV_STR) {
    return DIV_OP;
  } else if (instruction == JMP_STR) {
    return JMP_OP;
  } else if (instruction == JMPN_STR) {
    return JMPN_OP;
  } else if (instruction == JMPP_STR) {
    return JMPP_OP;
  } else if (instruction == JMPZ_STR) {
    return JMPZ_OP;
  } else if (instruction == COPY_STR) {
    return COPY_OP;
  } else if (instruction == LOAD_STR) {
    return LOAD_OP;
  } else if (instruction == STORE_STR) {
    return STORE_OP;
  } else if (instruction == INPUT_STR) {
    return INPUT_OP;
  } else if (instruction == OUTPUT_STR) {
    return OUTPUT_OP;
  } else if (instruction == STOP_STR) {
    return STOP_OP;
  } else if (instruction == S_INPUT_STR) {
    return S_INPUT_OP;
  } else if (instruction == S_OUTPUT_STR) {
    return S_OUTPUT_OP;
  }
  return -1;
}
