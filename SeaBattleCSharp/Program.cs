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
                Console.WriteLine($"Íåïðåäâèäåííàÿ îøèáêà: {ex.Message}");
                Console.WriteLine($"StackTrace: {ex.StackTrace}");
                Console.ResetColor();
                Console.WriteLine("Íàæìèòå Enter äëÿ âûõîäà...");
                Console.ReadLine();
            }
        }
    }
}