## shTaskManager - легкий диспетчер задач для Arduino, основанный на системном таймере millis(). 

Возможности библиотеки:

- Простое создание списка задач;
- Добавление новых и удаление существующих задач;
- Запуск, перезапуск, остановка и получение статуса любой задачи по ее идентификатору;
- Получение времени до срабатывания как ближайшей, так и конкретной задачи;
- Получение количества задач в списке;

Версия 1.3 - 19.06.2022
+ добавлена возможность создания диспетчера без указания количества задач; в этом случае перед использованием диспетчера нужно вызвать функцию init() с указанием количества;
+ добавлен метод init(uint8_t _taskCount), который нужно вызвать перед заполнением списка задачами и указать максимальное количество задач в списке (если диспетчер был создан без указания количества задач);

Версия 1.2 - 06.11.2021
+ добавлен метод `getNextTaskPoint()`, возвращающий количество милисекунд до следующего срабатывания конкретной задачи; 

Версия 1.1 - 11.06.2021
+ добавлен метод `getTaskState()`, возвращающий данные об активности задачи; возвращает true если статус задачи "активна" и callback-функция для задачи назначена;
+ добавлен метод `setTaskState()`, позволяющий запускать или останавливать задачу с помощью флага;

Версия 1.0 - 08.06.2021

Библиотека позволяет организовать выполнение различных задач независимо друг от друга. 

При инициализации диспетчера задач сразу указывается максимальное количество задач, которые могут быть помещены в список. Например 2, если нужно управлять двумя светодиодами:

`shTaskManager tasks(2);`

Максимально возможное количество задач - 255, если МК с ними справится ))

Так же есть возможность создания диспетчера без указанием количества задач с инициализацией его позже; это может понадобиться, например, если количество задач зависит от параметров условной компиляции и не может быть заранее известно

`shTaskManager tasks;`

В этом случае перед заполнение списка задачами нужно вызвать метод `init(uint8_t _taskCount)` и указать максимальное количестов задач.

`tasks.init(2);`

Задачи добавляются в список с помощью метода:

`shHandle addTask(uint32_t _interval, shCallback _callback, bool isActive = true);`

где `_interval` - интервал срабатывания задачи в милисекундах, `_callback` - имя функции, которая будет вызываться при срабатывании задачи (функция не должна принимать аргументы и возвращать значения), `isActive` - запускать или нет задачу в момент добавления;
Метод вовращает идентификатор задачи, по которому можно будет этой задачей управлять.

Опрос списка задач выполняется методом:

`void tick();`

который нужно вызывать как можно чаще для надежного срабатывания задач.

Остальные методы библиотеки:

`void delTask(shHandle _handle);` - удаление задачи из списка; `_handle` - идентификатор задачи;

`void startTask(shHandle _handle);` - запуск задачи; `_handle` - идентификатор задачи;

`void restartTask(shHandle _handle);` - перезапуск задачи; при перезапуске интервал срабатывания задачи начинает отсчитываться заново; `_handle` - идентификатор задачи;

`void stopTask(shHandle _handle);` - остановка задачи; `_handle` - идентификатор задачи;

`void taskExes(shHandle _handle);` - немедленное выполнение задачи; при этом если задача активна, то она будет перезапущена, ее интервал начнет заново отсчитываться с момента выполнения этого метода; `_handle` - идентификатор задачи;

`void setTaskInterval(shHandle _handle, uint32_t _interval, bool _restart = true);` - изменение интервала срабатывания задачи; `_handle` - идентификатор задачи, `_interval` - новый интервал в милисекундах, _restart - перезапускать или нет задачу с новым интервалом;

`void setTaskState(shHandle _handle, bool _state);` - установка статуса задачи; `_handle` - идентификатор задачи, `_state` - новый статус; в зависимости от значения флага _state будет вызван либо метод `startTask() (_state == true)`, либо метод `stopTask() (_state == false)`;

`bool getTaskState(shHandle _handle);` - получение текущего состояния задачи; возвращает **true**, если статус задачи "активна" и callback-функция для задачи назначена; `_handle` - идентификатор задачи;

`uint32_t getNextPoint();` - получение времени в милисекундах до срабатывания следующей задачи; можно использовать, например, для погружения МК в сон на это время;

`uint32_t getNextTaskPoint(shHandle _handle);` - получение времени в милисекундах до следующего срабатывания задачи; `_handle` - идентификатор задачи;

`uint16_t getTaskCount(bool onlyActive = false);` - получение количества задач, помещенных в список; при `onlyActive == true` будет возвращено количество активных на данный момент задач, иначе - общее количество;

Если возникнут вопросы, пишите на valesh-soft@yandex.ru 