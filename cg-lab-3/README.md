# Лабораторная работа №3:
### Вариант 21: Цилиндрическая подкова

Заметки:
* Опять же, [LearnOpenGL][1] - хороший источник по свету, хотя в отчете
  должны быть все необходимые формулы. Сложность может появится при подсчете
  вектора отражения, но [здесь][2] есть решение
* Соболезнования всем, у кого цилиндрическая подкова. Здесь она строится
  через пять эллипсов, расположенных путем проб и ошибок, причем сначала
  рисуются концы, а потом сами стены, и, т.к. фигура не выпуклая, 
  нужен глубинный буфер.

[1]: https://learnopengl.com/
[2]: https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector