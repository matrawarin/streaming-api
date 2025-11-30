
PROJECT=redq

LIBC=-lcrypto -lcrypt -lgcrypt -lssl -lm -lc -ldl -lanl -lz
CC= gcc
PROJECT_DIR=$(shell pwd)
DFLAGS=-c -Wall 
CFLAGS= -L"/usr/lib/x86_64-linux-gnu/" -DEXEC_PROC=1 -w 
LIBCRT=$(LIBC) `pkg-config --cflags --libs gtk+-3.0  libsoup-2.4 webkit2gtk-4.1` 
LIBS0=-lrpc64 -ldbc64 -lgui64  


LIBINC_DEF=$(PROJECT_DIR)/include

PROJECT00=genkeys
PROJECT01=localsign_trusted
PROJECT02=localsign_incmedia
PROJECT03=localsign_accept
PROJECT04=crlserials
PROJECT05=P7Pack





LIBRPC_PROJ=librpc64.so
LIBGUI_PROJ=libgui64.so
LIBDBC_PROJ=libdbc64.so

PROJECT_CLI=clientx64
PROJECT_SRV=serverx64
PROJECT_ERP=erpx64

SIGN64_DIR=sign64
LIBRPC64_DIR=librpc64
LIBGUI64_DIR=libgui64
LIBGUI64_DIR=libdbc64

PROJECT_CLI_DIR=clientx64
PROJECT_SRV_DIR=serverx64
PROJECT_ERP_DIR=erpx64

OUTPUT_DIR=build64
#$(PROJECT05)
all:  $(LIBRPC_PROJ) $(LIBGUI_PROJ) $(LIBDBC_PROJ)  $(PROJECT00) $(PROJECT01) $(PROJECT02) $(PROJECT03) $(PROJECT04) $(PROJECT_CLI) $(PROJECT_SRV) $(PROJECT_ERP)
##	$(CC) -o $(PROJECT) $(PROJECT).c $(LIBC)

###################################################################################################################################################


#                                      LIBRARY PROJECT


###################################################################################################################################################
LIBCPX=library


OBJDIRRPC_SRC=$(LIBCPX)/$(LIBERP64_DIR)
OBJLIBRPC_SRC=$(shell find $(OBJDIRRPC_SRC) -name '*.c')
OBJLIBRPC_OBJ=$(OBJLIBRPC_SRC:%.c=%.o)

%.o:%.c
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ $< 
$(LIBRPC_PROJ): $(OBJLIBRPC_OBJ)
	$(CC) -shared $(OBJLIBRPC_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) -L"$(OUTPUT_DIR)/$(OUTOUT_DIR)" $(CFLAGS)  -o $(OUTPUT_DIR)/$@

OBJDIRDBC_SRC=$(LIBCPX)/$(LIBDBC64_DIR)
OBJLIBDBC_SRC=$(shell find $(OBJDIRDBC_SRC) -name '*.c')
OBJLIBDBC_OBJ=$(OBJLIBDBC_SRC:%.c=%.o)

#%.o:%.c
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ $< 
$(LIBDBC_PROJ): $(OBJLIBDBC_OBJ)
	$(CC) -shared $(OBJLIBDBC_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) -L"$(OUTPUT_DIR)/$(OUTOUT_DIR)" $(CFLAGS)  -o $(OUTPUT_DIR)/$@

OBJDIRGUI_SRC=$(LIBCPX)/$(LIBGUI64_DIR)
OBJLIBGUI_SRC=$(shell find $(OBJDIRGUI_SRC) -name '*.c')
OBJLIBGUI_OBJ=$(OBJLIBGUI_SRC:%.c=%.o)

#%.o:%.c
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ $< 
$(LIBGUI_PROJ): $(OBJLIBGUI_OBJ)
	$(CC) -shared $(OBJLIBGUI_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) -L"$(OUTPUT_DIR)/$(OUTOUT_DIR)" $(CFLAGS)  -o $(OUTPUT_DIR)/$@


###################################################################################################################################################


#                                      SIGN64 PROJECT


###################################################################################################################################################


OBJTESTW00=$(SIGN64_DIR)/genkeys/main.o
$(SIGN64_DIR)/genkeys/main.o:
	$(CC) -I$(LIBINC_DEF) -I"$(PROJECT_DIR)/$(SIGN64_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)"  $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ $(SIGN64_DIR)/genkeys/main.c
$(PROJECT00): $(OBJTESTW00)
	$(CC) $(OBJTESTW00) -I$(LIBINC_DEF) -I$(PROJECT_DIR)/$(SIGN64_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@

OBJTESTW01=$(SIGN64_DIR)/LocalSign_Trusted/main.o
$(SIGN64_DIR)/LocalSign_Trusted/main.o:
	$(CC) -I$(LIBINC_DEF) -I"$(PROJECT_DIR)/$(SIGN64_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)"  $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ $(SIGN64_DIR)/LocalSign_Trusted/main.c
$(PROJECT01): $(OBJTESTW01)
	$(CC) $(OBJTESTW01) -I$(LIBINC_DEF) -I$(PROJECT_DIR)/$(SIGN64_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@


OBJTESTW02=$(SIGN64_DIR)/LocalSign_IncMedia/main.o
$(SIGN64_DIR)/LocalSign_IncMedia/main.o:
	$(CC) -I$(LIBINC_DEF) -I"$(PROJECT_DIR)/$(SIGN64_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)"  $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ $(SIGN64_DIR)/LocalSign_IncMedia/main.c
$(PROJECT02): $(OBJTESTW02)
	$(CC) $(OBJTESTW02) -I$(LIBINC_DEF) -I$(PROJECT_DIR)/$(SIGN64_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@


OBJTESTW03=$(SIGN64_DIR)/LocalSign_Accept/main.o
$(SIGN64_DIR)/LocalSign_Accept/main.o:
	$(CC) -I$(LIBINC_DEF) -I"$(PROJECT_DIR)/$(SIGN64_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)"  $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ $(SIGN64_DIR)/LocalSign_Accept/main.c
$(PROJECT03): $(OBJTESTW03)
	$(CC) $(OBJTESTW03) -I$(LIBINC_DEF) -I$(PROJECT_DIR)/$(SIGN64_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@


OBJTESTW04=$(SIGN64_DIR)/CRLSerials/main.o
$(SIGN64_DIR)/CRLSerials/main.o:
	$(CC) -I$(LIBINC_DEF) -I"$(PROJECT_DIR)/$(SIGN64_DIR)/include" -L"$(PROJECT_DIR)" -L"$(PROJECT_DIR)/$(OUTPUT_DIR)"  $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ $(SIGN64_DIR)/CRLSerials/main.c
$(PROJECT04): $(OBJTESTW04)
	$(CC) $(OBJTESTW04) -I$(LIBINC_DEF) -I$(PROJECT_DIR)/$(SIGN64_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@


###################################################################################################################################################


#                                      EXEC PROJECT


###################################################################################################################################################
EXECPX=runexec

OBJDIRCLI_SRC=$(EXECPX)/$(PROJECT_CLI_DIR)
OBJLIBCLI_SRC=$(shell find $(OBJDIRCLI_SRC) -name '*.c')
OBJLIBCLI_OBJ=$(OBJLIBCLI_SRC:%.c=%.o)
#OBJREDS=reds/main.o	
#reds/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ reds/main.c
$(PROJECT_CLI): $(OBJLIBCLI_OBJ)
	$(CC) $(OBJLIBCLI_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@


OBJDIRSRV_SRC=$(EXECPX)/$(PROJECT_SRV_DIR)
OBJLIBSRV_SRC=$(shell find $(OBJDIRSRV_SRC) -name '*.c')
OBJLIBSRV_OBJ=$(OBJLIBSRV_SRC:%.c=%.o)
#OBJREDS=reds/main.o	
#reds/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ reds/main.c
$(PROJECT_SRV): $(OBJLIBSRV_OBJ)
	$(CC) $(OBJLIBSRV_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@


OBJDIRERP_SRC=$(EXECPX)/$(PROJECT_ERP_DIR)
OBJLIBERP_SRC=$(shell find $(OBJDIRERP_SRC) -name '*.c')
OBJLIBERP_OBJ=$(OBJLIBERP_SRC:%.c=%.o)
#OBJREDS=reds/main.o	
#reds/main.o:
#	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)"  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ reds/main.c
$(PROJECT_ERP): $(OBJLIBERP_OBJ)
	$(CC) $(OBJLIBERP_OBJ) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR)  -L"$(PROJECT_DIR)/$(OUTPUT_DIR)" $(CFLAGS) $(LIBCRT) $(LIBS0) $(LIBRMD) -o $(OUTPUT_DIR)/$@

clean: 
	rm -rf $(OUTPUT_DIR)/$(PROJECT00) $(SIGN64_DIR)/genkeys/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECT01) $(SIGN64_DIR)/LocalSign_Trusted/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECT02) $(SIGN64_DIR)/LocalSign_IncMedia/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECT03) $(SIGN64_DIR)/LocalSign_Accept/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECT04) $(SIGN64_DIR)/CRLSerials/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECT05) $(SIGN64_DIR)/P7Pack/*.o
	rm -rf $(OUTPUT_DIR)/certs/*.*
	rm -rf $(OUTPUT_DIR)/$(LIBRPC_PROJ) $(LIBCPX)/$(LIBRPC64_DIR)/*.o
	rm -rf $(OUTPUT_DIR)/$(LIBGUI_PROJ) $(LIBCPX)/$(LIBGUI64_DIR)/*.o
	rm -rf $(OUTPUT_DIR)/$(LIBDBC_PROJ) $(LIBCPX)/$(LIBGUI64_DIR)/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECT_CLI) $(EXECPX)/$(PROJECT_CLI_DIR)/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECT_SRV) $(EXECPX)/$(PROJECT_SRV_DIR)/*.o
	rm -rf $(OUTPUT_DIR)/$(PROJECT_ERP) $(EXECPX)/$(PROJECT_ERP_DIR)/*.o				
exp:
	LD_LIBRARY_PATH=$(PROJECT_DIR)/bin:$LD_LIBRARY_PATH
	echo $(LD_LIBRARY_PATH)





