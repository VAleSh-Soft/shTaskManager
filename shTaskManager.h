#pragma once
#include <Arduino.h>

#ifndef UINT32_MAX
#define UINT32_MAX __UINT32_MAX__
#endif

typedef void (*shCallback)(void); // тип - указатель для Callback-функции
typedef int8_t shHandle;          // тип - идентификатор задачи

struct shTask // структура, описывающая задачу
{
  bool status;            // статус задачи
  unsigned long timer;    // таймер задачи
  unsigned long interval; // интервал срабатывания задачи
  shCallback callback;    // функция, вызываемая при срабатывании таймера задачи
};

class shTaskManager
{
private:
  uint8_t TASKCOUNT = 0;   // максимальное число задач
  shTask *taskList = NULL; // массив - список задач

  bool isValidHandle(shHandle _handle); // проверка корректности идентификатора задачи

public:
  /**
   * @brief Конструктор диспетчера задач;
   *
   * @param _taskCount максимальное количество задач, которое может быть добавлено в список;
   */
  shTaskManager(uint8_t _taskCount);

  /**
   * @brief Конструктор диспетчера задач; максимальное количество задач при таком варианте создания экземпляра диспетчера должно быть указано в последующем вызове метода init();
   *
   */
  shTaskManager();

  /**
   * @brief Деструктор класса // если кому-то важно наличие деструктора, раскомментируйте;
   *
   */
  // ~shTaskManager() { free(taskList); }

  /**
   * @brief Инициализация диспетчера задач, нужна, если экземпляр класса создан без указания количества задач;
   *
   * @param _taskCount максимальное количество задач, которое может быть добавлено в список;
   */
  void init(uint8_t _taskCount);

  /**
   * @brief Опрос списка задач и запуск тех из них, чье время выполнения наступило; необходимо выполнять максимально часто;
   *
   */
  void tick();

  /**
   * @brief Добавление задачи в список;
   *
   * @param _interval интервал срабатывания задачи в милисекундах;
   * @param _callback функция, которая будет вызываться при срабатывании задачи; функция не должна принимать никаких аргументов и не должна возвращать никаких значений;
   * @param isActive активна ли задача с момента добавления или будет запущена потом;
   * @return shHandle, идентификатор задачи в списке в случае успешного добавления ее в список или -1 в случае неудачи;
   */
  shHandle addTask(unsigned long _interval, shCallback _callback, bool isActive = true);

  /**
   * @brief Удаление задачи из списка;
   *
   * @param _handle идентификатор удаляемой задачи;
   */
  void delTask(shHandle _handle);

  /**
   * @brief Запуск задачи;
   *
   * @param _handle идентификатор запускаемой задачи;
   */
  void startTask(shHandle _handle);

  /**
   * @brief Перезапуск задачи;
   *
   * @param _handle идентификатор перезапускаемой задачи;
   */
  void restartTask(shHandle _handle);

  /**
   * @brief Остановка задачи;
   *
   * @param _handle идентификатор останавливаемой задачи;
   */
  void stopTask(shHandle _handle);

  /**
   * @brief Немедленное выполнение задачи;
   *
   * @param _handle идентификатор задачи;
   */
  void taskExes(shHandle _handle);

  /**
   * @brief Получение времени, оставшегося до срабатывания следующей задачи;
   *
   * @return unsigned long, время до срабатывания следующей задачи, милисекунд;
   */
  unsigned long getNextPoint();

  /*
  - _handle -
  */

  /**
   * @brief  Получение времени, оставшегося до следующего срабатывания задачи;
   *
   * @param _handle идентификатор задачи;
   * @return unsigned long, время до следующего срабатывания задачи, милисекунд;
   */
  unsigned long getNextTaskPoint(shHandle _handle);

  /**
   * @brief Получение статуса задачи;
   *
   * @param _handle идентификатор задачи;
   * @return true, задача активна;
   * @return false, задача неактивна или не существует;
   */
  bool getTaskState(shHandle _handle);

  /**
   * @brief Установка статуса задачи;
   *
   * @param _handle идентификатор задачи;
   * @param _state новый статус задачи;
   */
  void setTaskState(shHandle _handle, bool _state);

  /**
   * @brief Установка нового интервала срабатывания задачи;
   *
   * @param _handle идентификатор задачи;
   * @param _interval новое значение интервала срабатывания задачи;
   * @param _restart перезапускать или нет задачу с новым интервалом;
   */
  void setTaskInterval(shHandle _handle, unsigned long _interval, bool _restart = true);

  /**
   * @brief Получение количества задач в списке;
   *
   * @param onlyActive если true, то подсчитываются только активные задачи;
   * @return uint16_t, количество задач
   */
  uint16_t getTaskCount(bool onlyActive = false);
};
