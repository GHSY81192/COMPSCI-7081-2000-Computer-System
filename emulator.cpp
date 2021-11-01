
// emulate executing Hack CPU instructions
#include "iobuffer.h"
#include "symbols.h"
#include "hack_computer.h"
#include <bitset>
#include <vector>
#include <iostream>

// to make out programs a bit neater
using namespace std;

using namespace CS_IO_Buffers;
using namespace CS_Symbol_Tables;
using namespace Hack_Computer;

using std::to_string;

// This program must emulate the execution of Hack CPU instructions
//
// To read from registers A, D or PC, use read_A(), read_D() or read_PC()
// To write to registers A, D or PC, use write_A(value), write_D(value) or write_PC(value)
//
// To read from ROM use read_ROM(address)
// To read from RAM use read_RAM(address)
// To write to RAM use write_RAM(address,value)
//
// All values returned by a read_*() function are 16-bit unsigned integers
// All parameters passed to a write_*() function are 16-bit unsigned integers
//

/*****************   REPLACE THE FOLLOWING CODE  ******************/
static void write_M(uint16_t isA, uint16_t isD, uint16_t isM, uint16_t val){
    if(isA == 1){
        write_A(val);
    }
    if(isD == 1){
        write_D(val);
    }
    if(isM == 1){
        write_RAM(read_A(), val);
    }
}

static bool jumpPart(uint16_t jmp, uint16_t val){
    if(jmp == 1){// JGT
        if(val > 0 && val <= 32768){
            write_PC(read_A());
            return true;
        }
    }
    if (jmp == 2){// JEQ
        if (val == 0){
            write_PC(read_A());
            return true;
        }
    }
    if (jmp == 3){// JGE
        if(val >= 0 && val <= 32768){
            write_PC(read_A());
            return true;
        }
    }
    if (jmp == 4){// JLT
        if (val > 32768){
            write_PC(read_A());
            return true;
        }
    }
    if (jmp == 5){// JNE
        if (val != 0){
            write_PC(read_A());
            return true;
        }
    }
    if (jmp == 6){// JLE
        if (val > 32768 || val == 0){
           write_PC(read_A());
            return true;
        }
    }
    if (jmp == 7){// JMP
        write_PC(read_A());
        return true;
    }else{
        write_PC(read_PC() + 1);
        return true;
    }
}

// disassemble an instruction - convert binary to symbolic form
// A instructions should be "@" followed by the value in decimal
// C instructions should be dest=alu;jump
// omit dest= if all destination bits are 0
// omit ;jump if all jump bits are 0
string disassemble_instruction(uint16_t instruction)
{
    uint16_t ins[16] = {0};
    int i = 0;
    uint16_t tmp = instruction;

    // bitwise var for comp part, of C-instruction
    uint16_t comp = instruction << 3;
    comp = comp >> 9;

    // dest part
    uint16_t dest = instruction << 10;
    dest = dest >> 13;

    // jump part
    uint16_t jmp = instruction << 13;
    jmp = jmp >> 13;

    while(instruction != 0){
        ins[i] = instruction & 1;
        instruction = instruction >> 1;
        i++;
    }
    if(ins[15] == 0){
        return "@" + to_string(tmp);
    }else if(ins[15] == 1 && ins[14] == 1 && ins[13] == 1){
        return destination(dest) + aluop(comp) + jump(jmp);
    }
    return "";
}

// emulate a cpu instruction - the Hack Computer has been initialised
// the PC contains the address in ROM of the instruction to execute
// if you want to produce additional output, use write_to_traces()
static void emulate_instruction()
{
    uint16_t pc = read_PC();
    uint16_t instruction = read_ROM(pc);
    uint16_t ins[16] = {0};
    int i = 0;
    uint16_t tmp = instruction;
    
    uint16_t comp = instruction << 3;
    comp = comp >> 9;

    uint16_t dest = instruction << 10;
    dest = dest >> 13;

    uint16_t jmp = instruction << 13;
    jmp = jmp >> 13;

    while(instruction != 0){
        ins[i] = instruction & 1;
        instruction = instruction >> 1;
        i++;
    }
    if(ins[15] == 0){
        write_A(tmp);
        write_PC(read_PC() + 1);
    }else if (ins[15] == 1 && ins[14] == 1 && ins[13] == 1){
        if(comp == 0){// D&A
            uint16_t val = read_A()&read_D();
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 2){// D+A
            uint16_t val = read_D();
            val = val + read_A();
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 7){// A-D
            uint16_t val = read_A();
            val = val - read_D();
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 10){// .D.
            uint16_t val = read_D();
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 12){// D
            uint16_t val = read_D();
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 14){// D-1
            uint16_t val = read_D();
            val = val - 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 13){// !D
            uint16_t val = read_D();
            val = ~val;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 15){// -D
            uint16_t val = read_D();
            val = ~val + 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 19){// D-A
            uint16_t val = read_D();
            val = val - read_A();
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 21){// D|A
            uint16_t val = read_D();
            val = val|read_A();
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 31){// D+1
            uint16_t val = read_D();
            val = val + 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 42){// 0
            uint16_t val = 0;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if(comp == 48){// A
            uint16_t val = read_A();
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if(comp == 49){// !A
            uint16_t val = read_A();
            val = ~val;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if(comp == 50){// A-1
            uint16_t val = read_A();
            val = val - 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if(comp == 51){// -A
            uint16_t val = ~read_A();
            val = val + 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if(comp == 55){// A+1
            uint16_t val = read_A() + 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if(comp == 58){// -1
            uint16_t val = - 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 63){// 1
            uint16_t val = 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 64){// D&M
            uint16_t val = read_D();
            val = val & read_RAM(read_A());
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 66){// D+M
            uint16_t val = read_D();
            val = val + read_RAM(read_A());
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 71){// M-D
            uint16_t val = read_RAM(read_A());
            val = val - read_D();
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 83){// D-M
            uint16_t val = read_D();
            val = val - read_RAM(read_A());
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 85){// D|M
            uint16_t val = read_D();
            val = val|read_RAM(read_A());
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 112){// M
            uint16_t val = read_RAM(read_A());
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 113){// !M
            uint16_t val = read_RAM(read_A());
            val = ~val;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 114){// M-1
            uint16_t val = read_RAM(read_A());
            val = val - 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 115){// -M
            uint16_t val = read_RAM(read_A());
            val = ~val + 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 119){// M+1
            uint16_t val = read_RAM(read_A());
            val = val + 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 125){// M|D
            uint16_t val = read_D();
            val = val|read_RAM(read_A());
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }else if (comp == 127){// .1.
            uint16_t val = 1;
            write_M(ins[5], ins[4], ins[3], val);
            jumpPart(jmp, val);
        }
    }
}


/*****************        DOWN TO HERE         ******************/

// if the first command line argument is "D" the test output must display disassembled instructions
bool display_disassembled_instructions = false;

// this function only calls disassemble_instruction if required
// it is called from more_cpu_tests() and is passed the instruction being emulated
string disassemble(uint16_t instruction)
{
    if ( display_disassembled_instructions ) return disassemble_instruction(instruction);

    // default is to display instruction in binary
    return "\"" + std::bitset<16>(instruction).to_string() + "\"";
}


// main program
int main(int argc,char **argv)
{
    // force all output to be immediate - the test scripts will override this
    config_output(iob_immediate);
    config_errors(iob_immediate);

    // is the first command line argument "D", ie does the test output include disassembly of the instruction being emulated
    display_disassembled_instructions = argc > 1 && string(argv[1]) == "D";

    // more_cpu_tests() will initialise the next test and return true
    // if there are no more tests to be run, it returns false
    // more_cpu_tests() also outputs the results of the previous test
    while ( more_cpu_tests(disassemble) )
    {
        emulate_instruction();
    }

    // flush the output and any errors
    print_output();
    print_errors();
}

