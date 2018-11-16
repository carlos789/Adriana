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
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DSPICACEL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DSPICACEL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../MSPI1.c ../IMU.c ../RF.c ../UARTSD.c ../MemSPI.c ../main7.c "C:/Diseño Final/esclavo/drv_spi_16bit_v2.c" "C:/Diseño Final/esclavo/fileio.c" "C:/Diseño Final/esclavo/system.c" "C:/Diseño Final/esclavo/sd_spi.c"

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/MSPI1.o ${OBJECTDIR}/_ext/1472/IMU.o ${OBJECTDIR}/_ext/1472/RF.o ${OBJECTDIR}/_ext/1472/UARTSD.o ${OBJECTDIR}/_ext/1472/MemSPI.o ${OBJECTDIR}/_ext/1472/main7.o ${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o ${OBJECTDIR}/_ext/1164375339/fileio.o ${OBJECTDIR}/_ext/1164375339/system.o ${OBJECTDIR}/_ext/1164375339/sd_spi.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/MSPI1.o.d ${OBJECTDIR}/_ext/1472/IMU.o.d ${OBJECTDIR}/_ext/1472/RF.o.d ${OBJECTDIR}/_ext/1472/UARTSD.o.d ${OBJECTDIR}/_ext/1472/MemSPI.o.d ${OBJECTDIR}/_ext/1472/main7.o.d ${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o.d ${OBJECTDIR}/_ext/1164375339/fileio.o.d ${OBJECTDIR}/_ext/1164375339/system.o.d ${OBJECTDIR}/_ext/1164375339/sd_spi.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/MSPI1.o ${OBJECTDIR}/_ext/1472/IMU.o ${OBJECTDIR}/_ext/1472/RF.o ${OBJECTDIR}/_ext/1472/UARTSD.o ${OBJECTDIR}/_ext/1472/MemSPI.o ${OBJECTDIR}/_ext/1472/main7.o ${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o ${OBJECTDIR}/_ext/1164375339/fileio.o ${OBJECTDIR}/_ext/1164375339/system.o ${OBJECTDIR}/_ext/1164375339/sd_spi.o

# Source Files
SOURCEFILES=../MSPI1.c ../IMU.c ../RF.c ../UARTSD.c ../MemSPI.c ../main7.c C:/Diseño Final/esclavo/drv_spi_16bit_v2.c C:/Diseño Final/esclavo/fileio.c C:/Diseño Final/esclavo/system.c C:/Diseño Final/esclavo/sd_spi.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/DSPICACEL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512GM304
MP_LINKER_FILE_OPTION=,--script=p33EP512GM304.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/MSPI1.o: ../MSPI1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MSPI1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MSPI1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MSPI1.c  -o ${OBJECTDIR}/_ext/1472/MSPI1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MSPI1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MSPI1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/IMU.o: ../IMU.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/IMU.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/IMU.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../IMU.c  -o ${OBJECTDIR}/_ext/1472/IMU.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/IMU.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/IMU.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/RF.o: ../RF.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/RF.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/RF.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../RF.c  -o ${OBJECTDIR}/_ext/1472/RF.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/RF.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/RF.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/UARTSD.o: ../UARTSD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/UARTSD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/UARTSD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../UARTSD.c  -o ${OBJECTDIR}/_ext/1472/UARTSD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/UARTSD.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/UARTSD.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/MemSPI.o: ../MemSPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MemSPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MemSPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MemSPI.c  -o ${OBJECTDIR}/_ext/1472/MemSPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MemSPI.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MemSPI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/main7.o: ../main7.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main7.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main7.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main7.c  -o ${OBJECTDIR}/_ext/1472/main7.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main7.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main7.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o: C:/Diseño\ Final/esclavo/drv_spi_16bit_v2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1164375339" 
	@${RM} ${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "C:/Diseño Final/esclavo/drv_spi_16bit_v2.c"  -o ${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1164375339/fileio.o: C:/Diseño\ Final/esclavo/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1164375339" 
	@${RM} ${OBJECTDIR}/_ext/1164375339/fileio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1164375339/fileio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "C:/Diseño Final/esclavo/fileio.c"  -o ${OBJECTDIR}/_ext/1164375339/fileio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1164375339/fileio.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1164375339/fileio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1164375339/system.o: C:/Diseño\ Final/esclavo/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1164375339" 
	@${RM} ${OBJECTDIR}/_ext/1164375339/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1164375339/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "C:/Diseño Final/esclavo/system.c"  -o ${OBJECTDIR}/_ext/1164375339/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1164375339/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1164375339/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1164375339/sd_spi.o: C:/Diseño\ Final/esclavo/sd_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1164375339" 
	@${RM} ${OBJECTDIR}/_ext/1164375339/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1164375339/sd_spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "C:/Diseño Final/esclavo/sd_spi.c"  -o ${OBJECTDIR}/_ext/1164375339/sd_spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1164375339/sd_spi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1164375339/sd_spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/MSPI1.o: ../MSPI1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MSPI1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MSPI1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MSPI1.c  -o ${OBJECTDIR}/_ext/1472/MSPI1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MSPI1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MSPI1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/IMU.o: ../IMU.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/IMU.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/IMU.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../IMU.c  -o ${OBJECTDIR}/_ext/1472/IMU.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/IMU.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/IMU.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/RF.o: ../RF.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/RF.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/RF.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../RF.c  -o ${OBJECTDIR}/_ext/1472/RF.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/RF.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/RF.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/UARTSD.o: ../UARTSD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/UARTSD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/UARTSD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../UARTSD.c  -o ${OBJECTDIR}/_ext/1472/UARTSD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/UARTSD.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/UARTSD.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/MemSPI.o: ../MemSPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MemSPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MemSPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MemSPI.c  -o ${OBJECTDIR}/_ext/1472/MemSPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MemSPI.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MemSPI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/main7.o: ../main7.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main7.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main7.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main7.c  -o ${OBJECTDIR}/_ext/1472/main7.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main7.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main7.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o: C:/Diseño\ Final/esclavo/drv_spi_16bit_v2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1164375339" 
	@${RM} ${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "C:/Diseño Final/esclavo/drv_spi_16bit_v2.c"  -o ${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1164375339/drv_spi_16bit_v2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1164375339/fileio.o: C:/Diseño\ Final/esclavo/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1164375339" 
	@${RM} ${OBJECTDIR}/_ext/1164375339/fileio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1164375339/fileio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "C:/Diseño Final/esclavo/fileio.c"  -o ${OBJECTDIR}/_ext/1164375339/fileio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1164375339/fileio.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1164375339/fileio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1164375339/system.o: C:/Diseño\ Final/esclavo/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1164375339" 
	@${RM} ${OBJECTDIR}/_ext/1164375339/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1164375339/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "C:/Diseño Final/esclavo/system.c"  -o ${OBJECTDIR}/_ext/1164375339/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1164375339/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1164375339/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1164375339/sd_spi.o: C:/Diseño\ Final/esclavo/sd_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1164375339" 
	@${RM} ${OBJECTDIR}/_ext/1164375339/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1164375339/sd_spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "C:/Diseño Final/esclavo/sd_spi.c"  -o ${OBJECTDIR}/_ext/1164375339/sd_spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1164375339/sd_spi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -mlarge-data -O0 -I"../../" -I"../../../microchip_solutions_v2013-06-15/Microchip/Include/Transceivers/MRF24J40" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1164375339/sd_spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/DSPICACEL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/DSPICACEL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../"     -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/DSPICACEL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/DSPICACEL.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"../" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/DSPICACEL.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
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

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
