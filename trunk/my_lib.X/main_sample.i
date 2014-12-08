#line 1 "main_sample.c"
#line 1 "main_sample.c"

#line 6 "main_sample.c"
 




#line 12 "main_sample.c"









#line 22 "main_sample.c"


 


void
#line 31 "main_sample.c"
high_isr(void);

void
#line 37 "main_sample.c"
low_isr(void);



#pragma romdata bootloader = 0x2A
const rom char bootloader[APP_START - 0x2A];
extern void _startup(void);

#pragma code AppVector = APP_START
void AppVector(void) {
    _asm GOTO _startup _endasm
}

#pragma code AppHighIntVector = APP_HINT
void AppHighIntVector(void) {
    _asm
        GOTO high_isr 
    _endasm
}

#pragma code AppLowIntVector = APP_LINT
void low_vector(void) {
    _asm
        GOTO low_isr 
    _endasm
}
#line 64 "main_sample.c"

#pragma code  
#line 67 "main_sample.c"
void main() {
    AllDigital();
}


#pragma interrupt high_isr
#line 74 "main_sample.c"
void
#line 78 "main_sample.c"
high_isr(void) {
    
}


#pragma interruptlow low_isr
#line 85 "main_sample.c"
void
#line 89 "main_sample.c"
low_isr(void) {
    
}

