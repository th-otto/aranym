#include "sysdeps.h"
#include "hardware.h"
#include "cpu_emulation.h"
#include "memory.h"
#include "icio.h"
#include "acsifdc.h"
#include "rtc.h"
#include "blitter.h"
#include "ide.h"
#include "mmu.h"
#include "hostscreen.h"
#include "parallel.h"

#define DEBUG 0
#include "debug.h"

#ifdef HW_SIGSEGV
extern int in_handler;
# define BUS_ERROR(a)	{ regs.mmu_fault_addr=(a); in_handler = 0; longjmp(excep_env, 2); }
#else
# define BUS_ERROR(a)	{ regs.mmu_fault_addr=(a); longjmp(excep_env, 2); }
#endif

#define debug_print_IO(a) "unknown"

Parallel parallel;

MFP *mfp;
MMU *mmu;
IKBD *ikbd;
MIDI *midi;
ACSIFDC *fdc;
RTC *rtc;
IDE *ide;
DSP *dsp;
BLITTER *blitter;
VIDEL *videl;
YAMAHA *yamaha;
ARADATA *aradata;

enum {iMFP = 0, iMMU, iIKBD, iMIDI, iFDC, iRTC, iIDE, iDSP, iBLITTER, iVIDEL,
	  iYAMAHA, iARADATA, iSOUND, iDSPDMA, iSCC, iCARTRIDGE, iPADDLE,
	  /* the iITEMS must be the last one in the enum */
	  iITEMS};

BASE_IO *arhw[iITEMS];

void HWInit()
{
	arhw[iMFP] = mfp = new MFP(0xfffa00, 0x30);
	arhw[iMMU] = mmu = new MMU(0xff8000, 8);
	arhw[iIKBD] = ikbd = new IKBD(0xfffc00, 4);
	arhw[iMIDI] = midi = new MIDI(0xfffc04, 4);
	arhw[iFDC] = fdc = new ACSIFDC(0xff8600, 0x10);
	arhw[iRTC] = rtc = new RTC(0xff8960, 4);
	arhw[iIDE] = ide = new IDE(0xf00000, 0x3a);
	arhw[iDSP] = dsp = new DSP(0xffa200, 8);
	arhw[iBLITTER] = blitter = new BLITTER(0xff8A00, 0x3e);
	arhw[iVIDEL] = videl = new VIDEL(0xff8200, 0xc4);
	arhw[iYAMAHA] = yamaha = new YAMAHA(0xff8800, 4);
	arhw[iARADATA] = aradata = new ARADATA(0xf90000, 0x10000);

	arhw[iSOUND] = new BASE_IO(0xff8900, 0x22);
	arhw[iDSPDMA] = new BASE_IO(0xff8930, 0x14);
	arhw[iSCC] = new BASE_IO(0xff8c80, 0x16);
	arhw[iPADDLE] = new BASE_IO(0xff9200, 0x24);
	arhw[iCARTRIDGE] = new BASE_IO(0xfa0000, 0x20000);
}
//	{"DMA/SCSI", 0xff8700, 0x16, &fake_io},
// 	{"SCSI", 0xff8780, 0x10, &fake_io},
//	{"MicroWire", 0xff8922, 0x4},
//	{"DMA/SCC", 0xff8C00, 0x16},
//	{"VME", 0xff8e00, 0x0c},
//	{"STFPC", 0xfffa40, 8},
//	{"RTC", 0xfffc20, 0x20}

void HWExit()
{
	for(int i=0; i<iITEMS; i++) {
		delete arhw[i];
	}
}

BASE_IO *getModule(memptr addr)
{
	for(int i=0; i<iITEMS; i++) {
		if (arhw[i]->isMyHWRegister(addr))
			return arhw[i];
	}
	D(bug("HW register %08x not emulated", addr));
	return NULL;
}

ARADATA* getARADATA(){ return aradata; /* (ARADATA*)arhw[ARADATA];*/ }
MFP* getMFP()		{ return mfp; }
IKBD* getIKBD()		{ return ikbd; }
MIDI* getMIDI()		{ return midi; }
YAMAHA* getYAMAHA()	{ return yamaha; }
VIDEL* getVIDEL()	{ return videl; }
DSP* getDSP()		{ return dsp; }
ACSIFDC *getFDC()	{ return fdc; }

uae_u32 HWget_l (uaecptr addr) {
	D(bug("HWget_l %x <- %s at %08x", addr, debug_print_IO(addr), showPC()));
	BASE_IO *ptr = getModule(addr);
	if (ptr != NULL) {
		return ptr->handleReadL(addr);
	}
	BUS_ERROR(addr);
}

uae_u16 HWget_w (uaecptr addr) {
	D(bug("HWget_w %x <- %s at %08x", addr, debug_print_IO(addr), showPC()));
	BASE_IO *ptr = getModule(addr);
	if (ptr != NULL) {
		return ptr->handleReadW(addr);
	}
	BUS_ERROR(addr);
}

uae_u8 HWget_b (uaecptr addr) {
	D(bug("HWget_b %x <- %s at %08x", addr, debug_print_IO(addr), showPC()));
	BASE_IO *ptr = getModule(addr);
	if (ptr != NULL) {
		return ptr->handleRead(addr);
	}
	BUS_ERROR(addr);
}

void HWput_l (uaecptr addr, uae_u32 value) {
	D(bug("HWput_l %x,%d ($%08x) -> %s at %08x", addr, value, value, debug_print_IO(addr), showPC()));
	BASE_IO *ptr = getModule(addr);
	if (ptr != NULL) {
		ptr->handleWriteL(addr, value);
		return;
	}
	BUS_ERROR(addr);
}

void HWput_w (uaecptr addr, uae_u16 value) {
	D(bug("HWput_w %x,%d ($%04x) -> %s at %08x", addr, value, value, debug_print_IO(addr), showPC()));
	BASE_IO *ptr = getModule(addr);
	if (ptr != NULL) {
		ptr->handleWriteW(addr, value);
		return;
	}
	BUS_ERROR(addr);
}

void HWput_b (uaecptr addr, uae_u8 value) {
	D(bug("HWput_b %x,%u ($%02x) -> %s at %08x", addr, value, value, debug_print_IO(addr), showPC()));
	BASE_IO *ptr = getModule(addr);
	if (ptr != NULL) {
		ptr->handleWrite(addr, value);
		return;
	}
	BUS_ERROR(addr);
}
