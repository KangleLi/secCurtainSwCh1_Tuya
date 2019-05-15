################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include zll-on-off-server/subdir.mk
-include zll-level-control-server/subdir.mk
-include vancount/subdir.mk
-include update-tc-link-key/subdir.mk
-include simple-main/subdir.mk
-include sim-eeprom2/EFR32/subdir.mk
-include serial/EFR32/subdir.mk
-include serial/subdir.mk
-include scenes/subdir.mk
-include scan-dispatch/subdir.mk
-include reporting/subdir.mk
-include ota-storage-simple-eeprom/subdir.mk
-include ota-storage-simple/subdir.mk
-include ota-storage-common/subdir.mk
-include ota-common/subdir.mk
-include ota-client-policy/subdir.mk
-include ota-client/subdir.mk
-include ota-bootload/subdir.mk
-include on-off/subdir.mk
-include network-steering/subdir.mk
-include level-control/subdir.mk
-include led-blink/EFR32/subdir.mk
-include interpan/subdir.mk
-include identify/subdir.mk
-include hal-library/EFR32/subdir.mk
-include groups-server/subdir.mk
-include green-power-common/subdir.mk
-include green-power-client/subdir.mk
-include form-and-join/subdir.mk
-include find-and-bind-target/subdir.mk
-include external-generated-files/subdir.mk
-include emlib/subdir.mk
-include emdrv/subdir.mk
-include ember-minimal-printf/subdir.mk
-include efr32/subdir.mk
-include eeprom/subdir.mk
-include debug-jtag/EFR32/subdir.mk
-include debug-basic-library/EFR32/subdir.mk
-include counters/subdir.mk
-include coexistence/EFR32/subdir.mk
-include button-joining/subdir.mk
-include basic/subdir.mk
-include antenna-stub/subdir.mk
-include aes-cmac/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pre-build main-build

# Main-build Target
main-build: ShadeDeviceForOne506.axf

# Tool invocations
ShadeDeviceForOne506.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//platform/base/hal/micro/cortexm3/efm32/gcc-cfg.ld" -L"D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//platform/base/hal/micro/cortexm3/" -Xlinker --defsym=FLASH_SIZE=524288 -Xlinker --defsym=RAM_SIZE=65536 -Xlinker --defsym=SIMEEPROM_SIZE=36864 -Xlinker --defsym=APP_GECKO_INFO_PAGE_BTL=1 -Xlinker --defsym=EMBER_MALLOC_HEAP_SIZE=0 -Xlinker --defsym=INTERNAL_STORAGE_SIZE=241664 -Xlinker --gc-sections -Xlinker -Map="ShadeDeviceForOne506.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o ShadeDeviceForOne506.axf -Wl,--start-group "./aes-cmac/aes-cmac.o" "./af-event.o" "./af-main-common.o" "./af-main-soc.o" "./af-node.o" "./af-security-common.o" "./af-trust-center.o" "./afv2-bookkeeping.o" "./attribute-size.o" "./attribute-storage.o" "./attribute-table.o" "./call-command-handler.o" "./callback-stub.o" "./cli.o" "./client-api.o" "./command-interpreter2.o" "./core-cli.o" "./crc.o" "./crypto-state.o" "./ember-base-configuration.o" "./ember-configuration.o" "./endian.o" "./library.o" "./mem-util.o" "./message.o" "./multi-network.o" "./network-cli.o" "./option-cli.o" "./plugin-cli.o" "./print-formatter.o" "./print.o" "./process-cluster-message.o" "./process-global-message.o" "./random.o" "./security-address-cache.o" "./security-cli.o" "./service-discovery-common.o" "./service-discovery-soc.o" "./stack-handler-stub.o" "./time-util.o" "./token-def.o" "./util.o" "./zcl-cli.o" "./zdo-cli.o" "./zigbee-device-common.o" "./zigbee-device-library.o" "./antenna-stub/antenna-stub.o" "./basic/basic.o" "./button-joining/button-joining-cli.o" "./button-joining/button-joining.o" "./coexistence/EFR32/coexistence-802154.o" "./coexistence/EFR32/coexistence-hal.o" "./coexistence/EFR32/coexistence.o" "./counters/counters-cli.o" "./counters/counters-ota.o" "./counters/counters-soc.o" "./debug-basic-library/EFR32/SEGGER_RTT.o" "./debug-jtag/EFR32/debug-jtag-efr32.o" "./eeprom/eeprom-afv2.o" "./eeprom/eeprom-cli.o" "./eeprom/eeprom.o" "./efr32/assert-crash-handlers.o" "./efr32/button.o" "./efr32/buzzer-efr32.o" "./efr32/cstartup-common.o" "./efr32/diagnostic.o" "./efr32/ext-device.o" "./efr32/faults-v7m.o" "./efr32/isr-stubs.o" "./efr32/led.o" "./efr32/mfg-token.o" "./efr32/micro-common.o" "./efr32/micro.o" "./efr32/mpu.o" "./efr32/sleep-efm32.o" "./efr32/token.o" "./ember-minimal-printf/ember-printf.o" "./emdrv/dmadrv.o" "./emdrv/gpiointerrupt.o" "./emdrv/rtcdriver.o" "./emdrv/sleep.o" "./emdrv/tempdrv.o" "./emdrv/uartdrv.o" "./emdrv/ustimer.o" "./emlib/em_adc.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_cryotimer.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_ldma.o" "./emlib/em_leuart.o" "./emlib/em_mpu.o" "./emlib/em_msc.o" "./emlib/em_prs.o" "./emlib/em_rmu.o" "./emlib/em_rtcc.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./emlib/em_wdog.o" "./emlib/system_efr32mg13p.o" "./external-generated-files/bootloader-interface-app.o" "./external-generated-files/bootloader-interface.o" "./external-generated-files/btl_interface.o" "./external-generated-files/btl_interface_storage.o" "./find-and-bind-target/find-and-bind-target-cli.o" "./find-and-bind-target/find-and-bind-target.o" "./form-and-join/form-and-join-afv2.o" "./form-and-join/form-and-join-node-adapter.o" "./form-and-join/form-and-join.o" "./green-power-client/green-power-client-cli.o" "./green-power-client/green-power-client.o" "./green-power-common/green-power-common.o" "./groups-server/groups-server-cli.o" "./groups-server/groups-server.o" "./hal-library/EFR32/hal-config-gpio.o" "./hal-library/EFR32/hal-config.o" "./identify/identify-cli.o" "./identify/identify.o" "./interpan/interpan-cli.o" "./interpan/interpan-soc.o" "./interpan/interpan.o" "./led-blink/EFR32/led-blink.o" "./level-control/level-control.o" "./network-steering/network-steering-cli.o" "./network-steering/network-steering-soc.o" "./network-steering/network-steering.o" "./on-off/on-off.o" "./ota-bootload/ota-bootload-cli.o" "./ota-bootload/ota-bootload-soc.o" "./ota-client/ota-client-cli.o" "./ota-client/ota-client-page-request.o" "./ota-client/ota-client-signature-verify.o" "./ota-client/ota-client.o" "./ota-client-policy/ota-client-policy.o" "./ota-common/ota-common.o" "./ota-storage-common/ota-storage-common-cli.o" "./ota-storage-common/ota-storage-common.o" "./ota-storage-simple/ota-storage-simple.o" "./ota-storage-simple-eeprom/ota-storage-eeprom-page-erase.o" "./ota-storage-simple-eeprom/ota-storage-eeprom-read-modify-write.o" "./ota-storage-simple-eeprom/ota-storage-eeprom.o" "./reporting/reporting-cli.o" "./reporting/reporting-default-configuration.o" "./reporting/reporting.o" "./scan-dispatch/scan-dispatch.o" "./scenes/scenes-cli.o" "./scenes/scenes.o" "./serial/EFR32/serial.o" "./serial/com.o" "./sim-eeprom2/EFR32/sim-eeprom2.o" "./simple-main/simple-main.o" "./update-tc-link-key/update-tc-link-key-cli.o" "./update-tc-link-key/update-tc-link-key.o" "./vancount/_carel_ext.o" "./vancount/list.o" "./vancount/vancount-customer-extend.o" "./vancount/vancount-hal-serial.o" "./vancount/vancount-mf-test.o" "./vancount/vancount-tokens-cli.o" "./vancount/vancount-z3-net.o" "./vancount/vancount-z3.o" "./vancount/vancount-zigbee-extend.o" "./vancount/vc-device-shade.o" "./vancount/vc-onoff.o" "./vancount/vc-reporting.o" "./vancount/vc-scenes.o" "./vancount/vcHalLight.o" "./zll-level-control-server/zll-level-control-server.o" "./zll-on-off-server/zll-on-off-server.o" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//platform/radio/rail_lib/autogen/librail_release/librail_efr32xg13_gcc_release.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/sim-eeprom2-library-cortexm3-gcc-efr32mg13p-rail/sim-eeprom2-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/binding-table-library-cortexm3-gcc-efr32mg13p-rail/binding-table-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/cbke-163k1-stub-library-cortexm3-gcc-efr32mg13p-rail/cbke-163k1-stub-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/multi-network-stub-library-cortexm3-gcc-efr32mg13p-rail/multi-network-stub-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/cbke-library-dsa-sign-stub-cortexm3-gcc-efr32mg13p-rail/cbke-library-dsa-sign-stub.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/cbke-library-dsa-verify-stub-cortexm3-gcc-efr32mg13p-rail/cbke-library-dsa-verify-stub.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/end-device-bind-stub-library-cortexm3-gcc-efr32mg13p-rail/end-device-bind-stub-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/cbke-library-dsa-verify-283k1-stub-cortexm3-gcc-efr32mg13p-rail/cbke-library-dsa-verify-283k1-stub.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/zll-library-cortexm3-gcc-efr32mg13p-rail/zll-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/security-library-link-keys-stub-cortexm3-gcc-efr32mg13p-rail/security-library-link-keys-stub.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/packet-validate-stub-library-cortexm3-gcc-efr32mg13p-rail/packet-validate-stub-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/debug-extended-stub-library-cortexm3-gcc-efr32mg13p-rail/debug-extended-stub-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/zigbee-pro-stack-cortexm3-gcc-efr32mg13p-rail/zigbee-pro-stack.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/cbke-283k1-stub-library-cortexm3-gcc-efr32mg13p-rail/cbke-283k1-stub-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/hal-library-cortexm3-gcc-efr32mg13p-rail/hal-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/install-code-library-cortexm3-gcc-efr32mg13p-rail/install-code-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/zigbee-r22-support-stub-library-cortexm3-gcc-efr32mg13p-rail/zigbee-r22-support-stub-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/cbke-stub-library-cortexm3-gcc-efr32mg13p-rail/cbke-stub-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/gp-library-cortexm3-gcc-efr32mg13p-rail/gp-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/security-library-core-cortexm3-gcc-efr32mg13p-rail/security-library-core.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/debug-basic-library-cortexm3-gcc-efr32mg13p-rail/debug-basic-library.a" "D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4//protocol/zigbee/build/sim-eeprom2-1to2-upgrade-stub-library-cortexm3-gcc-efr32mg13p-rail/sim-eeprom2-1to2-upgrade-stub-library.a" -lm -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: ShadeDeviceForOne506.hex'
	arm-none-eabi-objcopy -O ihex "ShadeDeviceForOne506.axf" "ShadeDeviceForOne506.hex"
	@echo ' '

	@echo 'Building bin file: ShadeDeviceForOne506.bin'
	arm-none-eabi-objcopy -O binary "ShadeDeviceForOne506.axf" "ShadeDeviceForOne506.bin"
	@echo ' '

	@echo 'Building s37 file: ShadeDeviceForOne506.s37'
	arm-none-eabi-objcopy -O srec "ShadeDeviceForOne506.axf" "ShadeDeviceForOne506.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "ShadeDeviceForOne506.axf"
	@echo ' '

	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) ShadeDeviceForOne506.axf
	-@echo ' '

pre-build:
	-cmd //c 'D:\soft\SiliconLabs\SimplicityStudio\v4\developer\sdks\gecko_sdk_suite\v2.4/app/builder/ShadeDeviceForOne506/ShadeDeviceForOne506-prebuild.bat' 'ShadeDeviceForOne506' 'D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4/app/builder/ShadeDeviceForOne506' '' 'D:\soft\SiliconLabs\SimplicityStudio\v4'
	-@echo ' '

post-build:
	-cmd //c 'D:\soft\SiliconLabs\SimplicityStudio\v4\developer\sdks\gecko_sdk_suite\v2.4/app/builder/ShadeDeviceForOne506/ShadeDeviceForOne506-postbuild.bat' 'ShadeDeviceForOne506' 'D:/soft/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.4/app/builder/ShadeDeviceForOne506' '' 'D:\soft\SiliconLabs\SimplicityStudio\v4'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
