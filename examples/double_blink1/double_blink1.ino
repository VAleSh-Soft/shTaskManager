#include <shTaskManager.h>

// Пример независимого мигания двух светодиодов с разным интервалом

#define BLINK_1 5 // пин первого светодиода
#define BLINK_2 6 // пин второго светодиода

// создаем список на две задачи
shTaskManager tasks(2);

// обработчики задач
void blink1()
{
  digitalWrite(BLINK_1, !digitalRead(BLINK_1));
}

void blink2()
{
  digitalWrite(BLINK_2, !digitalRead(BLINK_2));
}

void setup()
{
  pinMode(BLINK_1, OUTPUT);
  pinMode(BLINK_2, OUTPUT);
  // добавляем задачи для мигания светодиодов с разными интервалами
  tasks.addTask(500, blink1);
  tasks.addTask(200, blink2);
}

void loop()
{
  tasks.tick(); // опрос списка задач, необходимо запускать как можно чаще
}
