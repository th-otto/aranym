/* MJ 2001 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "sysdeps.h"
#include "version.h"
#include "cfgopts.h"

#ifndef ARANYMHOME
# define ARANYMHOME		".aranym"
#endif
#define ARANYMCONFIG	"config"
#define ARANYMNVRAM		"nvram"
#define ARANYMKEYMAP	"keymap"
#ifndef DIRSEPARATOR
# define DIRSEPARATOR	"/"
#endif

enum geo_type {
	geoCylinders,
	geoHeads,
	geoSpt,
	geoByteswap
};

extern int get_geometry(char *, geo_type geo);

// External filesystem type
typedef struct {
	bool halfSensitive;
	char rootPath[512];
} bx_aranymfs_options_t;

// Floppy device
typedef struct {
  char path[512];
  // bool inserted;
  // bool enforceRemount;
} bx_floppy_options_t;

// IDE device
typedef struct {
  bool present;
  bool isCDROM;
  bool byteswap;
  char path[512];
  unsigned int cylinders;
  unsigned int heads;
  unsigned int spt;
} bx_disk_options_t;

// CDROM device
typedef struct {
  bool present;
  char path[512];
  bool inserted;
} bx_cdrom_options_t;

 /* 
typedef struct {
  char *path;
  unsigned long address;
  } bx_rom_options_t;
*/

// TOS options
typedef struct {
  bool console_redirect;
  long cookie_mch;
} bx_tos_options_t;

// Video output options
typedef struct {
  bool fullscreen;		// boot in fullscreen
  int8 boot_color_depth;		// boot color depth
  uint8 refresh;
  int8 monitor;				// VGA or TV
#ifdef DIRECT_TRUECOLOR
  bool direct_truecolor;	// patch TOS to enable direct true color
#endif /* DIRECT_TRUECOLOR */
} bx_video_options_t;

// Startup options
typedef struct {
  bool grabMouseAllowed;
  bool debugger;
} bx_startup_options_t;

// JIT compiler options
typedef struct {
  bool jit;
  bool jitfpu;
  uint32 jitcachesize;
  uint32 jitlazyflush;
} bx_jit_options_t;

/*
typedef struct {
  char      *path;
  bool   cmosImage;
  unsigned int time0;
  } bx_cmos_options;
*/

// Options 
typedef struct {
  bx_floppy_options_t	floppy;
  // bx_floppy_options_t floppyb;
  bx_disk_options_t	diskc;
  bx_disk_options_t	diskd;
  bx_cdrom_options_t	cdromd;
  bx_aranymfs_options_t	aranymfs[ 'Z'-'A'+1 ];
//  bx_cookies_options_t cookies;
  bx_video_options_t	video;
  bx_tos_options_t	tos;
  bx_startup_options_t	startup;
  bx_jit_options_t	jit;
  bool			autoMouseGrab;
  // char              bootdrive[2];
  // unsigned long     vga_update_interval;
  // unsigned long     keyboard_serial_delay;
  // unsigned long     floppy_command_delay;
  // unsigned long     ips;
  // bool           private_colormap;
  // bx_cmos_options_t	cmos;
  bool			newHardDriveSupport;
} bx_options_t;

extern bx_options_t bx_options;


extern uint32 FastRAMSize;	// Size of Fast-RAM

extern char *program_name;
extern char rom_path[512];
extern char emutos_path[512];

void usage(int);
extern int decode_switches(FILE *, int, char **);
extern int saveSettings(const char *);
extern char *getConfFilename(const char *file, char *buffer, unsigned int bufsize);

#endif
