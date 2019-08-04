/*
 * 
 * Copyright 2019 Nikita Makarevich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

/*
 * Избегать сокращений в именах
 * Членам класса с модификатором private присваивается суффикс-подчёркивание
 * Начинать имена функций по возможности с get, set, compute, find, initialize
 * n_...
 * ..._number
 * i, j, k
 * is/has/can/should для bool
 * Симметричные имена -- get/set, 
   add/remove, create/destroy, start/stop, 
   insert/delete, increment/decrement, 
   old/new, begin/end, first/last, up/down, 
   min/max, next/previous, old/new, 
   open/close, show/hide, suspend/resume
 * Избегать дополнительного именования указателей (ptr)
 * Нельзя давать булевым (логическим) переменным имена, содержащие отрицание
 * Константы в перечислениях могут иметь префикс — общее имя типа
 * Классам исключений следует присваивать префикс Exception
 * Заголовочные файлы должны содержать защиту от вложенного включения
   #define для защиты от вложенного включения начинать с префикса _H_...
 * Директивы включения следует сортировать (по месту в иерархии системы, 
   ниже уровень — выше позиция) и группировать. Оставляйте пустую строку между группами.
 * Директивы включения должны располагаться только в начале файла
 * Разделы класса public, protected и private должны быть отсортированы. Все разделы должны быть явно указаны
 * Приведение типов должно быть явным. Никогда не полагайтесь на неявное приведение типов
   Использовать приведение типов С++
 * Переменные следует объявлять в как можно меньшей области видимости
 * Для бесконечных циклов следует использовать форму while (true) 
 * Ожидаемую часть следует располагать в части if, исключение — в части else
 * Константы с плавающей точкой следует записывать с десятичной точкой и с указанием по крайней мере одной цифры после запятой
 * Объявления классов следует оформлять следующим образом:
	class SomeClass : public BaseClass
	{
	public:
		...
		
	protected:
		...
		
	private:
		...
	}
 * Пробелы после if, for, while, do
 * После // следует ставить пробел, а сам комментарий следует начинать писать с большой буквы
 * Комментарии к классам и заголовкам методов следует делать в соответствии с соглашениями Doxygen
 * Комментировать #endif // <имя макроса> и закрывающую скобку namespace как } // Namespace <имя>
 * 
 * Любое из этих правил может быть умышленно нарушено для повышения читаемости кода
 */

#include "../lscl/lscl.hpp"

namespace LSCL
{
	
	
} // Namespace LSCL
