/**The stb head file configuration item
 * #define STM32_F1xx_GPIO
 * #define STM32_F4xx_GPIO
 * */

#ifndef __BIT_BAND_GPIO_H
#define __BIT_BAND_GPIO_H


#define BITBAND(addr, bitnum) ((0x42000000UL + (((uint32_t)(addr) - 0x40000000UL) * 32UL) + ((bitnum) * 4UL)))

#ifdef STM32_F1xx_GPIO

#include "stm32f1xx.h"  // 根据实际使用的STM32系列包含对应的头文件
/* 位段操作宏定义 */

/* GPIO端口基地址定义 (以STM32F1为例) */
#define GPIOA_BASE    0x40010800UL
#define GPIOB_BASE    0x40010C00UL
#define GPIOC_BASE    0x40011000UL
#define GPIOD_BASE    0x40011400UL
#define GPIOE_BASE    0x40011800UL

/* GPIO寄存器偏移地址 */
#define GPIO_ODR_OFFSET  0x0C  // 输出数据寄存器偏移
#define GPIO_IDR_OFFSET  0x08  // 输入数据寄存器偏移

/* GPIO ODR寄存器地址计算 */
#define GPIOA_ODR_ADDR   (GPIOA_BASE + GPIO_ODR_OFFSET)
#define GPIOB_ODR_ADDR   (GPIOB_BASE + GPIO_ODR_OFFSET)
#define GPIOC_ODR_ADDR   (GPIOC_BASE + GPIO_ODR_OFFSET)
#define GPIOD_ODR_ADDR   (GPIOD_BASE + GPIO_ODR_OFFSET)
#define GPIOE_ODR_ADDR   (GPIOE_BASE + GPIO_ODR_OFFSET)

/* GPIO IDR寄存器地址计算 */
#define GPIOA_IDR_ADDR   (GPIOA_BASE + GPIO_IDR_OFFSET)
#define GPIOB_IDR_ADDR   (GPIOB_BASE + GPIO_IDR_OFFSET)
#define GPIOC_IDR_ADDR   (GPIOC_BASE + GPIO_IDR_OFFSET)
#define GPIOD_IDR_ADDR   (GPIOD_BASE + GPIO_IDR_OFFSET)
#define GPIOE_IDR_ADDR   (GPIOE_BASE + GPIO_IDR_OFFSET)

#elif defined STM32_F4xx_GPIO

#error undefined the STM32_F4xx GPIO Addr

#endif


/* 输出引脚位段别名定义 (用于控制输出电平) */
#define PA0_OUT  (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 0))
#define PA1_OUT  (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 1))
#define PA2_OUT  (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 2))
#define PA3_OUT  (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 3))
#define PA4_OUT  (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 4))
#define PA5_OUT  (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 5))
#define PA6_OUT  (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 6))
#define PA7_OUT  (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 7))
#define PA8_OUT  (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 8))
#define PA9_OUT  (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 9))
#define PA10_OUT (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 10))
#define PA11_OUT (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 11))
#define PA12_OUT (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 12))
#define PA13_OUT (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 13))
#define PA14_OUT (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 14))
#define PA15_OUT (*(volatile uint32_t *)BITBAND(GPIOA_ODR_ADDR, 15))

#define PB0_OUT  (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 0))
#define PB1_OUT  (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 1))
#define PB2_OUT  (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 2))
#define PB3_OUT  (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 3))
#define PB4_OUT  (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 4))
#define PB5_OUT  (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 5))
#define PB6_OUT  (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 6))
#define PB7_OUT  (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 7))
#define PB8_OUT  (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 8))
#define PB9_OUT  (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 9))
#define PB10_OUT (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 10))
#define PB11_OUT (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 11))
#define PB12_OUT (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 12))
#define PB13_OUT (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 13))
#define PB14_OUT (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 14))
#define PB15_OUT (*(volatile uint32_t *)BITBAND(GPIOB_ODR_ADDR, 15))

#define PC0_OUT  (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 0))
#define PC1_OUT  (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 1))
#define PC2_OUT  (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 2))
#define PC3_OUT  (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 3))
#define PC4_OUT  (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 4))
#define PC5_OUT  (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 5))
#define PC6_OUT  (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 6))
#define PC7_OUT  (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 7))
#define PC8_OUT  (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 8))
#define PC9_OUT  (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 9))
#define PC10_OUT (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 10))
#define PC11_OUT (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 11))
#define PC12_OUT (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 12))
#define PC13_OUT (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 13))
#define PC14_OUT (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 14))
#define PC15_OUT (*(volatile uint32_t *)BITBAND(GPIOC_ODR_ADDR, 15))

/* 输入引脚位段别名定义 (用于读取输入电平) */
#define PA0_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 0))
#define PA1_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 1))
#define PA2_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 2))
#define PA3_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 3))
#define PA4_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 4))
#define PA5_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 5))
#define PA6_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 6))
#define PA7_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 7))
#define PA8_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 8))
#define PA9_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 9))
#define PA10_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 10))
#define PA11_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 11))
#define PA12_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 12))
#define PA13_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 13))
#define PA14_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 14))
#define PA15_IN  (*(volatile uint32_t *)BITBAND(GPIOA_IDR_ADDR, 15))

#define PB0_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 0))
#define PB1_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 1))
#define PB2_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 2))
#define PB3_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 3))
#define PB4_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 4))
#define PB5_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 5))
#define PB6_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 6))
#define PB7_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 7))
#define PB8_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 8))
#define PB9_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 9))
#define PB10_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 10))
#define PB11_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 11))
#define PB12_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 12))
#define PB13_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 13))
#define PB14_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 14))
#define PB15_IN  (*(volatile uint32_t *)BITBAND(GPIOB_IDR_ADDR, 15))

#define PC0_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 0))
#define PC1_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 1))
#define PC2_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 2))
#define PC3_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 3))
#define PC4_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 4))
#define PC5_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 5))
#define PC6_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 6))
#define PC7_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 7))
#define PC8_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 8))
#define PC9_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 9))
#define PC10_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 10))
#define PC11_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 11))
#define PC12_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 12))
#define PC13_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 13))
#define PC14_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 14))
#define PC15_IN  (*(volatile uint32_t *)BITBAND(GPIOC_IDR_ADDR, 15))

#endif /* __BIT_BAND_GPIO_H */