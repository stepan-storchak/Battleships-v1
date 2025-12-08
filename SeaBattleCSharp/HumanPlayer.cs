using System;
using System.Linq;

namespace SeaBattleCSharp
{
    public class HumanPlayer : Player
    {
        private readonly int[] shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
        private const int BOARD_SIZE = 10;
        private const int MAX_SINGLE_PLACEMENT_TRIES = 100;

        public HumanPlayer(string name) : base(name) { }

        public override bool IsHuman() => true;

        public override void PlaceShips()
        {
            Console.WriteLine($"Расстановка кораблей для игрока: {name}");

            int choice;
            while (true)
            {
                Console.WriteLine("Выберите способ расстановки:");
                Color.SetColor(Color.GREEN);
                Console.WriteLine("1 - Автоматическая расстановка");
                Color.SetColor(Color.YELLOW);
                Console.WriteLine("2 - Ручная расстановка");
                Color.ResetColor();
                Console.Write("Ваш выбор: ");

                if (int.TryParse(Console.ReadLine(), out choice))
                {
                    if (choice == 1)
                    {
                        if (AutomaticPlacement())
                            break;
                        else
                        {
                            Color.SetColor(Color.RED);
                            Console.WriteLine("Автоматическая расстановка не удалась. Попробуйте ручную расстановку.");
                            Color.ResetColor();
                        }
                    }
                    else if (choice == 2)
                    {
                        ManualPlacement();
                        break;
                    }
                    else
                    {
                        Color.SetColor(Color.RED);
                        Console.WriteLine("Неверный выбор! Попробуйте снова.");
                        Color.ResetColor();
                    }
                }
            }
        }

        public override void MakeMove(Player enemy)
        {
            MakeMoveWithResult(enemy);
        }

        public override bool MakeMoveWithResult(Player enemy)
        {
            Console.WriteLine($"\n=== Ход игрока. {name} ===");

            Color.SetColor(Color.GREEN);
            Console.WriteLine("Ваше поле:");
            Color.ResetColor();
            myBoard.Display(true);

            Color.SetColor(Color.BLUE);
            Console.WriteLine("\nПоле противника:");
            Color.ResetColor();
            enemyBoard.Display(false);

            Coordinate target = InputCoordinate();
            ShotResult result = enemy.GetShotResult(target);
            bool wasHit = false;

            switch (result)
            {
                case ShotResult.Miss:
                    enemyBoard.SetCellState(target, CellState.Miss);
                    Color.SetColor(Color.BLUE);
                    Console.WriteLine("Промах!");
                    Color.ResetColor();
                    wasHit = false;
                    break;

                case ShotResult.Hit:
                    enemyBoard.SetCellState(target, CellState.Hit);
                    Color.SetColor(Color.YELLOW);
                    Console.WriteLine("Попадание!");
                    Color.ResetColor();
                    wasHit = true;
                    break;

                case ShotResult.Sunk:
                    enemyBoard.SetCellState(target, CellState.Hit);
                    Color.SetColor(Color.RED);
                    Console.WriteLine("Уничтожен корабль!");
                    Color.ResetColor();
                    wasHit = true;
                    MarkAreaAroundDestroyedShip(enemy, target);
                    break;
            }

            return wasHit;
        }

        public override void MarkAreaAroundDestroyedShip(Player enemy, Coordinate hitCoord)
        {
            MarkSurroundingCells(hitCoord);

            for (int radius = 1; radius <= 3; radius++)
            {
                for (int dy = -radius; dy <= radius; dy++)
                {
                    for (int dx = -radius; dx <= radius; dx++)
                    {
                        Coordinate around = new Coordinate(hitCoord.X + dx, hitCoord.Y + dy);
                        if (around.X >= 0 && around.X < BOARD_SIZE &&
                            around.Y >= 0 && around.Y < BOARD_SIZE)
                        {
                            if (enemyBoard.GetCellState(around) == CellState.Hit)
                            {
                                MarkSurroundingCells(around);
                            }
                        }
                    }
                }
            }
        }

        private void MarkSurroundingCells(Coordinate center)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    Coordinate around = new Coordinate(center.X + dx, center.Y + dy);
                    if (around.X >= 0 && around.X < BOARD_SIZE &&
                        around.Y >= 0 && around.Y < BOARD_SIZE)
                    {
                        if (enemyBoard.GetCellState(around) == CellState.Empty)
                        {
                            enemyBoard.SetCellState(around, CellState.Miss);
                        }
                    }
                }
            }
        }

        private Coordinate InputCoordinate()
        {
            while (true)
            {
                Console.Write("Введите координаты для выстрела (например, A1): ");
                string input = Console.ReadLine()?.ToUpper();

                if (string.IsNullOrEmpty(input) || input.Length < 2)
                {
                    Color.SetColor(Color.RED);
                    Console.WriteLine("Неверный формат!");
                    Color.ResetColor();
                    continue;
                }

                char letter = input[0];
                if (!int.TryParse(input.Substring(1), out int number))
                {
                    Color.SetColor(Color.RED);
                    Console.WriteLine("Неверный формат числа!");
                    Color.ResetColor();
                    continue;
                }

                int x = letter - 'A';
                int y = number - 1;

                if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
                {
                    Coordinate coord = new Coordinate(x, y);
                    CellState state = enemyBoard.GetCellState(coord);

                    if (state == CellState.Empty)
                        return coord;
                    else
                    {
                        Color.SetColor(Color.RED);
                        Console.WriteLine("Вы уже стреляли в эту клетку!");
                        Color.ResetColor();
                    }
                }
                else
                {
                    Color.SetColor(Color.RED);
                    Console.WriteLine("Неверные координаты! Используйте формат A1-J10.");
                    Color.ResetColor();
                }
            }
        }

        private void ManualPlacement()
        {
            string[] shipNames = { "четырехпалубный", "трехпалубный", "трехпалубный",
                                   "двухпалубный", "двухпалубный", "двухпалубный",
                                   "однопалубный", "однопалубный", "однопалубный", "однопалубный" };

            for (int i = 0; i < shipSizes.Length; i++)
            {
                int size = shipSizes[i];
                bool placed = false;

                while (!placed)
                {
                    Color.SetColor(Color.YELLOW);
                    Console.WriteLine($"\nРазмещение {shipNames[i]} корабля ({size} палубы)");
                    Color.ResetColor();

                    myBoard.Display(true);

                    Console.Write("Введите начальную координату (например, A1): ");
                    string input = Console.ReadLine()?.ToUpper();

                    if (string.IsNullOrEmpty(input) || input.Length < 2)
                    {
                        Color.SetColor(Color.RED);
                        Console.WriteLine("Неверный формат!");
                        Color.ResetColor();
                        continue;
                    }

                    char letter = input[0];
                    if (!int.TryParse(input.Substring(1), out int number))
                    {
                        Color.SetColor(Color.RED);
                        Console.WriteLine("Неверный формат числа!");
                        Color.ResetColor();
                        continue;
                    }

                    int x = letter - 'A';
                    int y = number - 1;

                    if (size > 1)
                    {
                        Console.Write("Выберите ориентацию (H - горизонтально, V - вертикально): ");
                        char orient = char.ToUpper(Console.ReadKey().KeyChar);
                        Console.WriteLine();

                        Orientation orientation = (orient == 'H') ? Orientation.Horizontal : Orientation.Vertical;
                        Ship ship = new Ship(size, new Coordinate(x, y), orientation);

                        if (IsValidShipPlacement(size, new Coordinate(x, y), orientation))
                        {
                            if (myBoard.PlaceShip(ship))
                            {
                                ships.Add(ship);
                                placed = true;
                                Color.SetColor(Color.GREEN);
                                Console.WriteLine("Корабль размещен успешно!");
                                Color.ResetColor();
                            }
                        }
                        else
                        {
                            Color.SetColor(Color.RED);
                            Console.WriteLine("Неверное размещение корабля! Попробуйте снова.");
                            Color.ResetColor();
                        }
                    }
                    else
                    {
                        Ship ship = new Ship(size, new Coordinate(x, y), Orientation.Horizontal);

                        if (IsValidShipPlacement(size, new Coordinate(x, y), Orientation.Horizontal))
                        {
                            if (myBoard.PlaceShip(ship))
                            {
                                ships.Add(ship);
                                placed = true;
                                Color.SetColor(Color.GREEN);
                                Console.WriteLine("Корабль размещен успешно!");
                                Color.ResetColor();
                            }
                        }
                        else
                        {
                            Color.SetColor(Color.RED);
                            Console.WriteLine("Неверное размещение корабля! Попробуйте снова.");
                            Color.ResetColor();
                        }
                    }
                }
            }
        }

        private bool AutomaticPlacement()
        {
            int attempts = 0;
            const int MAX_ATTEMPTS = 5;
            Random random = new Random();

            while (attempts < MAX_ATTEMPTS)
            {
                myBoard.ClearBoard();
                ships.Clear();
                bool success = true;

                foreach (int size in shipSizes)
                {
                    bool placed = false;
                    int placementAttempts = 0;

                    while (!placed && placementAttempts < MAX_SINGLE_PLACEMENT_TRIES)
                    {
                        int x = random.Next(BOARD_SIZE);
                        int y = random.Next(BOARD_SIZE);
                        Orientation orientation = (random.Next(2) == 0) ?
                            Orientation.Horizontal : Orientation.Vertical;

                        Ship ship = new Ship(size, new Coordinate(x, y), orientation);

                        if (myBoard.IsValidPlacement(ship))
                        {
                            if (myBoard.PlaceShip(ship))
                            {
                                ships.Add(ship);
                                placed = true;
                            }
                        }
                        placementAttempts++;
                    }

                    if (!placed)
                    {
                        success = false;
                        break;
                    }
                }

                if (success)
                {
                    Color.SetColor(Color.GREEN);
                    Console.WriteLine("Автоматическая расстановка завершена!");
                    Color.ResetColor();
                    myBoard.Display(true);

                    if (attempts > 0)
                    {
                        Console.WriteLine($"Потребовалось {attempts + 1} попыток для успешной расстановки.");
                    }
                    return true;
                }
                attempts++;
            }

            Color.SetColor(Color.RED);
            Console.WriteLine($"Не удалось автоматически расставить корабли после {MAX_ATTEMPTS} попыток.");
            Color.ResetColor();
            return false;
        }

        private bool IsValidShipPlacement(int size, Coordinate start, Orientation orientation)
        {
            Ship tempShip = new Ship(size, start, orientation);
            return myBoard.IsValidPlacement(tempShip);
        }
    }
}