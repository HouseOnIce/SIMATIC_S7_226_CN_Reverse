cmake_minimum_required(VERSION 3.20)
project(s7_226_reconstructed C ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m3)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(SDK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../APM32E10x_SDK/Libraries" CACHE PATH "Geehy APM32 SDK Libraries")
set(DEVICE "${SDK_ROOT}/Device/Geehy/APM32E10x")
set(DRIVER "${SDK_ROOT}/APM32E10x_StdPeriphDriver")
set(APP_SOURCES
    src/main.c src/plc_prog_parser.c src/plc_vm_interpreter.c
    src/s7_comm_eth_port.c src/s7_ppi_serial.c src/plc_hsc_exti.c
    src/plc_int_dispatch.c src/plc_sdb_expansion.c src/bsp_peripherals.c
    src/plc_timers_pto.c src/plc_usart_rs485.c
    platform/startup_apm32e10x_hd.S)
add_executable(s7_226_reconstructed.elf ${APP_SOURCES})
target_link_libraries(s7_226_reconstructed.elf PRIVATE m)
target_include_directories(s7_226_reconstructed.elf PRIVATE
    include "${DEVICE}/Include" "${SDK_ROOT}/CMSIS/Include" "${DRIVER}/inc")
target_compile_definitions(s7_226_reconstructed.elf PRIVATE APM32E10x_HD USE_STDPERIPH_DRIVER)
target_compile_options(s7_226_reconstructed.elf PRIVATE
    -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -ffunction-sections -fdata-sections
    -fno-strict-aliasing -fsigned-char -Wall -Wextra)
target_link_options(s7_226_reconstructed.elf PRIVATE
    -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -nostartfiles
    "-T${CMAKE_CURRENT_LIST_DIR}/linker/apm32e103vet6.ld" -Wl,--gc-sections -Wl,-Map=s7_226_reconstructed.map)
set_target_properties(s7_226_reconstructed.elf PROPERTIES SUFFIX "")

find_program(ARM_OBJCOPY arm-none-eabi-objcopy REQUIRED)
add_custom_command(TARGET s7_226_reconstructed.elf POST_BUILD
    COMMAND "${ARM_OBJCOPY}" -O binary $<TARGET_FILE:s7_226_reconstructed.elf> "${CMAKE_CURRENT_BINARY_DIR}/s7_226_reconstructed.bin"
    BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/s7_226_reconstructed.bin")
