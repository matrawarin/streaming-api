savedcmd_../../bin/driver/xctl10.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o ../../bin/driver/xctl10.o @../../bin/driver/xctl10.mod  ; /usr/src/linux-headers-6.13.5-lexne/tools/objtool/objtool --hacks=jump_label --hacks=noinstr --hacks=skylake --ibt --retpoline --rethunk --stackval --static-call --uaccess --prefix=16  --link  --module ../../bin/driver/xctl10.o

../../bin/driver/xctl10.o: $(wildcard /usr/src/linux-headers-6.13.5-lexne/tools/objtool/objtool)
