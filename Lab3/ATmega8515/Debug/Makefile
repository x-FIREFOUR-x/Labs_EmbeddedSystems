###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = ATmega8515
COMPILER = "AVRASM (Proteus)"
TARGET = Debug
SHELL = C:\WINDOWS\system32\cmd.exe

## Tools general options
ASMFLAGS=-fO -i"C:\Program Files (x86)\Labcenter Electronics\Proteus 8 Professional\Tools\AVRASM\appnotes\m8515def.inc"

## Processing Tools
ASM=avrasm2.exe
#  C:/Users/0E46~1/AppData/Local/Temp/VSM Studio/7bda63e5f5ec42cba8ea5b960f6a04ff/ATmega8515/Debug/Debug.obj: TOOL=ASM
#     C:/Users/0E46~1/AppData/Local/Temp/VSM Studio/7bda63e5f5ec42cba8ea5b960f6a04ff/ATmega8515/main.asm

# Build tree:
all:	Debug

Debug:	Debug.obj

Debug.obj:	../main.asm 
	$(ASM) $(ASMFLAGS)  -o "Debug.obj" -l Debug.lst "..\main.asm"

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf Debug.lst

# cleanup - delete all generated files
clean:	tidy
	rm -rf Debug.obj
