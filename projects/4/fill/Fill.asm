// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//  Pseudo code:
//  LOOP:
//      n = 8192
//      if (KBD == 0) goto LOOP_WHITE
//      LOOP_BLACK:
//          n = n - 1
//          *(SCREEN + n) = -1
//          if (n == 0) goto LOOP
//          goto LOOP_BLACK
//      LOOP_WHITE:
//          n = n - 1
//          *(SCREEN + n) = 0
//          if (n == 0) goto LOOP
//          goto LOOP_WHITE


//  Code:

(LOOP)
    //n = 8192
    @8192
    D=A
    @n
    M=D

    // if (KBD == 0) goto LOOP_WHITE
    @KBD
    D=M
    @LOOP_WHITE
    D;JEQ

(LOOP_BLACK)
    // n = n - 1
    @n
    M=M-1
    D=M

    // *(SCREEN + n) = -1
    @SCREEN
    A=D+A
    M=-1

    // if (n==0) goto LOOP
    @n
    D=M
    @LOOP
    D;JEQ

    // goto LOOP_BLACK
    @LOOP_BLACK
    0;JMP

(LOOP_WHITE)
    // n = n - 1
    @n
    M=M-1
    D=M

    // *(SCREEN + n) = 0
    @SCREEN
    A=D+A
    M=0

    // if (n==0) goto LOOP
    @n
    D=M
    @LOOP
    D;JEQ

    // goto LOOP_WHITE
    @LOOP_WHITE
    0;JMP
