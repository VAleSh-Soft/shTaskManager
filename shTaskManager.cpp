#include "shTaskManager.h"
#include <Arduino.h>

static const shHandle INVALID_HANDLE = -1;

shTaskManager::shTaskManager(uint8_t _taskCount)
{
  init(_taskCount);
}

shTaskManager::shTaskManager() {}

void shTaskManager::init(uint8_t _taskCount)
{
  free(taskList); // освобождаем ранее выделенную память, если init вызывается повторно
  taskList = NULL;
  TASKCOUNT = (_taskCount) ? _taskCount : 1;
  taskList = (shTask *)calloc(TASKCOUNT, sizeof(shTask));
  if (taskList == NULL)
  {
    TASKCOUNT = 0;
  }
}

shHandle shTaskManager::addTask(unsigned long _interval, shCallback _callback, bool isActive)
{
  int16_t result = INVALID_HANDLE;

  if (_callback != NULL) // задача без callback не добавляется
  {
    for (uint8_t i = 0; i < TASKCOUNT; i++)
    {
      if (taskList[i].callback == NULL)
      {
        taskList[i].status = isActive;
        taskList[i].interval = _interval;
        taskList[i].callback = _callback;
        taskList[i].timer = millis();
        result = i;
        break;
      }
    }
  }
  
  return (result);
}

void shTaskManager::delTask(shHandle _handle)
{
  if (isValidHandle(_handle))
  {
    taskList[_handle].callback = NULL;
  }
}

void shTaskManager::tick()
{
  for (uint8_t i = 0; i < TASKCOUNT; i++)
  {
    if (taskList[i].status && taskList[i].interval && taskList[i].callback != NULL)
    {
      if (millis() - taskList[i].timer >= taskList[i].interval)
      {
        taskList[i].timer += taskList[i].interval;
        taskList[i].callback();
      }
    }
  }
}

void shTaskManager::startTask(shHandle _handle)
{
  if (isValidHandle(_handle))
  {
    if (taskList[_handle].callback != NULL)
    {
      taskList[_handle].status = true;
      taskList[_handle].timer = millis();
    }
  }
}

void shTaskManager::restartTask(shHandle _handle)
{
  startTask(_handle);
}

void shTaskManager::stopTask(shHandle _handle)
{
  if (isValidHandle(_handle))
  {
    taskList[_handle].status = false;
  }
}

void shTaskManager::taskExes(shHandle _handle, bool _restart)
{
  if (isValidHandle(_handle))
  {
    if (taskList[_handle].callback != NULL)
    {
      if (_restart)
      {
        taskList[_handle].status = true;
        taskList[_handle].timer = millis();
      }
      taskList[_handle].callback();
    }
  }
}

unsigned long shTaskManager::getNextPoint()
{
  unsigned long result = UINT32_MAX;
  unsigned long now = millis();

  for (uint8_t i = 0; i < TASKCOUNT; i++)
  {
    if (taskList[i].status && taskList[i].interval && taskList[i].callback != NULL)
    {
      result = min(result, _getNextTime(now, i));
    }
  }
  return (result);
}

unsigned long shTaskManager::getNextTaskPoint(shHandle _handle)
{
  unsigned long result = UINT32_MAX;

  if (isValidHandle(_handle))
  {
    if (taskList[_handle].status && taskList[_handle].interval && taskList[_handle].callback != NULL)
    {

      return _getNextTime(millis(), _handle);
    }
  }
  return (result);
}

bool shTaskManager::getTaskState(shHandle _handle)
{
  bool result = isValidHandle(_handle);
  if (result && (taskList != NULL))
  {
    result = taskList[_handle].status && taskList[_handle].interval && taskList[_handle].callback != NULL;
  }
  return (result);
}

void shTaskManager::setTaskInterval(shHandle _handle, unsigned long _interval, bool _restart)
{
  if (isValidHandle(_handle))
  {
    taskList[_handle].interval = _interval;
    if (_restart && (taskList[_handle].callback != NULL))
    {
      taskList[_handle].status = true;
      taskList[_handle].timer = millis();
    }
  }
}

void shTaskManager::setTaskState(shHandle _handle, bool _state)
{
  (_state) ? startTask(_handle) : stopTask(_handle);
}

uint16_t shTaskManager::getTaskCount(bool _only_active)
{
  uint16_t result = 0;
  for (uint8_t i = 0; i < TASKCOUNT; i++)
  {
    if (taskList[i].callback)
    {
      if (_only_active && (!taskList[i].status ||
                           !taskList[i].interval ||
                           taskList[i].callback == NULL))
      {
        continue;
      }
      result++;
    }
  }
  return (result);
}

bool shTaskManager::isValidHandle(shHandle _handle)
{
  return (_handle > INVALID_HANDLE && _handle < TASKCOUNT);
}

unsigned long shTaskManager::_getNextTime(unsigned long _now, shHandle _handle)
{
  unsigned long deadline = taskList[_handle].timer + taskList[_handle].interval;
  // если время срабатывания уже наступило - возвращаем 0, чтобы не получить
  // переполнение беззнакового типа при вычитании
  return ((_now >= deadline) ? 0 : (deadline - _now));
}