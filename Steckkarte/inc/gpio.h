/*
 * gpio.h
 *
 *  Created on: 17.03.2020
 *      Author: klosskopf
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32l4xx.h"
#include "stdbool.h"

typedef struct{GPIO_TypeDef * port; int pin;}GPIO_PIN;

typedef enum {OUT, IN, AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15, ANALOG}GPIO_TYPE;
typedef enum {PUSH_PULL, OPEN_DRAIN}GPIO_PUSH_PULL;
typedef enum {PULL_UP, PULL_DOWN, OPEN}GPIO_PULL_DOWN_UP;
typedef enum {LOW, MEDIUM, HIGH, VERY_HIGH}GPIO_SPEED;

void init_gpio(GPIO_PIN pin, GPIO_TYPE type, GPIO_PUSH_PULL pushpull, GPIO_PULL_DOWN_UP pullupdown, GPIO_SPEED speed);
void toggle_gpio(GPIO_PIN pin);
void set_gpio(GPIO_PIN pin, bool value);
bool read_gpio(GPIO_PIN pin);

#endif /* GPIO_H_ */
