        IDEAL
        MODEL large
        P386

PUBLIC C hzpic, C font

SEGMENT kakka0 para use16 private 'FAR_DATA'
LABEL hzpic BYTE
INCLUDE 'hoi.in0'
ENDS
SEGMENT kakka1 byte use16 private 'FAR_DATA'
INCLUDE 'hoi.in1'
ENDS

SEGMENT asdf byte use16 private 'FAR_DATA'
LABEL font byte
INCLUDE 'fona.inc'

dw	1500*5 dup(?)

ENDS
END
