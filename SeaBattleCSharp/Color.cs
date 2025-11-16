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
    }
}