	list 	p = 10F200	    ; list directive to define processor

    include	 <p10f200.inc>	; processor specific variable definitions


	; PIC Configuration for watchdog timer, master clear reset and code protection. Refer to datasheet for details.

    __CONFIG _IntRC_OSC ;WDT_Placeholder ;MCLRE_Placeholder ;CP_Placeholder


    ; name pins for clarity

    #define	 TxPin	 GPIO,1	; TX pin

    #define	 LEPin	 GPIO,0	; Load Enable pin

    #define	 PLLclk	 GPIO,2	; PLL Clock


    ; RAM address allocations

    cblock 0x10			    ; start at RAM location 10h

      dly:3				    ; 3 bytes for delay loops

    endc


Main

	movwf	 OSCCAL	        ; load factory osccal value at start-up

    call     PauseOneSec    ; Call four times to pause four seconds before beginning.

    call     PauseOneSec

    call     PauseOneSec

    call     PauseOneSec

	call	 Init	        ; init hardware

    call     PauseOneSec

;Load_Latches_Placeholder

	sleep


    ;One second pause loop

PauseOneSec

	movlw	.8

	movwf	dly

	movlw	.47

	movwf	dly+.1

	movlw	.3

	movwf	dly+.2

Delay_One

	decfsz	dly, f

	goto	$+.2

	decfsz	dly+.1, f

	goto	$+.2

	decfsz	dly+.2, f

	goto	Delay_One

	goto	$+.1

	nop

	retlw	.0


	; Initialisation

Init

	bsf	 OSCCAL,0           ; FOSC4 applied to GP2

	movlw	 b'00001000'	; GP3 = input, rest outputs

	tris	 GPIO	        ; set I/O directions

    bcf LEPin               ;Ensure Load Enable is held low at the start.

	clrwdt		            ; per datasheet

	movlw	 b'11000000'    ; wake-up on pin change disabled, weak pull-ups off

	option		            ; write to OPTION_REG

	retlw	 .0	            ; return to main


;Latch_I_Placeholder

	bsf	 LEPin	            ; Set Load Enable high to load register.

    bcf	 LEPin              ; Set Load Enable low and wait for register to fill up again.

    retlw	 .0	            ; return to main


    ; Function Latch, Counter Reset = 0

LoadLatchF_CR0

;Latch_F_Placeholder

	bsf	 LEPin	            ; Set Load Enable high to load register.

    bcf	 LEPin              ; Set Load Enable low and wait for register to fill up again.

    retlw	 .0	            ; return to main


	; Reference Counter Latch

LoadLatchR

;Latch_R_Placeholder

	bsf	 LEPin	            ; Set Load Enable high to load register.

    bcf	 LEPin              ; Set Load Enable low and wait for register to fill up again.

    retlw	 .0	            ; return to main


	; N Counter Latch

LoadLatchN

;Latch_N_Placeholder

	bsf	 LEPin	            ; Set Load Enable high to load register.

    bcf	 LEPin              ; Set Load Enable low and wait for register to fill up again.

    retlw	 .0	            ; return to main


	; 0xff is the reset vector for the 10F200.

    ; value placed in w here will be loaded into OSCCAL for 4MHz

    ; adjust if required

    org	 0xff	            ; org sets the address at which the next instruction is placed.

    movlw	 .1	            ; oscillator set to center frequency with FOSC4 output on GP2

    END		                ; directive 'end of program'
