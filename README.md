Структура 
=========

1. launcher - небольшая программа для получения списка доступных тестов и вывода его пользователю. Для отладки скриптов в ней можно указать, что нужно запускать дебаггер.
2. TestWindow - приложение, проводящее тестирование. Запускается launcher'ом, в параметрах командной строки передаются название теста и нужен ли отладчик скриптов.
3. plotter  - плагин qt, содержащий методы для отрисовки графиков. 