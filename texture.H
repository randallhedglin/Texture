// COMMANDS /////////////////////////////////////////////////////////////////

#define CM_EXIT (WM_USER+100)

// GENERAL //////////////////////////////////////////////////////////////////

#define PALETTEVERSION    0x300
#define NUMPALETTEENTRIES 216
#define PALETTEMUL        51

#define TMR_INITIALIZE 1
#define TMR_MAINLOOP   2
#define TMR_FPSCOUNT   3

#define DESFPS 20

// ERROR MESSAGES ///////////////////////////////////////////////////////////

#define ERR_PALINSMEM     (1<<0)
#define ERR_PALGENFAIL1   (1<<1)
#define ERR_PALGENFAIL2   (1<<2)
#define ERR_VIDINSCOL     (1<<3)
#define ERR_PALGENFAIL3   (1<<4)
#define ERR_PALGENFAIL4   (1<<5)
#define ERR_PALGENFAIL5   (1<<6)
#define ERR_BMPBUFGENFAIL (1<<7)
#define ERR_INSSCRSPC     (1<<8)
#define ERR_BMPLOADFAIL   (1<<9)
#define ERR_SETBKCOLOR    (1<<10)

