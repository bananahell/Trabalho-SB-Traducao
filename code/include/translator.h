#ifndef TRANSLATOR
#define TRANSLATOR

#include <map>
#include <string>

void translateCode(char*);
int getOpcode(const std::string&);
std::map<int, std::string> insertLabelInTable(std::map<int, std::string>, int,
                                              int);

#define ADD_STR "1"
#define SUB_STR "2"
#define MUL_STR "3"
#define DIV_STR "4"
#define JMP_STR "5"
#define JMPN_STR "6"
#define JMPP_STR "7"
#define JMPZ_STR "8"
#define COPY_STR "9"
#define LOAD_STR "10"
#define STORE_STR "11"
#define INPUT_STR "12"
#define OUTPUT_STR "13"
#define STOP_STR "14"
#define S_INPUT_STR "15"
#define S_OUTPUT_STR "16"

#define ADD_OP 1
#define SUB_OP 2
#define MULT_OP 3
#define DIV_OP 4
#define JMP_OP 5
#define JMPN_OP 6
#define JMPP_OP 7
#define JMPZ_OP 8
#define COPY_OP 9
#define LOAD_OP 10
#define STORE_OP 11
#define INPUT_OP 12
#define OUTPUT_OP 13
#define STOP_OP 14
#define S_INPUT_OP 15
#define S_OUTPUT_OP 16

#endif  // TRANSLATOR
