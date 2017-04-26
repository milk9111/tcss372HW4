; Daniel and Connor 
; 372 Problem 4

; Part 1
; The assembly program will prompt a user to enter their first name. It will allow the user to type in their name. The string
; should be stored in a memory location (BLKW #20). 

; Part 2a
; The program should then call a subroutine that "encrypts" the name by 
; subtracting a small constant from each character and replacing the original string with the new one. The subroutine will
; need the starting address of the string location passed to it in R1. (Get location of subroutine from R1)

; Part 3a (Function of Subroutine)
; It will loop through the string subtracting the constant from each character and replacing it in the array.

; Part 3b
; The subroutine return the starting address of the array in R0, (to indicate success) the program will print prompt
; "Press any key to continue: ".  

; Part 4
; When user presses key (without echo) the program prints the array (using trap x22). 


		.ORIG x3000
			LEA R1, HELLO
	AGAIN		LDR R2, R1, #0
			BRz NEXT
			ADD R1, R1, #1
			BR AGAIN
	NEXT		LEA R0, PROMPT
			PUTS
			LD R3, NEGENTER
		
	AGAIN2 		GETC
			OUT
			ADD R2, R0, R3
			BRz CONT
			STR R0, R1, #0
			ADD R1, R1, #1

			BR AGAIN2
	CONT		AND R2, R2, #0
			STR R2, R1, #0

			LEA R0, HELLO  ;Used as test to print name back
			PUTS		; Printing name
	
			AND R1, R1, #0
		JSRR R1	
			


		HALT

NEGENTER	.FILL xFFF6	
PROMPT		.STRINGZ "Please enter your name: "
HELLO 		.STRINGZ "Hello, "
		.BLKW #20 #8
		.END