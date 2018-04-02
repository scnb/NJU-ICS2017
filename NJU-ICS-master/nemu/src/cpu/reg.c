#include "nemu.h"
#include <stdlib.h>
#include <time.h>

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

/* Generate some random data to test if registers work correctly */
void reg_test() {
  /* generate some data randomly */
  srand(time(0));
  uint32_t sample[8];
  uint32_t eip_sample = rand();
  /* the EIP register aka the PC(Program Counter) */
  cpu.eip = eip_sample;

  int i;
  /* loop for all the 32bit register */
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
	/* sample is a array that have member who is 32bit */
   /* and reg_l's l means 32bit */
    reg_l(i) = sample[i];
  /* reg_w's w means word aka 2bytes = 16bit */
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

 /* take the lowest 8bits to & with sample[R_EAX] */
  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  /* first right shift sample[R_EAX] then take it's lowest 8 bits */
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(eip_sample == cpu.eip);
}
