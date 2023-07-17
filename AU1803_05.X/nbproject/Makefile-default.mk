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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AU1803_05.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AU1803_05.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=battery_manage_system/battery_manage_system.c battery_manage_system/SOC.c battery_manage_system/SOH.c can/can.c can/can_dbc.c can/can_dbc_mem_map_uit.c can/can_dbc_usages_cat0.c can/can_dbc_usages_cat1.c can/can_dbc_usages_cat2.c can/can_dbc_usages_mdat.c can/can_dbc_usages_sysreq.c can/can_dbc_usages_vdat.c can/can_dbc_usages_ver.c can/can_dbc_utils.c can/can_dbc_cmd.c can/can_dbc_usages_auxreq.c can/can_dbc_usages_auxresp.c can/can_dbc_usages_bmftp.c can/can_dbc_usages_ldat.c daq/daq_adc.c EventLogData/EventLog.c flash/MX25L.c i2c/i2cstate.c i2c/i2c_a1006.c main_app/mcu_init.c main_app/scheduler.c main_app/int.c main_app/version_config.c mcc_generated_files/reset.c mcc_generated_files/system.c mcc_generated_files/can1.c mcc_generated_files/clock.c mcc_generated_files/adc1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c mcc_generated_files/tmr1.c mcc_generated_files/traps.c mcc_generated_files/dma.c mcc_generated_files/tmr3.c mcc_generated_files/spi2.c mcc_generated_files/uart1.c protection/checkSafety.c security/aes.c security/sha256.c smbus/i2c_crc_table.c smbus/smbus.c smbus/smbus_cmd.c sys_time/sys_time.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/battery_manage_system/battery_manage_system.o ${OBJECTDIR}/battery_manage_system/SOC.o ${OBJECTDIR}/battery_manage_system/SOH.o ${OBJECTDIR}/can/can.o ${OBJECTDIR}/can/can_dbc.o ${OBJECTDIR}/can/can_dbc_mem_map_uit.o ${OBJECTDIR}/can/can_dbc_usages_cat0.o ${OBJECTDIR}/can/can_dbc_usages_cat1.o ${OBJECTDIR}/can/can_dbc_usages_cat2.o ${OBJECTDIR}/can/can_dbc_usages_mdat.o ${OBJECTDIR}/can/can_dbc_usages_sysreq.o ${OBJECTDIR}/can/can_dbc_usages_vdat.o ${OBJECTDIR}/can/can_dbc_usages_ver.o ${OBJECTDIR}/can/can_dbc_utils.o ${OBJECTDIR}/can/can_dbc_cmd.o ${OBJECTDIR}/can/can_dbc_usages_auxreq.o ${OBJECTDIR}/can/can_dbc_usages_auxresp.o ${OBJECTDIR}/can/can_dbc_usages_bmftp.o ${OBJECTDIR}/can/can_dbc_usages_ldat.o ${OBJECTDIR}/daq/daq_adc.o ${OBJECTDIR}/EventLogData/EventLog.o ${OBJECTDIR}/flash/MX25L.o ${OBJECTDIR}/i2c/i2cstate.o ${OBJECTDIR}/i2c/i2c_a1006.o ${OBJECTDIR}/main_app/mcu_init.o ${OBJECTDIR}/main_app/scheduler.o ${OBJECTDIR}/main_app/int.o ${OBJECTDIR}/main_app/version_config.o ${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/can1.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/dma.o ${OBJECTDIR}/mcc_generated_files/tmr3.o ${OBJECTDIR}/mcc_generated_files/spi2.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/protection/checkSafety.o ${OBJECTDIR}/security/aes.o ${OBJECTDIR}/security/sha256.o ${OBJECTDIR}/smbus/i2c_crc_table.o ${OBJECTDIR}/smbus/smbus.o ${OBJECTDIR}/smbus/smbus_cmd.o ${OBJECTDIR}/sys_time/sys_time.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/battery_manage_system/battery_manage_system.o.d ${OBJECTDIR}/battery_manage_system/SOC.o.d ${OBJECTDIR}/battery_manage_system/SOH.o.d ${OBJECTDIR}/can/can.o.d ${OBJECTDIR}/can/can_dbc.o.d ${OBJECTDIR}/can/can_dbc_mem_map_uit.o.d ${OBJECTDIR}/can/can_dbc_usages_cat0.o.d ${OBJECTDIR}/can/can_dbc_usages_cat1.o.d ${OBJECTDIR}/can/can_dbc_usages_cat2.o.d ${OBJECTDIR}/can/can_dbc_usages_mdat.o.d ${OBJECTDIR}/can/can_dbc_usages_sysreq.o.d ${OBJECTDIR}/can/can_dbc_usages_vdat.o.d ${OBJECTDIR}/can/can_dbc_usages_ver.o.d ${OBJECTDIR}/can/can_dbc_utils.o.d ${OBJECTDIR}/can/can_dbc_cmd.o.d ${OBJECTDIR}/can/can_dbc_usages_auxreq.o.d ${OBJECTDIR}/can/can_dbc_usages_auxresp.o.d ${OBJECTDIR}/can/can_dbc_usages_bmftp.o.d ${OBJECTDIR}/can/can_dbc_usages_ldat.o.d ${OBJECTDIR}/daq/daq_adc.o.d ${OBJECTDIR}/EventLogData/EventLog.o.d ${OBJECTDIR}/flash/MX25L.o.d ${OBJECTDIR}/i2c/i2cstate.o.d ${OBJECTDIR}/i2c/i2c_a1006.o.d ${OBJECTDIR}/main_app/mcu_init.o.d ${OBJECTDIR}/main_app/scheduler.o.d ${OBJECTDIR}/main_app/int.o.d ${OBJECTDIR}/main_app/version_config.o.d ${OBJECTDIR}/mcc_generated_files/reset.o.d ${OBJECTDIR}/mcc_generated_files/system.o.d ${OBJECTDIR}/mcc_generated_files/can1.o.d ${OBJECTDIR}/mcc_generated_files/clock.o.d ${OBJECTDIR}/mcc_generated_files/adc1.o.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d ${OBJECTDIR}/mcc_generated_files/tmr1.o.d ${OBJECTDIR}/mcc_generated_files/traps.o.d ${OBJECTDIR}/mcc_generated_files/dma.o.d ${OBJECTDIR}/mcc_generated_files/tmr3.o.d ${OBJECTDIR}/mcc_generated_files/spi2.o.d ${OBJECTDIR}/mcc_generated_files/uart1.o.d ${OBJECTDIR}/protection/checkSafety.o.d ${OBJECTDIR}/security/aes.o.d ${OBJECTDIR}/security/sha256.o.d ${OBJECTDIR}/smbus/i2c_crc_table.o.d ${OBJECTDIR}/smbus/smbus.o.d ${OBJECTDIR}/smbus/smbus_cmd.o.d ${OBJECTDIR}/sys_time/sys_time.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/battery_manage_system/battery_manage_system.o ${OBJECTDIR}/battery_manage_system/SOC.o ${OBJECTDIR}/battery_manage_system/SOH.o ${OBJECTDIR}/can/can.o ${OBJECTDIR}/can/can_dbc.o ${OBJECTDIR}/can/can_dbc_mem_map_uit.o ${OBJECTDIR}/can/can_dbc_usages_cat0.o ${OBJECTDIR}/can/can_dbc_usages_cat1.o ${OBJECTDIR}/can/can_dbc_usages_cat2.o ${OBJECTDIR}/can/can_dbc_usages_mdat.o ${OBJECTDIR}/can/can_dbc_usages_sysreq.o ${OBJECTDIR}/can/can_dbc_usages_vdat.o ${OBJECTDIR}/can/can_dbc_usages_ver.o ${OBJECTDIR}/can/can_dbc_utils.o ${OBJECTDIR}/can/can_dbc_cmd.o ${OBJECTDIR}/can/can_dbc_usages_auxreq.o ${OBJECTDIR}/can/can_dbc_usages_auxresp.o ${OBJECTDIR}/can/can_dbc_usages_bmftp.o ${OBJECTDIR}/can/can_dbc_usages_ldat.o ${OBJECTDIR}/daq/daq_adc.o ${OBJECTDIR}/EventLogData/EventLog.o ${OBJECTDIR}/flash/MX25L.o ${OBJECTDIR}/i2c/i2cstate.o ${OBJECTDIR}/i2c/i2c_a1006.o ${OBJECTDIR}/main_app/mcu_init.o ${OBJECTDIR}/main_app/scheduler.o ${OBJECTDIR}/main_app/int.o ${OBJECTDIR}/main_app/version_config.o ${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/can1.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/dma.o ${OBJECTDIR}/mcc_generated_files/tmr3.o ${OBJECTDIR}/mcc_generated_files/spi2.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/protection/checkSafety.o ${OBJECTDIR}/security/aes.o ${OBJECTDIR}/security/sha256.o ${OBJECTDIR}/smbus/i2c_crc_table.o ${OBJECTDIR}/smbus/smbus.o ${OBJECTDIR}/smbus/smbus_cmd.o ${OBJECTDIR}/sys_time/sys_time.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=battery_manage_system/battery_manage_system.c battery_manage_system/SOC.c battery_manage_system/SOH.c can/can.c can/can_dbc.c can/can_dbc_mem_map_uit.c can/can_dbc_usages_cat0.c can/can_dbc_usages_cat1.c can/can_dbc_usages_cat2.c can/can_dbc_usages_mdat.c can/can_dbc_usages_sysreq.c can/can_dbc_usages_vdat.c can/can_dbc_usages_ver.c can/can_dbc_utils.c can/can_dbc_cmd.c can/can_dbc_usages_auxreq.c can/can_dbc_usages_auxresp.c can/can_dbc_usages_bmftp.c can/can_dbc_usages_ldat.c daq/daq_adc.c EventLogData/EventLog.c flash/MX25L.c i2c/i2cstate.c i2c/i2c_a1006.c main_app/mcu_init.c main_app/scheduler.c main_app/int.c main_app/version_config.c mcc_generated_files/reset.c mcc_generated_files/system.c mcc_generated_files/can1.c mcc_generated_files/clock.c mcc_generated_files/adc1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c mcc_generated_files/tmr1.c mcc_generated_files/traps.c mcc_generated_files/dma.c mcc_generated_files/tmr3.c mcc_generated_files/spi2.c mcc_generated_files/uart1.c protection/checkSafety.c security/aes.c security/sha256.c smbus/i2c_crc_table.c smbus/smbus.c smbus/smbus_cmd.c sys_time/sys_time.c main.c



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/AU1803_05.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP256MC504
MP_LINKER_FILE_OPTION=,--script=p33EP256MC504.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/battery_manage_system/battery_manage_system.o: battery_manage_system/battery_manage_system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/battery_manage_system" 
	@${RM} ${OBJECTDIR}/battery_manage_system/battery_manage_system.o.d 
	@${RM} ${OBJECTDIR}/battery_manage_system/battery_manage_system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  battery_manage_system/battery_manage_system.c  -o ${OBJECTDIR}/battery_manage_system/battery_manage_system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/battery_manage_system/battery_manage_system.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/battery_manage_system/battery_manage_system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/battery_manage_system/SOC.o: battery_manage_system/SOC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/battery_manage_system" 
	@${RM} ${OBJECTDIR}/battery_manage_system/SOC.o.d 
	@${RM} ${OBJECTDIR}/battery_manage_system/SOC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  battery_manage_system/SOC.c  -o ${OBJECTDIR}/battery_manage_system/SOC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/battery_manage_system/SOC.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/battery_manage_system/SOC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/battery_manage_system/SOH.o: battery_manage_system/SOH.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/battery_manage_system" 
	@${RM} ${OBJECTDIR}/battery_manage_system/SOH.o.d 
	@${RM} ${OBJECTDIR}/battery_manage_system/SOH.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  battery_manage_system/SOH.c  -o ${OBJECTDIR}/battery_manage_system/SOH.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/battery_manage_system/SOH.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/battery_manage_system/SOH.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can.o: can/can.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can.o.d 
	@${RM} ${OBJECTDIR}/can/can.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can.c  -o ${OBJECTDIR}/can/can.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc.o: can/can_dbc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc.c  -o ${OBJECTDIR}/can/can_dbc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_mem_map_uit.o: can/can_dbc_mem_map_uit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_mem_map_uit.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_mem_map_uit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_mem_map_uit.c  -o ${OBJECTDIR}/can/can_dbc_mem_map_uit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_mem_map_uit.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_mem_map_uit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_cat0.o: can/can_dbc_usages_cat0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat0.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_cat0.c  -o ${OBJECTDIR}/can/can_dbc_usages_cat0.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_cat0.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_cat0.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_cat1.o: can/can_dbc_usages_cat1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat1.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_cat1.c  -o ${OBJECTDIR}/can/can_dbc_usages_cat1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_cat1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_cat1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_cat2.o: can/can_dbc_usages_cat2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat2.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_cat2.c  -o ${OBJECTDIR}/can/can_dbc_usages_cat2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_cat2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_cat2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_mdat.o: can/can_dbc_usages_mdat.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_mdat.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_mdat.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_mdat.c  -o ${OBJECTDIR}/can/can_dbc_usages_mdat.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_mdat.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_mdat.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_sysreq.o: can/can_dbc_usages_sysreq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_sysreq.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_sysreq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_sysreq.c  -o ${OBJECTDIR}/can/can_dbc_usages_sysreq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_sysreq.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_sysreq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_vdat.o: can/can_dbc_usages_vdat.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_vdat.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_vdat.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_vdat.c  -o ${OBJECTDIR}/can/can_dbc_usages_vdat.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_vdat.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_vdat.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_ver.o: can/can_dbc_usages_ver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_ver.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_ver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_ver.c  -o ${OBJECTDIR}/can/can_dbc_usages_ver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_ver.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_ver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_utils.o: can/can_dbc_utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_utils.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_utils.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_utils.c  -o ${OBJECTDIR}/can/can_dbc_utils.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_utils.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_utils.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_cmd.o: can/can_dbc_cmd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_cmd.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_cmd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_cmd.c  -o ${OBJECTDIR}/can/can_dbc_cmd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_cmd.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_cmd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_auxreq.o: can/can_dbc_usages_auxreq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_auxreq.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_auxreq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_auxreq.c  -o ${OBJECTDIR}/can/can_dbc_usages_auxreq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_auxreq.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_auxreq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_auxresp.o: can/can_dbc_usages_auxresp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_auxresp.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_auxresp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_auxresp.c  -o ${OBJECTDIR}/can/can_dbc_usages_auxresp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_auxresp.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_auxresp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_bmftp.o: can/can_dbc_usages_bmftp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_bmftp.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_bmftp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_bmftp.c  -o ${OBJECTDIR}/can/can_dbc_usages_bmftp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_bmftp.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_bmftp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_ldat.o: can/can_dbc_usages_ldat.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_ldat.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_ldat.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_ldat.c  -o ${OBJECTDIR}/can/can_dbc_usages_ldat.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_ldat.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_ldat.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/daq/daq_adc.o: daq/daq_adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/daq" 
	@${RM} ${OBJECTDIR}/daq/daq_adc.o.d 
	@${RM} ${OBJECTDIR}/daq/daq_adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  daq/daq_adc.c  -o ${OBJECTDIR}/daq/daq_adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/daq/daq_adc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/daq/daq_adc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/EventLogData/EventLog.o: EventLogData/EventLog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/EventLogData" 
	@${RM} ${OBJECTDIR}/EventLogData/EventLog.o.d 
	@${RM} ${OBJECTDIR}/EventLogData/EventLog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  EventLogData/EventLog.c  -o ${OBJECTDIR}/EventLogData/EventLog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/EventLogData/EventLog.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/EventLogData/EventLog.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/flash/MX25L.o: flash/MX25L.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/flash" 
	@${RM} ${OBJECTDIR}/flash/MX25L.o.d 
	@${RM} ${OBJECTDIR}/flash/MX25L.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  flash/MX25L.c  -o ${OBJECTDIR}/flash/MX25L.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/flash/MX25L.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/flash/MX25L.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/i2c/i2cstate.o: i2c/i2cstate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/i2c" 
	@${RM} ${OBJECTDIR}/i2c/i2cstate.o.d 
	@${RM} ${OBJECTDIR}/i2c/i2cstate.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2c/i2cstate.c  -o ${OBJECTDIR}/i2c/i2cstate.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/i2c/i2cstate.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/i2c/i2cstate.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/i2c/i2c_a1006.o: i2c/i2c_a1006.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/i2c" 
	@${RM} ${OBJECTDIR}/i2c/i2c_a1006.o.d 
	@${RM} ${OBJECTDIR}/i2c/i2c_a1006.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2c/i2c_a1006.c  -o ${OBJECTDIR}/i2c/i2c_a1006.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/i2c/i2c_a1006.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/i2c/i2c_a1006.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main_app/mcu_init.o: main_app/mcu_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/main_app" 
	@${RM} ${OBJECTDIR}/main_app/mcu_init.o.d 
	@${RM} ${OBJECTDIR}/main_app/mcu_init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main_app/mcu_init.c  -o ${OBJECTDIR}/main_app/mcu_init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main_app/mcu_init.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main_app/mcu_init.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main_app/scheduler.o: main_app/scheduler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/main_app" 
	@${RM} ${OBJECTDIR}/main_app/scheduler.o.d 
	@${RM} ${OBJECTDIR}/main_app/scheduler.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main_app/scheduler.c  -o ${OBJECTDIR}/main_app/scheduler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main_app/scheduler.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main_app/scheduler.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main_app/int.o: main_app/int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/main_app" 
	@${RM} ${OBJECTDIR}/main_app/int.o.d 
	@${RM} ${OBJECTDIR}/main_app/int.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main_app/int.c  -o ${OBJECTDIR}/main_app/int.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main_app/int.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main_app/int.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main_app/version_config.o: main_app/version_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/main_app" 
	@${RM} ${OBJECTDIR}/main_app/version_config.o.d 
	@${RM} ${OBJECTDIR}/main_app/version_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main_app/version_config.c  -o ${OBJECTDIR}/main_app/version_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main_app/version_config.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main_app/version_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/reset.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/can1.o: mcc_generated_files/can1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/can1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/can1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/can1.c  -o ${OBJECTDIR}/mcc_generated_files/can1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/can1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/can1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/clock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/adc1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/mcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/dma.o: mcc_generated_files/dma.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/dma.c  -o ${OBJECTDIR}/mcc_generated_files/dma.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/dma.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/dma.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr3.o: mcc_generated_files/tmr3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr3.c  -o ${OBJECTDIR}/mcc_generated_files/tmr3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr3.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr3.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/spi2.o: mcc_generated_files/spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi2.c  -o ${OBJECTDIR}/mcc_generated_files/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/spi2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/uart1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/protection/checkSafety.o: protection/checkSafety.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/protection" 
	@${RM} ${OBJECTDIR}/protection/checkSafety.o.d 
	@${RM} ${OBJECTDIR}/protection/checkSafety.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  protection/checkSafety.c  -o ${OBJECTDIR}/protection/checkSafety.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/protection/checkSafety.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/protection/checkSafety.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/security/aes.o: security/aes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/security" 
	@${RM} ${OBJECTDIR}/security/aes.o.d 
	@${RM} ${OBJECTDIR}/security/aes.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  security/aes.c  -o ${OBJECTDIR}/security/aes.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/security/aes.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/security/aes.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/security/sha256.o: security/sha256.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/security" 
	@${RM} ${OBJECTDIR}/security/sha256.o.d 
	@${RM} ${OBJECTDIR}/security/sha256.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  security/sha256.c  -o ${OBJECTDIR}/security/sha256.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/security/sha256.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/security/sha256.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/smbus/i2c_crc_table.o: smbus/i2c_crc_table.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/smbus" 
	@${RM} ${OBJECTDIR}/smbus/i2c_crc_table.o.d 
	@${RM} ${OBJECTDIR}/smbus/i2c_crc_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  smbus/i2c_crc_table.c  -o ${OBJECTDIR}/smbus/i2c_crc_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/smbus/i2c_crc_table.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/smbus/i2c_crc_table.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/smbus/smbus.o: smbus/smbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/smbus" 
	@${RM} ${OBJECTDIR}/smbus/smbus.o.d 
	@${RM} ${OBJECTDIR}/smbus/smbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  smbus/smbus.c  -o ${OBJECTDIR}/smbus/smbus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/smbus/smbus.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/smbus/smbus.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/smbus/smbus_cmd.o: smbus/smbus_cmd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/smbus" 
	@${RM} ${OBJECTDIR}/smbus/smbus_cmd.o.d 
	@${RM} ${OBJECTDIR}/smbus/smbus_cmd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  smbus/smbus_cmd.c  -o ${OBJECTDIR}/smbus/smbus_cmd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/smbus/smbus_cmd.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/smbus/smbus_cmd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/sys_time/sys_time.o: sys_time/sys_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sys_time" 
	@${RM} ${OBJECTDIR}/sys_time/sys_time.o.d 
	@${RM} ${OBJECTDIR}/sys_time/sys_time.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sys_time/sys_time.c  -o ${OBJECTDIR}/sys_time/sys_time.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sys_time/sys_time.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/sys_time/sys_time.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/battery_manage_system/battery_manage_system.o: battery_manage_system/battery_manage_system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/battery_manage_system" 
	@${RM} ${OBJECTDIR}/battery_manage_system/battery_manage_system.o.d 
	@${RM} ${OBJECTDIR}/battery_manage_system/battery_manage_system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  battery_manage_system/battery_manage_system.c  -o ${OBJECTDIR}/battery_manage_system/battery_manage_system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/battery_manage_system/battery_manage_system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/battery_manage_system/battery_manage_system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/battery_manage_system/SOC.o: battery_manage_system/SOC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/battery_manage_system" 
	@${RM} ${OBJECTDIR}/battery_manage_system/SOC.o.d 
	@${RM} ${OBJECTDIR}/battery_manage_system/SOC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  battery_manage_system/SOC.c  -o ${OBJECTDIR}/battery_manage_system/SOC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/battery_manage_system/SOC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/battery_manage_system/SOC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/battery_manage_system/SOH.o: battery_manage_system/SOH.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/battery_manage_system" 
	@${RM} ${OBJECTDIR}/battery_manage_system/SOH.o.d 
	@${RM} ${OBJECTDIR}/battery_manage_system/SOH.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  battery_manage_system/SOH.c  -o ${OBJECTDIR}/battery_manage_system/SOH.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/battery_manage_system/SOH.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/battery_manage_system/SOH.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can.o: can/can.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can.o.d 
	@${RM} ${OBJECTDIR}/can/can.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can.c  -o ${OBJECTDIR}/can/can.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc.o: can/can_dbc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc.c  -o ${OBJECTDIR}/can/can_dbc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_mem_map_uit.o: can/can_dbc_mem_map_uit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_mem_map_uit.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_mem_map_uit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_mem_map_uit.c  -o ${OBJECTDIR}/can/can_dbc_mem_map_uit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_mem_map_uit.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_mem_map_uit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_cat0.o: can/can_dbc_usages_cat0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat0.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_cat0.c  -o ${OBJECTDIR}/can/can_dbc_usages_cat0.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_cat0.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_cat0.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_cat1.o: can/can_dbc_usages_cat1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat1.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_cat1.c  -o ${OBJECTDIR}/can/can_dbc_usages_cat1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_cat1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_cat1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_cat2.o: can/can_dbc_usages_cat2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat2.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_cat2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_cat2.c  -o ${OBJECTDIR}/can/can_dbc_usages_cat2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_cat2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_cat2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_mdat.o: can/can_dbc_usages_mdat.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_mdat.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_mdat.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_mdat.c  -o ${OBJECTDIR}/can/can_dbc_usages_mdat.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_mdat.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_mdat.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_sysreq.o: can/can_dbc_usages_sysreq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_sysreq.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_sysreq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_sysreq.c  -o ${OBJECTDIR}/can/can_dbc_usages_sysreq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_sysreq.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_sysreq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_vdat.o: can/can_dbc_usages_vdat.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_vdat.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_vdat.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_vdat.c  -o ${OBJECTDIR}/can/can_dbc_usages_vdat.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_vdat.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_vdat.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_ver.o: can/can_dbc_usages_ver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_ver.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_ver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_ver.c  -o ${OBJECTDIR}/can/can_dbc_usages_ver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_ver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_ver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_utils.o: can/can_dbc_utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_utils.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_utils.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_utils.c  -o ${OBJECTDIR}/can/can_dbc_utils.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_utils.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_utils.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_cmd.o: can/can_dbc_cmd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_cmd.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_cmd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_cmd.c  -o ${OBJECTDIR}/can/can_dbc_cmd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_cmd.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_cmd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_auxreq.o: can/can_dbc_usages_auxreq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_auxreq.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_auxreq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_auxreq.c  -o ${OBJECTDIR}/can/can_dbc_usages_auxreq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_auxreq.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_auxreq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_auxresp.o: can/can_dbc_usages_auxresp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_auxresp.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_auxresp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_auxresp.c  -o ${OBJECTDIR}/can/can_dbc_usages_auxresp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_auxresp.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_auxresp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_bmftp.o: can/can_dbc_usages_bmftp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_bmftp.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_bmftp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_bmftp.c  -o ${OBJECTDIR}/can/can_dbc_usages_bmftp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_bmftp.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_bmftp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/can/can_dbc_usages_ldat.o: can/can_dbc_usages_ldat.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/can" 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_ldat.o.d 
	@${RM} ${OBJECTDIR}/can/can_dbc_usages_ldat.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  can/can_dbc_usages_ldat.c  -o ${OBJECTDIR}/can/can_dbc_usages_ldat.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/can/can_dbc_usages_ldat.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/can/can_dbc_usages_ldat.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/daq/daq_adc.o: daq/daq_adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/daq" 
	@${RM} ${OBJECTDIR}/daq/daq_adc.o.d 
	@${RM} ${OBJECTDIR}/daq/daq_adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  daq/daq_adc.c  -o ${OBJECTDIR}/daq/daq_adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/daq/daq_adc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/daq/daq_adc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/EventLogData/EventLog.o: EventLogData/EventLog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/EventLogData" 
	@${RM} ${OBJECTDIR}/EventLogData/EventLog.o.d 
	@${RM} ${OBJECTDIR}/EventLogData/EventLog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  EventLogData/EventLog.c  -o ${OBJECTDIR}/EventLogData/EventLog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/EventLogData/EventLog.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/EventLogData/EventLog.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/flash/MX25L.o: flash/MX25L.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/flash" 
	@${RM} ${OBJECTDIR}/flash/MX25L.o.d 
	@${RM} ${OBJECTDIR}/flash/MX25L.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  flash/MX25L.c  -o ${OBJECTDIR}/flash/MX25L.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/flash/MX25L.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/flash/MX25L.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/i2c/i2cstate.o: i2c/i2cstate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/i2c" 
	@${RM} ${OBJECTDIR}/i2c/i2cstate.o.d 
	@${RM} ${OBJECTDIR}/i2c/i2cstate.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2c/i2cstate.c  -o ${OBJECTDIR}/i2c/i2cstate.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/i2c/i2cstate.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/i2c/i2cstate.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/i2c/i2c_a1006.o: i2c/i2c_a1006.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/i2c" 
	@${RM} ${OBJECTDIR}/i2c/i2c_a1006.o.d 
	@${RM} ${OBJECTDIR}/i2c/i2c_a1006.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2c/i2c_a1006.c  -o ${OBJECTDIR}/i2c/i2c_a1006.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/i2c/i2c_a1006.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/i2c/i2c_a1006.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main_app/mcu_init.o: main_app/mcu_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/main_app" 
	@${RM} ${OBJECTDIR}/main_app/mcu_init.o.d 
	@${RM} ${OBJECTDIR}/main_app/mcu_init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main_app/mcu_init.c  -o ${OBJECTDIR}/main_app/mcu_init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main_app/mcu_init.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main_app/mcu_init.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main_app/scheduler.o: main_app/scheduler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/main_app" 
	@${RM} ${OBJECTDIR}/main_app/scheduler.o.d 
	@${RM} ${OBJECTDIR}/main_app/scheduler.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main_app/scheduler.c  -o ${OBJECTDIR}/main_app/scheduler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main_app/scheduler.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main_app/scheduler.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main_app/int.o: main_app/int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/main_app" 
	@${RM} ${OBJECTDIR}/main_app/int.o.d 
	@${RM} ${OBJECTDIR}/main_app/int.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main_app/int.c  -o ${OBJECTDIR}/main_app/int.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main_app/int.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main_app/int.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main_app/version_config.o: main_app/version_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/main_app" 
	@${RM} ${OBJECTDIR}/main_app/version_config.o.d 
	@${RM} ${OBJECTDIR}/main_app/version_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main_app/version_config.c  -o ${OBJECTDIR}/main_app/version_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main_app/version_config.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main_app/version_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/reset.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/can1.o: mcc_generated_files/can1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/can1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/can1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/can1.c  -o ${OBJECTDIR}/mcc_generated_files/can1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/can1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/can1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/clock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/adc1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/mcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/dma.o: mcc_generated_files/dma.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/dma.c  -o ${OBJECTDIR}/mcc_generated_files/dma.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/dma.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/dma.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr3.o: mcc_generated_files/tmr3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr3.c  -o ${OBJECTDIR}/mcc_generated_files/tmr3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr3.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/spi2.o: mcc_generated_files/spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi2.c  -o ${OBJECTDIR}/mcc_generated_files/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/spi2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/uart1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/protection/checkSafety.o: protection/checkSafety.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/protection" 
	@${RM} ${OBJECTDIR}/protection/checkSafety.o.d 
	@${RM} ${OBJECTDIR}/protection/checkSafety.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  protection/checkSafety.c  -o ${OBJECTDIR}/protection/checkSafety.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/protection/checkSafety.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/protection/checkSafety.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/security/aes.o: security/aes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/security" 
	@${RM} ${OBJECTDIR}/security/aes.o.d 
	@${RM} ${OBJECTDIR}/security/aes.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  security/aes.c  -o ${OBJECTDIR}/security/aes.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/security/aes.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/security/aes.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/security/sha256.o: security/sha256.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/security" 
	@${RM} ${OBJECTDIR}/security/sha256.o.d 
	@${RM} ${OBJECTDIR}/security/sha256.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  security/sha256.c  -o ${OBJECTDIR}/security/sha256.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/security/sha256.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/security/sha256.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/smbus/i2c_crc_table.o: smbus/i2c_crc_table.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/smbus" 
	@${RM} ${OBJECTDIR}/smbus/i2c_crc_table.o.d 
	@${RM} ${OBJECTDIR}/smbus/i2c_crc_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  smbus/i2c_crc_table.c  -o ${OBJECTDIR}/smbus/i2c_crc_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/smbus/i2c_crc_table.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/smbus/i2c_crc_table.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/smbus/smbus.o: smbus/smbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/smbus" 
	@${RM} ${OBJECTDIR}/smbus/smbus.o.d 
	@${RM} ${OBJECTDIR}/smbus/smbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  smbus/smbus.c  -o ${OBJECTDIR}/smbus/smbus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/smbus/smbus.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/smbus/smbus.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/smbus/smbus_cmd.o: smbus/smbus_cmd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/smbus" 
	@${RM} ${OBJECTDIR}/smbus/smbus_cmd.o.d 
	@${RM} ${OBJECTDIR}/smbus/smbus_cmd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  smbus/smbus_cmd.c  -o ${OBJECTDIR}/smbus/smbus_cmd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/smbus/smbus_cmd.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/smbus/smbus_cmd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/sys_time/sys_time.o: sys_time/sys_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sys_time" 
	@${RM} ${OBJECTDIR}/sys_time/sys_time.o.d 
	@${RM} ${OBJECTDIR}/sys_time/sys_time.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sys_time/sys_time.c  -o ${OBJECTDIR}/sys_time/sys_time.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sys_time/sys_time.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/sys_time/sys_time.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -mlarge-code -O0 -DEVENT_LOG -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/AU1803_05.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/AU1803_05.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time"  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp=${DFP_DIR}/xc16 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/AU1803_05.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/AU1803_05.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"main_app" -I"sys_time" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp=${DFP_DIR}/xc16 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/AU1803_05.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp=${DFP_DIR}/xc16 
	
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
