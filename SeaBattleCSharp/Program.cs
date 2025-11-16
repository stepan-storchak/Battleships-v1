using System;

namespace SeaBattleCSharp
{
    class Program
    {
        static void Main()
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            Game game = new Game();
            game.Run();
        }
    }
}