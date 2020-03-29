;Tester for Maman 14

;wont work missing operands
mov label1
cmp label1
add label1
sub label1
lea label1
clr
not
inc
dec
jmp
bne
red
prn
jsr 

;wrong miun
mov #-1, #6
mov label1, #6
mov *r1, #6
mov r1, #6

;will work
mov #-1, label1
mov #-1, *r1
mov #-1, r1
mov label1 , label1
mov label1 ,*r1
mov label1,r1
mov *r1, label1
mov *r1, *r1
mov *r1, r1
mov r1, label1
mov r1, *r1
mov r1, r1


;will work
cmp #-1, #6
cmp label1, #6
cmp *r1, #6
cmp r1, #6
cmp #-1, label1
cmp #-1, *r1
cmp #-1 ,r1
cmp label1, label1
cmp label1 ,*r1
cmp label1 ,r1
cmp *r1 ,label1
cmp *r1 ,*r1
cmp *r1 ,r1
cmp r1 ,label1
cmp r1 ,*r1
cmp r1 ,r1

;wrong miun
add #-1, #6
add label1, #6
add *r1 ,#6
add r1 ,#6

;will work
add #-1 ,label1
add #-1 ,*r1
add #-1 ,r1
add label1, label1
add label1 ,*r1
add label1 ,r1
add *r1 ,label1
add *r1 ,*r1
add *r1 ,r1
add r1 ,label1
add r1 ,*r1
add r1 ,r1

;wrong miun
sub #-1 ,#6
sub label1, #6
sub *r1 ,#6
sub r1 ,#6

;will work
sub #-1 ,label1
sub #-1 ,*r1
sub #-1 ,r1
sub label1, label1
sub label1, *r1
sub label1 ,r1
sub *r1 ,label1
sub *r1 ,*r1
sub *r1 ,r1
sub r1 ,label1
sub r1 ,*r1
sub r1 ,r1

;wrong miun
lea #-1 ,#6
lea label1, #6
lea *r1 ,#6
lea r1 ,#6
lea #-1 ,label1
lea #-1 ,*r1
lea #-1 ,r1
lea *r1 ,label1
lea *r1 ,*r1
lea *r1 ,r1
lea r1 ,label1
lea r1 ,*r1
lea r1 ,r1

;will work
sub label1, label1
sub label1 ,*r1
sub label1 ,r1

;wrong miun
clr #-1 

;will work
clr label1
clr *r2 
clr r2

;wrong miun
not #-1 

;will work
not label1
not *r2 
not r2

;wrong miun
inc #-1 

;will work
inc label1
inc *r2 
inc r2

;wrong miun
dec #-1 

;will work
dec label1
dec *r2 
dec r2

;wrong miun
jmp #-1 
jmp r2

;will work
jmp label1
jmp *r2 

;wrong miun
bne #-1 
bne r2

;will work
bne label1
bne *r2

;wrong miun
red #-1 

;will work
red label1
red *r2 
red r2 

;will work
prn #-1
prn label1
prn *r2 
prn r2

;wrong miun
jsr #-1 
jsr r2

;will work
jsr label1
jsr *r2

;will work
rts
stop

;wont work, too much operands or comma was not included
mov label1 ,label1 #1 
cmp label1 ,label1 #1 
add label1 ,label1 #1 
sub label1 ,label1 #1 
lea label1 ,label1 #1 
clr lanel1 ,#1
not lanel1 ,#1
inc lanel1 ,#1
dec lanel1 ,#1
jmp lanel1 ,#1
bne lanel1 ,#1
red lanel1 ,#1
prn lanel1 ,#1
jsr lanel1 ,#1
mov label1 ,label1, #1 
cmp label1 ,label1, #1 
add label1 ,label1, #1 
sub label1 ,label1 ,#1 
lea label1 ,label1 ,#1 
clr lanel1 #1
not lanel1 #1
inc lanel1 #1
dec lanel1 #1
jmp lanel1 #1
bne lanel1 #1
red lanel1 #1
prn lanel1 #1
jsr lanel1 #1
rts #1
stop #1
rts ,#1
stop ,#1

;will work
label1: .data 1

;wont work double label initialization
label1: .data 2
.extern label1

	;will work:
	.extern label2
	.extern label2

;wont work:
	.extern 1label2
	.extern 1label%2
	.extern label2 label3
	

;wont work range problems
.data -1,-9999999999999
.data -1,9999999999999

;wont work not an intiger
.data -1,cvb,2
.data -1,45.3,4

;will work
.data 1,2 ,1, 1 , 1

;wont work comma problems
.data 1,,1
.data 1 ,,1
.data 1,, 1
.data 1, ,1
.data 1 , ,1
.data 1 ,, 1
.data 1, , 1
.data 1 , , 1
;wont work comma problems
.data 1,1,,1
.data 1 ,1,,1
.data 1, 1,,1
.data 1 , 1,,1
;wont work comma problems
.data 1,1 ,,1
.data 1 ,1 ,,1
.data 1, 1 ,,1
.data 1 , 1 ,,1
;wont work comma problems
.data 1,1,, 1
.data 1 ,1,, 1
.data 1, 1,, 1
.data 1 , 1,, 1
;wont work comma problems
.data 1,1, ,1
.data 1 ,1, ,1
.data 1, 1, ,1
.data 1 , 1, ,1
;wont work comma problems
.data 1,1 , ,1
.data 1 ,1 , ,1
.data 1, 1 , ,1
.data 1 , 1 , ,1
;wont work comma problems
.data 1,1 ,, 1
.data 1 ,1 ,, 1
.data 1, 1 ,, 1
.data 1 , 1 ,, 1
;wont work comma problems
.data 1,1, , 1
.data 1 ,1, , 1
.data 1, 1, , 1
.data 1 , 1, , 1
;wont work comma problems
.data 1,1 , , 1
.data 1 ,1 , , 1
.data 1, 1 , , 1
.data 1 , 1 , , 1
;wont work comma problems
.data 1,1,
.data 1,1 ,


;will work
.string ""
.string "asf"
.string "as d  dsa$%%"

;wont work
.string "dsfs"dlkfj"
.string ,"dsfsdf"
.string """"
.string "dsfsd" fsdfds
.string "sdfsadfsdf" "dfs"
.string "	"

;wont work illigal label
9elklk: stop
#elklk: stop
el##klk: stop
rrrrrrrrrrrrrrrrrrrrrrrrrelklkrrrrrrrrrrrrr: stop
fhsjkh : stop

;wont work line lenght
.data 1,2,3,4,5,67,8,9,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,,8,8,8,8,8,8,8,8,8,8,8,8,8

;wont work illegal command
label3: go #r1
go label1
go *r1
go r3

;wont work saved word
r0:.string "asf"
r1:.string "asf"
r2:.string "asf"
r3:.string "asf"
r4:.string "asf"
r5:.string "asf"
r6:.string "asf"
r7:.string "asf"
mov: .string "asf"
cmp: .string "asf"
add: .string "asf"
sub: .string "asf"
lea: .string "asf"
clr: .string "asf"
not: .string "asf"
inc: .string "asf"
dec: .string "asf"
jmp: .string "asf"
bne: .string "asf"
red: .string "asf"
prn: .string "asf"
jsr: .string "asf"
