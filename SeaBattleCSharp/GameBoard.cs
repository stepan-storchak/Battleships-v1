// Добавляем в метод Display цветовое оформление:
public void Display(bool showShips)
{
    Console.Write("  ");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        Color.Yellow();
        Console.Write($"{(char)('A' + i)} ");
        Color.ResetColor();
    }
    Console.WriteLine();

    for (int y = 0; y < BOARD_SIZE; y++)
    {
        Color.Yellow();
        Console.Write($"{y + 1}");
        if (y < 9) Console.Write(" ");
        Color.ResetColor();
        Console.Write(" ");

        for (int x = 0; x < BOARD_SIZE; x++)
        {
            CellState state = grid[y, x];
            char symbol = '~';
            ConsoleColor color = ConsoleColor.Blue;

            switch (state)
            {
                case CellState.Empty:
                    symbol = '~';
                    color = ConsoleColor.Blue;
                    break;
                case CellState.Miss:
                    symbol = 'O';
                    color = ConsoleColor.Blue;
                    break;
                case CellState.Hit:
                    symbol = 'X';
                    color = ConsoleColor.Red;
                    break;
                case CellState.Ship1:
                    symbol = showShips ? '1' : '~';
                    color = showShips ? ConsoleColor.Green : ConsoleColor.Blue;
                    break;
                case CellState.Ship2:
                    symbol = showShips ? '2' : '~';
                    color = showShips ? ConsoleColor.Green : ConsoleColor.Blue;
                    break;
                case CellState.Ship3:
                    symbol = showShips ? '3' : '~';
                    color = showShips ? ConsoleColor.Green : ConsoleColor.Blue;
                    break;
                case CellState.Ship4:
                    symbol = showShips ? '4' : '~';
                    color = showShips ? ConsoleColor.Green : ConsoleColor.Blue;
                    break;
            }

            Color.SetColor(color);
            Console.Write($"{symbol} ");
            Color.ResetColor();
        }
        Console.WriteLine();
    }
}