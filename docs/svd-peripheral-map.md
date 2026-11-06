# APM32E103 SVD cross-check

Source: `../APM32E10x_SDK/Utilities/SVD/APM32E103xx.svd` (APM32E103xx, Cortex-M3).

The SVD confirms the target addresses used by the reconstructed firmware. The names below are the SVD spelling; firmware code may use STM32-compatible aliases.

| SVD peripheral | Base address | SVD register examples (offset) | Reconstructed code use |
|---|---:|---|---|
| GPIOA | `0x40010800` | CFGLOW `0x00`, CFGHIG `0x04`, IDATA `0x08`, ODATA `0x0C`, BSC `0x10` | Digital I/O |
| AFIO | `0x40010000` | EVCTRL `0x00`, REMAP1 `0x04`, EINTSEL1 `0x08` | Pin remapping and EXTI routing |
| USART1 | `0x40013800` | STS `0x00`, DATA `0x04`, BR `0x08`, CTRL1 `0x0C` | RS-485 port 0 |
| USART2 | `0x40004400` | SVD peripheral block present | RS-485 port 1 |
| USART3 | `0x40004800` | SVD peripheral block present | Expansion / serial port |
| ADC1 | `0x40012400` | STS `0x00`, CTRL1 `0x04`, CTRL2 `0x08` | Analog input sampling |
| DMA1 | `0x40020000` | INTSTS `0x00`, INTFCLR `0x04`, CHCFG1 `0x08` | Peripheral data movement |
| TMR1 | `0x40012C00` | CTRL1 `0x00`, DIEN `0x0C`, STS `0x10`, CEG `0x14` | Timer/PTO services |
| TMR2 | `0x40000000` | CTRL1 `0x00`, DIEN `0x0C`, STS `0x10` | Timer services |
| TMR3 | `0x40000400` | CTRL1 `0x00`, DIEN `0x0C`, STS `0x10` | Timer services |
| TMR4 | `0x40000800` | SVD peripheral block present | Timer services |
| RTC | `0x40002800` | CTRL `0x00`, CSTS `0x04`, PSCRLDH `0x08`, PSCRLDL `0x0C` | Calendar / time base |
| IWDT | `0x40003000` | KEY `0x00`, PSC `0x04`, CNTRLD `0x08`, STS `0x0C` | Independent watchdog |
| WWDT | `0x40002C00` | CTRL `0x00`, CFG `0x04`, STS `0x08` | Window watchdog |

The checked offsets align with the corresponding STM32-compatible layouts used by the firmware reconstruction. The SVD is now referenced by this project for address review; this does not establish that a peripheral's application semantics or all bit-field interpretations are correct. Firmware code still has a legacy register-structure layer, so SDK CMSIS names and that layer must not be mixed in the same expression.

---

# APM32E103 SVD 交叉核对

来源：`../APM32E10x_SDK/Utilities/SVD/APM32E103xx.svd`（APM32E103xx，Cortex-M3）。

SVD 确认了重构固件所使用的目标地址。下表中的名称为 SVD 拼写；固件代码中可能使用与 STM32 兼容的别名。

| SVD 外设 | 基地址 | SVD 寄存器示例（偏移量） | 重构代码用途 |
|---|---:|---|---|
| GPIOA | `0x40010800` | CFGLOW `0x00`, CFGHIG `0x04`, IDATA `0x08`, ODATA `0x0C`, BSC `0x10` | 数字 I/O |
| AFIO | `0x40010000` | EVCTRL `0x00`, REMAP1 `0x04`, EINTSEL1 `0x08` | 引脚重映射及 EXTI 路由 |
| USART1 | `0x40013800` | STS `0x00`, DATA `0x04`, BR `0x08`, CTRL1 `0x0C` | RS-485 端口 0 |
| USART2 | `0x40004400` | 存在 SVD 外设块 | RS-485 端口 1 |
| USART3 | `0x40004800` | 存在 SVD 外设块 | 扩展 / 串口 |
| ADC1 | `0x40012400` | STS `0x00`, CTRL1 `0x04`, CTRL2 `0x08` | 模拟输入采样 |
| DMA1 | `0x40020000` | INTSTS `0x00`, INTFCLR `0x04`, CHCFG1 `0x08` | 外设数据搬运 |
| TMR1 | `0x40012C00` | CTRL1 `0x00`, DIEN `0x0C`, STS `0x10`, CEG `0x14` | 定时器 / PTO 服务 |
| TMR2 | `0x40000000` | CTRL1 `0x00`, DIEN `0x0C`, STS `0x10` | 定时器服务 |
| TMR3 | `0x40000400` | CTRL1 `0x00`, DIEN `0x0C`, STS `0x10` | 定时器服务 |
| TMR4 | `0x40000800` | 存在 SVD 外设块 | 定时器服务 |
| RTC | `0x40002800` | CTRL `0x00`, CSTS `0x04`, PSCRLDH `0x08`, PSCRLDL `0x0C` | 日历 / 时基 |
| IWDT | `0x40003000` | KEY `0x00`, PSC `0x04`, CNTRLD `0x08`, STS `0x0C` | 独立看门狗 |
| WWDT | `0x40002C00` | CTRL `0x00`, CFG `0x04`, STS `0x08` | 窗口看门狗 |

已核对的偏移量与固件重构所用的 STM32 兼容寄存器布局一致。本项目现以 SVD 作为地址审查依据；这并不能证明外设的应用语义或所有位域解释均正确。固件代码仍保留一层旧式寄存器结构，因此不得在同一表达式中混用 SDK CMSIS 名称与该结构层。
