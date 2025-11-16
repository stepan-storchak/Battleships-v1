using System;

namespace SeaBattleCSharp
{
    class Program
    {
        /// Точка входа в приложение "Морской бой" на C#
        static void Main()
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            Game game = new Game();
            game.Run();
        }
    }
}