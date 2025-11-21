using System;

namespace SeaBattleCSharp
{
    class Program
    {
        static void Main()
        {
            try
            {
                Console.OutputEncoding = System.Text.Encoding.UTF8;
                Game game = new Game();
                game.Run();
            }
            catch (Exception ex)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"Критическая ошибка: {ex.Message}");
                Console.WriteLine($"Детали: {ex.StackTrace}");
                Console.ResetColor();
                Console.WriteLine("Нажмите Enter для выхода...");
                Console.ReadLine();
            }
        }
    }
}