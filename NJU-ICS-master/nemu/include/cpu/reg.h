#ifndef __REG_H__
#define __REG_H__

#include "common.h"
//原始的数据 
//enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
//enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
//enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

enum { R_EAX, R_EDX, R_ECX, R_EBX, R_EBP, R_ESI, R_EDI, R_ESP };
enum { R_AX, R_DX, R_CX, R_BX, R_BP, R_SI, R_DI, R_SP };
enum { R_AL, R_DL, R_CL, R_BL, R_AH, R_DH, R_CH, R_BH };


/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme(方案、格式) in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint(提示): Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
  struct {
    uint32_t _32;					//32 bits register
    uint16_t _16;					//16 bits register
    uint8_t _8[2];					//the high 8 bits register and low 8 bits register
  } gpr[8];

  /* Do NOT change the order of the GPRs' definitions. */

  /* In NEMU, rtlreg_t is exactly uint32_t. This makes RTL instructions
   * in PA2 able to directly access these registers.
   */
 // rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;   
  rtlreg_t eax, edx, ecx, ebx, ebp, esi, edi, esp;

  vaddr_t eip;

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
  assert(index >= 0 && index < 8);
  return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

static inline const char* reg_name(int index, int width) {
  assert(index >= 0 && index < 8);
  switch (width) {
    case 4: return regsl[index];
    case 1: return regsb[index];
    case 2: return regsw[index];
    default: assert(0);
  }
}

#endif
