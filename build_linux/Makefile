#
#  TOPPERS athrill
#      Athrill is a CPU emulator. Athrill was developed to easily
#      execute and debug embedded programs on virtual microcomputer.
#
#  Copyright (C) 2019 by Center for Embedded Computing Systems
#                Graduate School of Informatics, Nagoya Univ., JAPAN
#  Copyright (C) 2019 by ESM, Inc.
#
#
#  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
#  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
#  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
#  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
#      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
#      スコード中に含まれていること．
#  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
#      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
#      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
#      の無保証規定を掲載すること．
#  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
#      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
#      と．
#    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
#        作権表示，この利用条件および下記の無保証規定を掲載すること．
#    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
#        報告すること．
#  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
#      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
#      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
#      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
#      免責すること．
#
#  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
#  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
#  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
#  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
#  の責任を負わない．
#
#  $Id$
#

include Makefile.env

LIBCUI		:= libcui.a
LIBCPU		:= libcpu.a
LIBFPU		:= libfpu.a
LIBCAN		:= libcan.a
LIBDEVICE	:= libdevice.a
LIBBUS		:= libbus.a
LIBMPU		:= libmpu.a
LIBLOADER	:= libloader.a
LIBSTD		:= libstd.a
LIBMAIN		:= libmain.a
LIBMROS		:= libmros.a


AROBJS		:= $(LIBMAIN)
AROBJS		+= $(LIBCUI)
AROBJS		+= $(LIBCPU)
AROBJS		+= $(LIBFPU)
AROBJS		+= $(LIBDEVICE)
AROBJS		+= $(LIBBUS)
AROBJS		+= $(LIBMPU)
AROBJS		+= $(LIBCAN)
AROBJS		+= $(LIBLOADER)
AROBJS		+= $(LIBSTD)
AROBJS		+= $(LIBMROS)

all: $(AROBJS)
	$(RANLIB) $(AROBJS)
	$(GCC) -O3 $(LFLAGS) $(AROBJS) -o $(TARGET)  $(LIBS)
	$(CP) $(TARGET) $(INSTALL_DIR)/
	chmod +x $(INSTALL_DIR)/$(TARGET)

build_all:
	make -f $(CORE_MKDIR)/Makefile.main all
	make -f $(CORE_MKDIR)/Makefile.cui all
	make -f $(TARGET_MKDIR)/Makefile.cpu all
	make -f $(TARGET_MKDIR)/Makefile.device all
	make -f $(CORE_MKDIR)/Makefile.bus all
	make -f $(CORE_MKDIR)/Makefile.loader all
	make -f $(TARGET_MKDIR)/Makefile.mpu all
	make -f $(CORE_MKDIR)/Makefile.std all
	make -f $(CORE_MKDIR)/Makefile.mros all
	$(RANLIB) $(AROBJS)
	$(GCC) -O3 $(LFLAGS) $(AROBJS) -o $(TARGET)  $(LIBS)
	$(CP) $(TARGET) $(INSTALL_DIR)/
	chmod +x $(INSTALL_DIR)/$(TARGET)

$(LIBMAIN):
	make -f $(CORE_MKDIR)/Makefile.main all

$(LIBCUI): 
	make -f $(CORE_MKDIR)/Makefile.cui all

$(LIBCPU): 
	make -f $(TARGET_MKDIR)/Makefile.cpu all

$(LIBFPU): 
	make -f $(TARGET_MKDIR)/Makefile.fpu all

$(LIBDEVICE): 
	make -f $(TARGET_MKDIR)/Makefile.device all

$(LIBBUS): 
	make -f $(CORE_MKDIR)/Makefile.bus all

$(LIBLOADER):
	make -f $(CORE_MKDIR)/Makefile.loader all

$(LIBMPU): 
	make -f $(TARGET_MKDIR)/Makefile.mpu all

$(LIBSTD):
	make -f $(CORE_MKDIR)/Makefile.std all

$(LIBMROS):
	make -f $(CORE_MKDIR)/Makefile.mros all

clean:
	make -f $(TARGET_MKDIR)/Makefile.cpu clean
	make -f $(TARGET_MKDIR)/Makefile.fpu clean
	make -f $(TARGET_MKDIR)/Makefile.device clean
	make -f $(CORE_MKDIR)/Makefile.bus clean
	make -f $(CORE_MKDIR)/Makefile.cui clean
	make -f $(CORE_MKDIR)/Makefile.loader clean
	make -f $(TARGET_MKDIR)/Makefile.mpu clean
	make -f $(CORE_MKDIR)/Makefile.std clean
	make -f $(CORE_MKDIR)/Makefile.main clean
	make -f $(CORE_MKDIR)/Makefile.mros clean
	$(RM) -f $(TARGET) 
	$(RM) -f $(INSTALL_DIR)/$(TARGET)
		
list:
	@echo $(AROBJS)

