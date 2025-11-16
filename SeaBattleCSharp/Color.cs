using System;

namespace SeaBattleCSharp
{
    public static class Color
    {
        private static ConsoleColor _originalColor = Console.ForegroundColor;

        public static void SetColor(ConsoleColor color)
        {
            Console.ForegroundColor = color;
        }

        public static void ResetColor()
        {
            Console.ForegroundColor = _originalColor;
        }

        public static void Red() => SetColor(ConsoleColor.Red);
        public static void Green() => SetColor(ConsoleColor.Green);
        public static void Blue() => SetColor(ConsoleColor.Blue);
        public static void Yellow() => SetColor(ConsoleColor.Yellow);
        public static void White() => SetColor(ConsoleColor.White);
        public static void Gray() => SetColor(ConsoleColor.Gray);
    }
}