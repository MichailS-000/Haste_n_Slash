# Документация по Lua-скриптингу

## Содержание
1. [Управление сущностями](#управление-сущностями)
2. [Компоненты](#компоненты)
3. [Время](#время)
4. [Обработка ввода](#обработка-ввода)
5. [Аудио](#аудио)
6. [Переменные](#переменные)

---

## Управление сущностями <a name="управление-сущностями"></a>

### Функции

#### `addEntity()`
Создает новую сущность и возвращает ее.  
**Возвращает**: Объект `Entity`.  
**Пример**:
```lua
local newEntity = addEntity()
```

#### `getCurrentEntity()`
Возвращает сущность, связанную с текущим выполняемым скриптом.  
**Возвращает**: Объект `Entity`.  
**Пример**:
```lua
local currentEntity = getCurrentEntity()
```

#### `getFirstEntityWithComponent(componentName)`
Находит первую сущность с указанным компонентом.  
**Параметры**:
- `componentName` (строка): Имя компонента (например, `"Transform"`).  
**Возвращает**: `Entity` или `nil`.  
**Пример**:
```lua
local cameraEntity = getFirstEntityWithComponent("Camera")
```

#### `getAllEntitiesWithComponent(componentName)`
Возвращает таблицу всех сущностей с указанным компонентом.  
**Параметры**:
- `componentName` (строка): Имя компонента.  
**Возвращает**: Таблица Lua с объектами `Entity`.  
**Пример**:
```lua
local sprites = getAllEntitiesWithComponent("Sprite")
for i, entity in ipairs(sprites) do
    print(entity:getID())
end
```

#### `getEntityWithTag(tag)`
Находит первую сущность с указанным тегом.  
**Параметры**:
- `tag` (строка): Значение тега.  
**Возвращает**: `Entity` или `nil`.  
**Пример**:
```lua
local player = getEntityWithTag("Player")
```

#### `getAllEntitiesWithTag(tag)`
Возвращает все сущности с указанным тегом.  
**Параметры**:
- `tag` (строка): Значение тега.  
**Возвращает**: Таблица Lua с объектами `Entity`.  
**Пример**:
```lua
local enemies = getAllEntitiesWithTag("Enemy")
```

---

### Методы сущности (Entity)
```lua
Entity:getID()          -- Возвращает уникальный ID сущности.
Entity:getComponent("ComponentName")  -- Возвращает компонент (например, `entity.Transform`).
Entity:addComponent("ComponentName")  -- Добавляет новый компонент к сущности.
Entity:removeComponent("ComponentName")  -- Удаляет компонент.
Entity:destroy()        -- Уничтожает сущность.
Entity.transform       -- Свойство для доступа к компоненту Transform.
```

**Пример**:
```lua
local entity = addEntity()
entity:addComponent("Sprite")
local transform = entity.transform
transform.positionX = 100
```

---

## Компоненты <a name="компоненты"></a>

### Общие свойства
Все компоненты наследуются от `ComponentBase`:
- `.enabled` (логическое): Включает/отключает компонент.

---

### Компонент Transform
**Класс**: `entity.Transform`  
**Свойства**:
- `.positionX`, `.positionY` (число): Позиция в мире.
- `.scaleX`, `.scaleY` (число): Масштаб.

**Методы**:
- `movePosition(x, y)`: Перемещает сущность относительно текущей позиции.  
**Пример**:
```lua
local transform = entity:getComponent("Transform")
transform:movePosition(10, 0)
```

---

### Компонент Camera
**Класс**: `entity.Camera`  
**Свойства**:
- `.scale` (число): Уровень масштаба камеры.

---

### Компонент Sprite
**Класс**: `entity.Sprite`  
**Свойства**:
- `.textureName` (строка): Имя загруженной текстуры.

---

### Компонент Tag
**Класс**: `entity.Tag`  
**Свойства**:
- `.tag` (строка): Тег для идентификации сущности.

---

## Время <a name="время"></a>

### Функции (пространство имен `time`)
- `time.getTime()`: Возвращает общее время работы приложения (в секундах).  
- `time.getDeltaTime()`: Возвращает время между кадрами (в секундах).  

**Пример**:
```lua
function update()
    local speed = 50
    local moveAmount = speed * time.getDeltaTime()
    transform:movePosition(moveAmount, 0)
end
```

---

## Обработка ввода <a name="обработка-ввода"></a>

### Функции (пространство имен `input`)
- `input.getKeyDown(key)`: Возвращает `true`, если клавиша была нажата в текущем кадре.  
- `input.getKey(key)`: Возвращает `true`, если клавиша удерживается.  
- `input.getKeyUp(key)`: Возвращает `true`, если клавиша была отпущена в текущем кадре.  

**Поддерживаемые клавиши**: Используйте названия клавиш SDL (например, `"Space"`, `"A"`, `"Escape"`).  
**Пример**:
```lua
if input.getKey("W") then
    transform:movePosition(0, 5 * time.getDeltaTime())
end
```

---

## Аудио <a name="аудио"></a>

### Функции (пространство имен `audio`)
- `audio.playSound(soundName)`: Проигрывает звук один раз.  
- `audio.startPlayMusicGroup(musicGroupName)`: Начинает проигрывание музыки.  
- `audio.stopPlayingMusic()`: Останавливает всю музыку.  

**Пример**:
```lua
audio.playSound("Jump")
audio.startPlayMusicGroup("BackgroundMusic")
```

---

## Переменные <a name="переменные"></a>

### `setVariable(varName, value)`
Сохраняет значение, связанное с текущей сущностью.  
**Примечание**: Переменные уникальны для каждой сущности (внутри используется `varName + ID_сущности`).  

### `getVariable(varName)`
Получает ранее сохраненное значение.  

**Пример**:
```lua
setVariable("health", 100)
local health = getVariable("health")
```