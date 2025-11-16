using System;

namespace SeaBattleCSharp
{
    class Program
    {
        static void Main()
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            // Тестирование базовой функциональности
            TestGameClasses();
        }

        static void TestGameClasses()
        {
            Console.WriteLine("=== ТЕСТИРОВАНИЕ КЛАССОВ ИГРЫ ===");

            // Тест координат
            var coord = new Coordinate(5, 5);
            Console.WriteLine($"Координата: ({coord.X}, {coord.Y})");

            // Тест игровой доски
            var board = new GameBoard();
            Console.WriteLine("Пустая доска:");
            board.Display(true);

            // Тест корабля
            var ship = new Ship(3, new Coordinate(0, 0), Orientation.Horizontal);
            Console.WriteLine($"Корабль размером {ship.Size} создан");

            Console.WriteLine("Тестирование завершено!");
        }
    }
}