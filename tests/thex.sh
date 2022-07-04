#!/bin/bash
hexdump -Cv binario17.bin > my.hex
hexdump -Cv depois/binario17.bin > certo.hex
diff my.hex certo.hex
