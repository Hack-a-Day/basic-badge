#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/badge1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/badge1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/basic/tokenizer.c src/basic/ubasic.c src/Z80/hwz.c src/Z80/iosim.c src/Z80/sim1.c src/Z80/sim2.c src/Z80/sim3.c src/Z80/sim4.c src/Z80/sim5.c src/Z80/sim6.c src/Z80/sim7.c src/Z80/simfun.c src/Z80/simglb.c src/disp.c src/hw.c src/images.c src/vt100.c src/main.c src/splash.c src/tetrapuzz.c src/box_game.c src/menu.c src/tune_player.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/basic/tokenizer.o ${OBJECTDIR}/src/basic/ubasic.o ${OBJECTDIR}/src/Z80/hwz.o ${OBJECTDIR}/src/Z80/iosim.o ${OBJECTDIR}/src/Z80/sim1.o ${OBJECTDIR}/src/Z80/sim2.o ${OBJECTDIR}/src/Z80/sim3.o ${OBJECTDIR}/src/Z80/sim4.o ${OBJECTDIR}/src/Z80/sim5.o ${OBJECTDIR}/src/Z80/sim6.o ${OBJECTDIR}/src/Z80/sim7.o ${OBJECTDIR}/src/Z80/simfun.o ${OBJECTDIR}/src/Z80/simglb.o ${OBJECTDIR}/src/disp.o ${OBJECTDIR}/src/hw.o ${OBJECTDIR}/src/images.o ${OBJECTDIR}/src/vt100.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/splash.o ${OBJECTDIR}/src/tetrapuzz.o ${OBJECTDIR}/src/box_game.o ${OBJECTDIR}/src/menu.o ${OBJECTDIR}/src/tune_player.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/basic/tokenizer.o.d ${OBJECTDIR}/src/basic/ubasic.o.d ${OBJECTDIR}/src/Z80/hwz.o.d ${OBJECTDIR}/src/Z80/iosim.o.d ${OBJECTDIR}/src/Z80/sim1.o.d ${OBJECTDIR}/src/Z80/sim2.o.d ${OBJECTDIR}/src/Z80/sim3.o.d ${OBJECTDIR}/src/Z80/sim4.o.d ${OBJECTDIR}/src/Z80/sim5.o.d ${OBJECTDIR}/src/Z80/sim6.o.d ${OBJECTDIR}/src/Z80/sim7.o.d ${OBJECTDIR}/src/Z80/simfun.o.d ${OBJECTDIR}/src/Z80/simglb.o.d ${OBJECTDIR}/src/disp.o.d ${OBJECTDIR}/src/hw.o.d ${OBJECTDIR}/src/images.o.d ${OBJECTDIR}/src/vt100.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/splash.o.d ${OBJECTDIR}/src/tetrapuzz.o.d ${OBJECTDIR}/src/box_game.o.d ${OBJECTDIR}/src/menu.o.d ${OBJECTDIR}/src/tune_player.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/basic/tokenizer.o ${OBJECTDIR}/src/basic/ubasic.o ${OBJECTDIR}/src/Z80/hwz.o ${OBJECTDIR}/src/Z80/iosim.o ${OBJECTDIR}/src/Z80/sim1.o ${OBJECTDIR}/src/Z80/sim2.o ${OBJECTDIR}/src/Z80/sim3.o ${OBJECTDIR}/src/Z80/sim4.o ${OBJECTDIR}/src/Z80/sim5.o ${OBJECTDIR}/src/Z80/sim6.o ${OBJECTDIR}/src/Z80/sim7.o ${OBJECTDIR}/src/Z80/simfun.o ${OBJECTDIR}/src/Z80/simglb.o ${OBJECTDIR}/src/disp.o ${OBJECTDIR}/src/hw.o ${OBJECTDIR}/src/images.o ${OBJECTDIR}/src/vt100.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/splash.o ${OBJECTDIR}/src/tetrapuzz.o ${OBJECTDIR}/src/box_game.o ${OBJECTDIR}/src/menu.o ${OBJECTDIR}/src/tune_player.o

# Source Files
SOURCEFILES=src/basic/tokenizer.c src/basic/ubasic.c src/Z80/hwz.c src/Z80/iosim.c src/Z80/sim1.c src/Z80/sim2.c src/Z80/sim3.c src/Z80/sim4.c src/Z80/sim5.c src/Z80/sim6.c src/Z80/sim7.c src/Z80/simfun.c src/Z80/simglb.c src/disp.c src/hw.c src/images.c src/vt100.c src/main.c src/splash.c src/tetrapuzz.c src/box_game.c src/menu.c src/tune_player.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/badge1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX370F512H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/basic/tokenizer.o: src/basic/tokenizer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/basic" 
	@${RM} ${OBJECTDIR}/src/basic/tokenizer.o.d 
	@${RM} ${OBJECTDIR}/src/basic/tokenizer.o 
	@${FIXDEPS} "${OBJECTDIR}/src/basic/tokenizer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/basic/tokenizer.o.d" -o ${OBJECTDIR}/src/basic/tokenizer.o src/basic/tokenizer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/basic/ubasic.o: src/basic/ubasic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/basic" 
	@${RM} ${OBJECTDIR}/src/basic/ubasic.o.d 
	@${RM} ${OBJECTDIR}/src/basic/ubasic.o 
	@${FIXDEPS} "${OBJECTDIR}/src/basic/ubasic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/basic/ubasic.o.d" -o ${OBJECTDIR}/src/basic/ubasic.o src/basic/ubasic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/hwz.o: src/Z80/hwz.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/hwz.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/hwz.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/hwz.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/hwz.o.d" -o ${OBJECTDIR}/src/Z80/hwz.o src/Z80/hwz.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/iosim.o: src/Z80/iosim.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/iosim.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/iosim.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/iosim.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/iosim.o.d" -o ${OBJECTDIR}/src/Z80/iosim.o src/Z80/iosim.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim1.o: src/Z80/sim1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim1.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim1.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim1.o.d" -o ${OBJECTDIR}/src/Z80/sim1.o src/Z80/sim1.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim2.o: src/Z80/sim2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim2.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim2.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim2.o.d" -o ${OBJECTDIR}/src/Z80/sim2.o src/Z80/sim2.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim3.o: src/Z80/sim3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim3.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim3.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim3.o.d" -o ${OBJECTDIR}/src/Z80/sim3.o src/Z80/sim3.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim4.o: src/Z80/sim4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim4.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim4.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim4.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim4.o.d" -o ${OBJECTDIR}/src/Z80/sim4.o src/Z80/sim4.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim5.o: src/Z80/sim5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim5.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim5.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim5.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim5.o.d" -o ${OBJECTDIR}/src/Z80/sim5.o src/Z80/sim5.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim6.o: src/Z80/sim6.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim6.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim6.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim6.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim6.o.d" -o ${OBJECTDIR}/src/Z80/sim6.o src/Z80/sim6.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim7.o: src/Z80/sim7.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim7.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim7.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim7.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim7.o.d" -o ${OBJECTDIR}/src/Z80/sim7.o src/Z80/sim7.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/simfun.o: src/Z80/simfun.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/simfun.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/simfun.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/simfun.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/simfun.o.d" -o ${OBJECTDIR}/src/Z80/simfun.o src/Z80/simfun.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/simglb.o: src/Z80/simglb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/simglb.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/simglb.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/simglb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/simglb.o.d" -o ${OBJECTDIR}/src/Z80/simglb.o src/Z80/simglb.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/disp.o: src/disp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/disp.o.d 
	@${RM} ${OBJECTDIR}/src/disp.o 
	@${FIXDEPS} "${OBJECTDIR}/src/disp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/disp.o.d" -o ${OBJECTDIR}/src/disp.o src/disp.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/hw.o: src/hw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/hw.o.d 
	@${RM} ${OBJECTDIR}/src/hw.o 
	@${FIXDEPS} "${OBJECTDIR}/src/hw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/hw.o.d" -o ${OBJECTDIR}/src/hw.o src/hw.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/images.o: src/images.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/images.o.d 
	@${RM} ${OBJECTDIR}/src/images.o 
	@${FIXDEPS} "${OBJECTDIR}/src/images.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/images.o.d" -o ${OBJECTDIR}/src/images.o src/images.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/vt100.o: src/vt100.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/vt100.o.d 
	@${RM} ${OBJECTDIR}/src/vt100.o 
	@${FIXDEPS} "${OBJECTDIR}/src/vt100.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/vt100.o.d" -o ${OBJECTDIR}/src/vt100.o src/vt100.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/main.o.d" -o ${OBJECTDIR}/src/main.o src/main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/splash.o: src/splash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/splash.o.d 
	@${RM} ${OBJECTDIR}/src/splash.o 
	@${FIXDEPS} "${OBJECTDIR}/src/splash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/splash.o.d" -o ${OBJECTDIR}/src/splash.o src/splash.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/tetrapuzz.o: src/tetrapuzz.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tetrapuzz.o.d 
	@${RM} ${OBJECTDIR}/src/tetrapuzz.o 
	@${FIXDEPS} "${OBJECTDIR}/src/tetrapuzz.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/tetrapuzz.o.d" -o ${OBJECTDIR}/src/tetrapuzz.o src/tetrapuzz.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/box_game.o: src/box_game.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/box_game.o.d 
	@${RM} ${OBJECTDIR}/src/box_game.o 
	@${FIXDEPS} "${OBJECTDIR}/src/box_game.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/box_game.o.d" -o ${OBJECTDIR}/src/box_game.o src/box_game.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/menu.o: src/menu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/menu.o.d 
	@${RM} ${OBJECTDIR}/src/menu.o 
	@${FIXDEPS} "${OBJECTDIR}/src/menu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/menu.o.d" -o ${OBJECTDIR}/src/menu.o src/menu.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/tune_player.o: src/tune_player.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tune_player.o.d 
	@${RM} ${OBJECTDIR}/src/tune_player.o 
	@${FIXDEPS} "${OBJECTDIR}/src/tune_player.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPK4Tool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/tune_player.o.d" -o ${OBJECTDIR}/src/tune_player.o src/tune_player.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/src/basic/tokenizer.o: src/basic/tokenizer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/basic" 
	@${RM} ${OBJECTDIR}/src/basic/tokenizer.o.d 
	@${RM} ${OBJECTDIR}/src/basic/tokenizer.o 
	@${FIXDEPS} "${OBJECTDIR}/src/basic/tokenizer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/basic/tokenizer.o.d" -o ${OBJECTDIR}/src/basic/tokenizer.o src/basic/tokenizer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/basic/ubasic.o: src/basic/ubasic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/basic" 
	@${RM} ${OBJECTDIR}/src/basic/ubasic.o.d 
	@${RM} ${OBJECTDIR}/src/basic/ubasic.o 
	@${FIXDEPS} "${OBJECTDIR}/src/basic/ubasic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/basic/ubasic.o.d" -o ${OBJECTDIR}/src/basic/ubasic.o src/basic/ubasic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/hwz.o: src/Z80/hwz.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/hwz.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/hwz.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/hwz.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/hwz.o.d" -o ${OBJECTDIR}/src/Z80/hwz.o src/Z80/hwz.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/iosim.o: src/Z80/iosim.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/iosim.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/iosim.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/iosim.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/iosim.o.d" -o ${OBJECTDIR}/src/Z80/iosim.o src/Z80/iosim.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim1.o: src/Z80/sim1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim1.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim1.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim1.o.d" -o ${OBJECTDIR}/src/Z80/sim1.o src/Z80/sim1.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim2.o: src/Z80/sim2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim2.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim2.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim2.o.d" -o ${OBJECTDIR}/src/Z80/sim2.o src/Z80/sim2.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim3.o: src/Z80/sim3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim3.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim3.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim3.o.d" -o ${OBJECTDIR}/src/Z80/sim3.o src/Z80/sim3.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim4.o: src/Z80/sim4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim4.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim4.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim4.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim4.o.d" -o ${OBJECTDIR}/src/Z80/sim4.o src/Z80/sim4.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim5.o: src/Z80/sim5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim5.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim5.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim5.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim5.o.d" -o ${OBJECTDIR}/src/Z80/sim5.o src/Z80/sim5.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim6.o: src/Z80/sim6.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim6.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim6.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim6.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim6.o.d" -o ${OBJECTDIR}/src/Z80/sim6.o src/Z80/sim6.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/sim7.o: src/Z80/sim7.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/sim7.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/sim7.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/sim7.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/sim7.o.d" -o ${OBJECTDIR}/src/Z80/sim7.o src/Z80/sim7.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/simfun.o: src/Z80/simfun.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/simfun.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/simfun.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/simfun.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/simfun.o.d" -o ${OBJECTDIR}/src/Z80/simfun.o src/Z80/simfun.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/Z80/simglb.o: src/Z80/simglb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/Z80" 
	@${RM} ${OBJECTDIR}/src/Z80/simglb.o.d 
	@${RM} ${OBJECTDIR}/src/Z80/simglb.o 
	@${FIXDEPS} "${OBJECTDIR}/src/Z80/simglb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/Z80/simglb.o.d" -o ${OBJECTDIR}/src/Z80/simglb.o src/Z80/simglb.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/disp.o: src/disp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/disp.o.d 
	@${RM} ${OBJECTDIR}/src/disp.o 
	@${FIXDEPS} "${OBJECTDIR}/src/disp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/disp.o.d" -o ${OBJECTDIR}/src/disp.o src/disp.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/hw.o: src/hw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/hw.o.d 
	@${RM} ${OBJECTDIR}/src/hw.o 
	@${FIXDEPS} "${OBJECTDIR}/src/hw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/hw.o.d" -o ${OBJECTDIR}/src/hw.o src/hw.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/images.o: src/images.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/images.o.d 
	@${RM} ${OBJECTDIR}/src/images.o 
	@${FIXDEPS} "${OBJECTDIR}/src/images.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/images.o.d" -o ${OBJECTDIR}/src/images.o src/images.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/vt100.o: src/vt100.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/vt100.o.d 
	@${RM} ${OBJECTDIR}/src/vt100.o 
	@${FIXDEPS} "${OBJECTDIR}/src/vt100.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/vt100.o.d" -o ${OBJECTDIR}/src/vt100.o src/vt100.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/main.o.d" -o ${OBJECTDIR}/src/main.o src/main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/splash.o: src/splash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/splash.o.d 
	@${RM} ${OBJECTDIR}/src/splash.o 
	@${FIXDEPS} "${OBJECTDIR}/src/splash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/splash.o.d" -o ${OBJECTDIR}/src/splash.o src/splash.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/tetrapuzz.o: src/tetrapuzz.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tetrapuzz.o.d 
	@${RM} ${OBJECTDIR}/src/tetrapuzz.o 
	@${FIXDEPS} "${OBJECTDIR}/src/tetrapuzz.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/tetrapuzz.o.d" -o ${OBJECTDIR}/src/tetrapuzz.o src/tetrapuzz.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/box_game.o: src/box_game.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/box_game.o.d 
	@${RM} ${OBJECTDIR}/src/box_game.o 
	@${FIXDEPS} "${OBJECTDIR}/src/box_game.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/box_game.o.d" -o ${OBJECTDIR}/src/box_game.o src/box_game.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/menu.o: src/menu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/menu.o.d 
	@${RM} ${OBJECTDIR}/src/menu.o 
	@${FIXDEPS} "${OBJECTDIR}/src/menu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/menu.o.d" -o ${OBJECTDIR}/src/menu.o src/menu.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/src/tune_player.o: src/tune_player.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/tune_player.o.d 
	@${RM} ${OBJECTDIR}/src/tune_player.o 
	@${FIXDEPS} "${OBJECTDIR}/src/tune_player.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -funroll-loops -D_SUPPRESS_PLIB_WARNING -MMD -MF "${OBJECTDIR}/src/tune_player.o.d" -o ${OBJECTDIR}/src/tune_player.o src/tune_player.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/badge1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -DPK4Tool=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/badge1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC0275F  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=PK4Tool=1,--defsym=_min_heap_size=128,--defsym=_min_stack_size=128,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/badge1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/badge1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=128,--defsym=_min_stack_size=128,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/badge1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
