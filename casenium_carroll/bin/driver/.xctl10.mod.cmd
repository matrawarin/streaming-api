savedcmd_../../bin/driver/xctl10.mod := printf '%s\n'   bcd10.o ctl10.o gen10.o | awk '!x[$$0]++ { print("./"$$0) }' > ../../bin/driver/xctl10.mod
