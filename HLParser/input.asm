DATA SECTION

CODE SECTION
START:
PUSH EBP
MOV EBP, ESP

SUB ESP, 4

PUSH 3

PUSH 4

POP EAX
POP EDX
ADD EAX, EDX
PUSH EAX

PUSH 55

PUSH 4

POP EAX
POP EDX
IMUL EAX, EDX
PUSH EAX

PUSH 100

PUSH 2

POP EAX
POP EDX
IMUL EAX, EDX
PUSH EAX

POP EAX
POP EDX
SUB EDX, EAX
PUSH EDX

POP EAX
POP EDX
ADD EAX, EDX
PUSH EAX

PUSH 3

PUSH 4

POP EAX
POP EDX
IMUL EAX, EDX
PUSH EAX

POP EAX
POP EDX
ADD EAX, EDX
PUSH EAX

POP EAX
MOV [EBP - 4], EAX

ADD ESP, 4
MOV ESP, EBP
POP EBP
MOV EAX, 0
RET

