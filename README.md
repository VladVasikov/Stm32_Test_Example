Пример проекта под МК STM32F405 на C++ с использованием библиотеки m.

1. Arm GNU Toolchain 13.3.Rel1 + Vscode + Embedded IDE plugin
2. Форматирование кода .clang-format Google
3. Стиль кода в основном опирается на "Руководство Google по стилю в C++" (Google C++ Style Guide: https://google.github.io/styleguide/cppguide.html)
4. Не используются: макросы, выделение памяти, RTTI, исключения.
5. Конфигурация периферии МК - полность из CubeMX.
6. main.cpp - основной, main.c исключён из проекта (генерируется автоматически из CubeMX)
7. RTOS не используется

Краткое описание:
1. Все задержки, таймауты и т.д. базируются на основе классов TimeUs, TimeMs, которые в свою очередь являются реализациями интерфейсов
   m::ifc::ITime<Us<uint32_t>> и m::ifc::ITime<Ms<uint32_t>>. 
