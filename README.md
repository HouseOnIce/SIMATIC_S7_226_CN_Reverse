# SIMATIC S7-226 CN firmware reconstruction

This directory contains a working reconstruction derived from [SIMATIC-S7-226-RE](https://github.com/AL-255/SIMATIC-S7-226-RE) and aligned to the Geehy APM32E10x SDK in `APM32E10x_SDK`.

The recovered application and protocol modules retain their original Ghidra-derived C. The master device header now includes the SDK CMSIS device header (`apm32e10x.h`) so peripheral and core definitions come from the target SDK. Register-level accesses are retained where they preserve observed firmware behavior.

## Layout

- `src/`: reconstructed firmware modules (main loop, VM, protocol stacks, peripherals and interrupts)
- `include/`: PLC memory map and protocol definitions
- `platform/`: SDK device startup and system initialization source
- `linker/`: 512 KiB Flash / 64 KiB SRAM linker script
- `APM32E10x_SDK/Libraries`: CMSIS, device headers and standard peripheral driver sources

## Build

Requires `arm-none-eabi-gcc`, `arm-none-eabi-objcopy`, and GNU Make. Run `make` here. SDK headers and drivers are referenced from the sibling SDK directory; the SDK itself is not copied or modified.

This is a source-level reconstruction, not a bit-identical rebuild of the vendor image. Review `docs/reconstruction-notes.md` for integration limits.

---

# SIMATIC S7-226 CN 固件重构

本目录包含一个可用的重构版本，源自[SIMATIC-S7-226-RE](https://github.com/AL-255/SIMATIC-S7-226-RE)，并与 `APM32E10x_SDK` 中的极海 APM32E10x SDK 保持一致。

已恢复的应用和协议模块保留了原始的 Ghidra 反编译 C 代码。主设备头文件现已包含 SDK 的 CMSIS 设备头文件（`apm32e10x.h`），因此外设和内核定义均来自目标 SDK。在保留观察到的固件行为所必需的地方，仍保留寄存器级访问。

## 目录结构

- `src/`：重构的固件模块（主循环、VM、协议栈、外设和中断）
- `include/`：PLC 内存映射和协议定义
- `platform/`：SDK 设备启动和系统初始化源代码
- `linker/`：512 KiB Flash / 64 KiB SRAM 链接脚本
- `APM32E10x_SDK/Libraries`：CMSIS、设备头文件和标准外设驱动源代码

## 构建

需要 `arm-none-eabi-gcc`、`arm-none-eabi-objcopy` 和 GNU Make。在此目录运行 `make`。SDK 头文件和驱动从同级 SDK 目录引用；SDK 本身不会被复制或修改。

这是源代码级别的重构，并非厂商固件镜像的逐位一致重建。集成限制请参阅 `docs/reconstruction-notes.md`。
