#pragma once

typedef struct CHS {
    unsigned short cyl;
    unsigned short head;
    unsigned short sec;
} tCHS;

typedef struct ECHS {
    unsigned short cyl;
    unsigned short head;
    unsigned short sec;
} tLARGE;

typedef struct IDECHS {
    unsigned short cyl;
    unsigned short head;
    unsigned short sec;
} tIDECHS;

typedef struct LBA {
    unsigned int lba;
} tLBA;

typedef struct PART {
    int activ;
    tCHS beg;
    tCHS end;
    tLBA lba_beg;
    int size;
} tPART;

int g_lba2chs(tLBA orig, tCHS *dest);
int g_lba2large(tLBA orig, tLARGE *dest);
int g_lba2idechs(tLBA orig, tIDECHS *dest);
int g_chs2large(tCHS orig, tLARGE *dest);
int g_chs2idechs(tCHS orig, tIDECHS *dest);
int g_chs2lba(tCHS orig, tLBA *dest);
int g_large2chs(tLARGE orig, tCHS *dest);
int g_large2idechs(tLARGE orig, tIDECHS *dest);
int g_large2lba(tLARGE orig, tLBA *dest);
int g_idechs2chs(tIDECHS orig, tCHS *dest);
int g_idechs2large(tIDECHS orig, tLARGE *dest);
int g_idechs2lba(tIDECHS orig, tLBA *dest);
int a_lba2chs(tCHS geometry, tLBA orig, tCHS *dest);
