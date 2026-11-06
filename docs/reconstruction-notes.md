# Reconstruction notes

The baseline is the initial C decompilation in `SIMATIC-S7-226-RE`. The new project is isolated in this directory and uses the workspace Geehy APM32E10x SDK's Cortex-M3 CMSIS definitions and GNU startup/system files.

The recovered PLC logic directly accesses memory-mapped registers and has many fixed SRAM addresses, reconstructed Ghidra types, and absolute firmware-layout assumptions. Those accesses are intentionally preserved pending function-by-function validation against the binary. SDK headers provide device definitions; they do not make the decompilation equivalent to original vendor source.

The reconstructed code's original README reports a syntax check of the source modules. A complete target link requires resolving reconstructed fixed-address symbols and reviewing startup/vector ownership and linker placement. This project supplies a conventional SDK-based target build as an integration scaffold; successful compilation does not prove behavioral equivalence or safe operation on hardware.

---

# 重构说明

基线为 `SIMATIC-S7-226-RE` 中的初始 C 反编译代码。新项目独立于本目录，使用工作区中极海 APM32E10x SDK 的 Cortex-M3 CMSIS 定义以及 GNU 启动/系统文件。

已恢复的 PLC 逻辑直接访问内存映射寄存器，并包含大量固定 SRAM 地址、Ghidra 重建的类型以及绝对的固件布局假设。这些访问被有意保留，待与二进制文件逐函数验证。SDK 头文件仅提供器件定义，并不意味着该反编译代码与厂商原始源代码等价。

重构代码的原始 README 报告了源模块的语法检查结果。要完成完整的目标链接，需要解决重构中的固定地址符号问题，并检查启动/中断向量归属及链接器段放置。本项目提供了一个常规的基于 SDK 的目标构建，作为集成框架；编译成功并不能证明行为等价，也不能证明在硬件上运行安全。
