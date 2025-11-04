# Руководство по тестированию

## Запуск тестов

```bash
make test        # Компиляция тестов
make run-tests   # Запуск тестов
```

## Структура тестов

### Тестовый фреймворк

Простой встроенный фреймворк (`tests/TestFramework.h`):
- Автоматическая регистрация тестов через макрос `TEST()`
- Макросы для проверок: `ASSERT_TRUE`, `ASSERT_FALSE`, `ASSERT_EQ`, `ASSERT_NE`, `ASSERT_GE`, `ASSERT_LE`
- Подробный вывод результатов

### Тесты по компонентам

#### BurgerInventory (9 тестов)
- `BurgerInventory_InitialStock` - проверка начального состояния
- `BurgerInventory_BuyBurger` - покупка бургеров
- `BurgerInventory_BuyOutOfStock` - покупка при отсутствии
- `BurgerInventory_Restock` - пополнение инвентаря
- `BurgerInventory_IsOutOfStock` - проверка наличия
- `BurgerInventory_InvalidBurgerType` - обработка неверных типов
- `BurgerInventory_ThreadSafety` - потокобезопасность
- `BurgerInventory_Callback` - callback при окончании запаса
- `BurgerInventory_MultipleTypes` - работа с несколькими типами

#### TableManager (6 тестов)
- `TableManager_InitialState` - начальное состояние
- `TableManager_ReserveAndRelease` - резервирование и освобождение
- `TableManager_ReserveAllTables` - резервирование всех столиков
- `TableManager_ThreadSafety` - потокобезопасность
- `TableManager_WaitForTable` - ожидание свободного столика
- `TableManager_MultipleWaiters` - несколько ожидающих потоков

#### Manager (5 тестов)
- `Manager_Restock` - пополнение инвентаря
- `Manager_MultipleRestocks` - множественные пополнения
- `Manager_InvalidBurgerType` - обработка неверных типов
- `Manager_StopBeforeProcessing` - остановка до обработки
- `Manager_ThreadSafety` - потокобезопасность

#### Customer (4 теста)
- `Customer_BuyAndEat` - покупка и прием пищи
- `Customer_Stop` - остановка покупателя
- `Customer_HandleOutOfStock` - обработка отсутствия бургеров
- `Customer_MultipleCustomers` - работа нескольких покупателей

#### Restaurant (5 интеграционных тестов)
- `Restaurant_Initialization` - инициализация
- `Restaurant_StartStop` - запуск и остановка
- `Restaurant_Integration` - интеграционный тест
- `Restaurant_RestockOnOutOfStock` - пополнение при отсутствии
- `Restaurant_TableManagement` - управление столиками

## Всего: 29 тестов

## Покрытие тестами

✅ **Покрыты все основные компоненты:**
- Инвентарь бургеров
- Управление столиками
- Менеджер
- Покупатели
- Интеграция системы

✅ **Проверяются:**
- Базовые операции
- Потокобезопасность
- Обработка ошибок
- Граничные случаи
- Интеграционные сценарии

## Добавление новых тестов

```cpp
#include "TestFramework.h"
#include "../src/core/YourClass.h"

TEST(YourClass_TestName) {
    // Setup
    YourClass obj;
    
    // Test
    obj.doSomething();
    
    // Assert
    ASSERT_EQ(expected, actual);
}
```

Тесты автоматически регистрируются при компиляции.

