;            testusb   program
;***************************************************
      FIFOE   BIT     P2.2	   ;定义
      FIFOF   BIT     P2.3
      DMARD   BIT     P3.7
      DMAWR   BIT     P3.6
      DMAOE   BIT     P2.5
      DMADIR  BIT     P2.1
      DMACS   BIT     P3.4
      dmaing  bit     p2.0
      ptkend  bit     p2.6

;**************************************************8
      org    0000h
      ljmp   ini
;*****************************************************
      org    0030h
ini:								  ;初始化
      mov    sp,#60h
      mov    r2,#0aah
      mov    r3,#0ffh
      mov    r4,#2
      mov    r1,#30h
      mov    r0,#22h
      mov    22h,#45
      CLR    DMADIR
      CLR    DMACS
      setb   ptkend
       setb   dmaing
       setb   FIFOF
;******************************************************
;    发送数据
;*******************************************************
send:
      setb   ptkend
      SETB   DMAWR
    
send1:
      nop
      nop
      jnb    dmaing,send
     
send2:
      NOP
      NOP
      JNB    FIFOF,send
      NOP
      mov    a,r2
      NOP
      movx   @r0,a
      nop
      nop
    
      nop
      mov    r3,#0ffh

      NOP
      NOP
      nop
      nop
      nop
      INC    R2
      LJMP   SEND

;******************************************************
ini110:
      setb   dmaing
ini10:
      nop
      nop
      jnb    dmaing,ini110
      setb   FIFOE
INI1:
      NOP
      NOP
      JNB    FIFOE,INI110
      MOV    P0,#0FFH
start:                     ;读数据
      NOP
      nop
      CLR    DMAOE
      NOP
      NOP
      CLR    DMARD
      NOP
      NOP
      setb   dmard
      nop
      nop
      MOV     A,P0
      MOV     @R1,A
      NOP
      NOP
      setb    dmaoe
      nop
      nop
      clr     dmard
      INC     R1
      nop
      MOV     A,R1
      CJNE    A,#60H,ini110
      MOV     R1,#30H
      nop
      nop

      LJMP    ini110
;*******************************************************8
      end
