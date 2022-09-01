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

  cout << "Label Table" << endl;
  for (auto const& [key, val] : labelTable) {
    cout << key << ':' << val << endl;
  }
  cout << "Jump Label Table" << endl;
  for (auto const& [key, val] : jmpLabelTable) {
    cout << key << ':' << val << endl;
  }

  inputFile.close();

  string extension = fileNameIn;
  extension = extension.substr(extension.find("."));
  string fileNameOut = fileNameIn;
  fileNameOut.replace(fileNameOut.find(extension), extension.length(), ".s");
  ofstream outputFile(fileNameOut, ifstream::out);

  map<int, string>::iterator tokenFound;

  outputFile << "section .data";
  for (unsigned i = labelTable.begin()->first; i < lineVector.size(); i++) {
    tokenFound = labelTable.find(i);
    if (tokenFound != labelTable.end()) {
      outputFile << endl;
      outputFile << tokenFound->second << " db " << lineVector.at(i);
    } else {
      outputFile << ", " << lineVector.at(i);
    }
  }

  outputFile << endl;
  outputFile << "section .text" << endl;

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
