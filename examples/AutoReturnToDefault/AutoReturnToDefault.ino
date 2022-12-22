/**
 * @file AutoReturnToDefault.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief 
 * @version 1.0
 * @date 13.07.2022
 * 
 * @copyright Copyright (c) 2022
 * 
 * пример возврата в режим по умолчанию при отсутствии активности пользователя;
 * при клике кнопкой загорается светодиод (например, подсветка экрана), через 
 * шесть секунд после последнего клика кнопкой светодиод гаснет
 */
#include <shButton.h> // https://github.com/VAleSh-Soft/shButton
#include <shTaskManager.h>

#define BTN_PIN 10
#define LED_PIN 13

shTaskManager tasks(1);
shButton btn(BTN_PIN);

shHandle auto_return_to_def_mode;

void autoReturnToDefMode()
{
  digitalWrite(LED_PIN, LOW);
  tasks.stopTask(auto_return_to_def_mode);
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  auto_return_to_def_mode = tasks.addTask(6000, autoReturnToDefMode, false);
}

void loop()
{
  tasks.tick();
  
  switch (btn.getButtonState())
  {
  case BTN_DOWN:
    digitalWrite(LED_PIN, HIGH);
    tasks.startTask(auto_return_to_def_mode);
    break;
  }
}