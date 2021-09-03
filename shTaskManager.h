#pragma once
#include <Arduino.h>

using shCallback = void (*)(void); // тип - указатель для Callback-функции
using shHandle = int16_t;          // тип - идентификатор задачи
static const shHandle INVALID_HANDLE = -1;

struct shTask // структура, описывающая задачу
{
  bool status;         // статус задачи
  uint32_t timer;      // таймер задачи
  uint32_t interval;   // интервал срабатывания задачи
  shCallback callback; // функция, вызываемая при срабатывании таймера задачи
};

class shTaskManager
{
private:
  int16_t TASKCOUNT;                    // максимальное число задач
  shTask *taskList = NULL;              // массив - список задач
  bool isValidHandle(shHandle _handle); // проверка корректности идентификатора задачи

public:
  /* Конструктор класс
  - _taskCount - максимальное количество задач в списке;
  */
  shTaskManager(int16_t _taskCount);
  // Деструктор класса
  ~shTaskManager();
  // Опрос списка задач; необходимо выполнять максимально часто
  void tick();
  /* Добавление задачи в список
  - _interval - интервал срабатывания задачи в милисекундах;
  - _callback - функция, которая будет вызываться при срабатывании задачи; функция не должна принимать никаких аргументов и не должна возвращать никаких значений;
  - isActive - активна ли задача с момента добавления или будет запущена потом;
  новая задача добавляется в первую свободную ячейку списка;
  метод возвращает идентификатор задачи в списке в случае успешного добавления или -1 в случае неудачи;
  */
  shHandle addTask(uint32_t _interval, shCallback _callback, bool isActive = true);
  /* Удаление задачи из списка
  - _handle - идентификатор удаляемой задачи;
  */
  void delTask(shHandle _handle);
  /* Запуск задачи
  - _handle - идентификатор запускаемой задачи;
  */
  void startTask(shHandle _handle);
  /* Перезапуск задачи
  - _handle - идентификатор перезапускаемой задачи;
  */
  void restartTask(shHandle _handle);
  /* Остановка задачи
  - _handle - идентификатор останавливаемой задачи;
  */
  void stopTask(shHandle _handle);
  /* Немедленное выполнение задачи
  - _handle - идентификатор задачи;
  */
  void taskExes(shHandle _handle);
  // Получение времени в милисекундах до срабатывания следующей задачи
  uint32_t getNextPoint();
  /* Получение статуса задачи
  - _handle - идентификатор останавливаемой задачи;
  */
  bool getTaskState(shHandle _handle);
  /* Установка статуса задачи
  - _handle - идентификатор останавливаемой задачи;
  - _state - новый статус задачи;
  */
  void setTaskState(shHandle _handle, bool _state);
  /* Установка нового интервала срабатывания задачи
  - _interval - новое значение интервала срабатывания задачи;
  - _handle - идентификатор задачи;
  - _restart - перезапускать или нет задачу с новым интервалом;
  */
  void setTaskInterval(shHandle _handle, uint32_t _interval, bool _restart = true);
  /* Получение количества задач в списке
  - onlyActive - если true, то подсчитываются только активные задачи;
  */
  uint16_t getTaskCount(bool onlyActive = false);
};
