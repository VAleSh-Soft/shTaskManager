#include "shTaskManager.h"
#include <Arduino.h>

shTaskManager::shTaskManager(uint8_t _taskCount)
{
  init(_taskCount);
}

shTaskManager::shTaskManager() {}

// shTaskManager::~shTaskManager() // если кому-то важно наличие деструктора, раскомментируйте
// {
//   delete[] taskList;
// }

void shTaskManager::init(uint8_t _taskCount)
{
  TASKCOUNT = (_taskCount) ? _taskCount : 1;
  taskList = new shTask[TASKCOUNT];
  for (uint8_t i = 0; i < TASKCOUNT; i++)
  {
    taskList[i].callback = NULL;
    taskList[i].status = false;
  }
}

shHandle shTaskManager::addTask(uint32_t _interval, shCallback _callback, bool isActive)
{
  int16_t result = INVALID_HANDLE;
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
    if (taskList[i].status && taskList[i].callback != NULL)
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

void shTaskManager::taskExes(shHandle _handle)
{
  if (isValidHandle(_handle))
  {
    if (taskList[_handle].callback != NULL)
    {
      taskList[_handle].timer = millis();
      taskList[_handle].callback();
    }
  }
}

uint32_t shTaskManager::getNextPoint()
{
  uint32_t result = UINT32_MAX;
  for (uint8_t i = 0; i < TASKCOUNT; i++)
  {
    if (taskList[i].status && taskList[i].callback != NULL)
    {
      uint32_t x = taskList[i].timer + taskList[i].interval - millis();
      result = min(result, x);
    }
  }
  return (result);
}

uint32_t shTaskManager::getNextTaskPoint(shHandle _handle)
{
  uint32_t result = UINT32_MAX;
  if (isValidHandle(_handle))
  {
    if (taskList[_handle].status && taskList[_handle].callback != NULL)
    {
      result = taskList[_handle].timer + taskList[_handle].interval - millis();
    }
  }
  return (result);
}

bool shTaskManager::getTaskState(shHandle _handle)
{
  return (isValidHandle(_handle) && taskList[_handle].status && taskList[_handle].callback != NULL);
}

void shTaskManager::setTaskInterval(shHandle _handle, uint32_t _interval, bool _restart)
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

uint16_t shTaskManager::getTaskCount(bool onlyActive)
{
  uint16_t result = 0;
  for (uint8_t i = 0; i < TASKCOUNT; i++)
  {
    if (taskList[i].callback)
    {
      if (onlyActive && !taskList[i].status)
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
