
namespace SeaBattleCSharp
{
    public static class Color
    {
        private static int usageCount = 0;

        public const int RED = 12;
        public const int GREEN = 10;
        public const int BLUE = 9;
        public const int YELLOW = 14;
        public const int WHITE = 15;
        public const int GRAY = 8;

        public static void SetColor(int color)
        {
            try
            {
                if (Environment.OSVersion.Platform == PlatformID.Win32NT)
                {
                    Console.ForegroundColor = (ConsoleColor)(color % 16);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"Ошибка при установке цвета: {e.Message}");
            }
        }

        public static void ResetColor()
        {
            try
            {
                Console.ResetColor();
            }
            catch (Exception e)
            {
                Console.WriteLine($"Ошибка при сбросе цвета: {e.Message}");
            }
        }

        public static int GetUsageCount() => usageCount;
    }
}