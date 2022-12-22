/**
 * @file double_blink2.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief 
 * @version 1.0
 * @date 08.06.2021
 * 
 * @copyright Copyright (c) 2022
 * 
 * Пример взаимного управления миганием светодиодов. Первый светодиод 
 * начинает мигать при старте МК, второй выключен. После того, как 
 * первый светодиод мигнет пять раз, он выключится и начнет мигать 
 * второй. После того, как второй светодиод мигнет 3 раза, цикл 
 * начнется заново
 */
#include <shTaskManager.h>

#define BLINK_1 5 // пин первого светодиода
#define BLINK_2 6 // пин второго светодиода

// создаем список на две задачи
shTaskManager tasks(2);

// идентификаторы задач
shHandle tBlink1;
shHandle tBlink2;

// обработчики задач
void blink1()
{
  static byte i = 0;
  digitalWrite(BLINK_1, !digitalRead(BLINK_1));
  if (++i >= 10) 
  { // после пяти включений и пяти выключений останавливаем задачу и запускаем вторую
    i = 0;
    tasks.stopTask(tBlink1);
    tasks.startTask(tBlink2);
  }
}

void blink2()
{
  static byte i = 0;
  digitalWrite(BLINK_2, !digitalRead(BLINK_2));
  if (++i >= 6)
  { // после трех включений и трех выключений останавливаем задачу и снова запускаем первую
    i = 0;
    tasks.stopTask(tBlink2);
    tasks.startTask(tBlink1);
  }
}

void setup()
{
  pinMode(BLINK_1, OUTPUT);
  pinMode(BLINK_2, OUTPUT);
  // добавляем задачи для мигания светодиодов и сохраняем их идентификаторы для последующего доступа к ним для остановки и перезапуска; вторая задача изначально неактивна
  tBlink1 = tasks.addTask(500, blink1);
  tBlink2 = tasks.addTask(200, blink2, false);
}

void loop()
{
  tasks.tick(); // опрос списка задач, необходимо запускать как можно чаще
}
