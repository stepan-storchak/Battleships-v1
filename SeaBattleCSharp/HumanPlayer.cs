using System;
using System.Collections.Generic;

namespace SeaBattleCSharp
{
    public class HumanPlayer : Player
    {
        private static readonly int[] shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
        private static readonly string[] shipNames = {
            "четырехпалубный", "трехпалубный", "трехпалубный",
            "двухпалубный", "двухпалубный", "двухпалубный",
            "однопалубный", "однопалубный", "однопалубный", "однопалубный"
        };

        public HumanPlayer(string name) : base(name) { }

        public override void PlaceShips()
        {
            Console.WriteLine($"Расстановка кораблей для игрока: {name}");
            int choice;

            while (true)
            {
                Console.WriteLine("Выберите способ расстановки:");
                Color.Green();
                Console.WriteLine("1 - Автоматическая расстановка");
                Color.Yellow();
                Console.WriteLine("2 - Ручная расстановка");
                Color.ResetColor();
                Console.Write("Ваш выбор: ");

                try
                {
                    if (int.TryParse(Console.ReadLine(), out choice))
                    {
                        if (choice == 1)
                        {
                            if (AutomaticPlacement())
                            {
                                break;
                            }
                            else
                            {
                                Color.Red();
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
                            throw new ArgumentOutOfRangeException("Неверный выбор способа расстановки");
                        }
                    }
                    else
                    {
                        throw new FormatException("Неверный формат ввода");
                    }
                }
                catch (Exception ex)
                {
                    Color.Red();
                    Console.WriteLine($"Ошибка: {ex.Message}");
                    Color.ResetColor();
                }
            }
        }

        public override void MakeMove(Player enemy)
        {
            MakeMoveWithResult(enemy);
        }

        public override bool MakeMoveWithResult(Player enemy)
        {
            Console.WriteLine($"\n=== Ход игрока {name} ===");
            Color.Green();
            Console.WriteLine("Ваше поле:");
            Color.ResetColor();
            myBoard.Display(true);
            Color.Blue();
            Console.WriteLine("\nПоле противника:");
            Color.ResetColor();
            enemyBoard.Display(false);

            Coordinate target = InputCoordinate();

            try
            {
                ShotResult result = enemy.GetShotResult(target);
                bool wasHit = false;

                switch (result)
                {
                    case ShotResult.Miss:
                        enemyBoard.SetCellState(target, CellState.Miss);
                        Color.Blue();
                        Console.WriteLine("Промах!");
                        Color.ResetColor();
                        wasHit = false;
                        break;
                    case ShotResult.Hit:
                        enemyBoard.SetCellState(target, CellState.Hit);
                        Color.Yellow();
                        Console.WriteLine("Попадание!");
                        Color.ResetColor();
                        wasHit = true;
                        break;
                    case ShotResult.Sunk:
                        enemyBoard.SetCellState(target, CellState.Hit);
                        Color.Red();
                        Console.WriteLine("Уничтожен корабль!");
                        Color.ResetColor();
                        wasHit = true;
                        MarkAreaAroundDestroyedShip(enemy, target);
                        break;
                }
                return wasHit;
            }
            catch (Exception ex)
            {
                Color.Red();
                Console.WriteLine($"Ошибка при выстреле: {ex.Message}");
                Color.ResetColor();
                return false;
            }
        }

        public override void MarkAreaAroundDestroyedShip(Player enemy, Coordinate hitCoord)
        {
            try
            {
                var enemyShips = enemy.GetShips();
                ShipBase destroyedShip = null;
                foreach (var ship in enemyShips)
                {
                    if (ship.Coordinates.Exists(c => c.X == hitCoord.X && c.Y == hitCoord.Y) && ship.IsSunk())
                    {
                        destroyedShip = ship;
                        break;
                    }
                }

                if (destroyedShip != null)
                {
                    foreach (var shipCoord in destroyedShip.Coordinates)
                    {
                        for (int dy = -1; dy <= 1; dy++)
                        {
                            for (int dx = -1; dx <= 1; dx++)
                            {
                                Coordinate around = new Coordinate(shipCoord.X + dx, shipCoord.Y + dy);
                                if (around.X >= 0 && around.X < GameBoard.GetBoardSize() &&
                                    around.Y >= 0 && around.Y < GameBoard.GetBoardSize())
                                {
                                    if (enemyBoard.GetCellState(around) == CellState.Empty)
                                    {
                                        enemyBoard.SetCellState(around, CellState.Miss);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка при отметке области вокруг корабля: {ex.Message}");
            }
        }

        private Coordinate InputCoordinate()
        {
            while (true)
            {
                try
                {
                    Console.Write("Введите координаты для выстрела (например, A1): ");
                    string input = Console.ReadLine()?.ToUpper();

                    if (string.IsNullOrEmpty(input))
                    {
                        throw new ArgumentException("Ввод не может быть пустым");
                    }

                    if (input.Length < 2)
                    {
                        throw new ArgumentException("Неверный формат координат");
                    }

                    char letter = input[0];
                    if (!int.TryParse(input.Substring(1), out int number))
                    {
                        throw new FormatException("Неверный числовой формат координаты");
                    }

                    int x = letter - 'A';
                    int y = number - 1;

                    if (x < 0 || x >= GameBoard.GetBoardSize() || y < 0 || y >= GameBoard.GetBoardSize())
                    {
                        throw new ArgumentOutOfRangeException("Координаты выходят за пределы поля");
                    }

                    Coordinate coord = new Coordinate(x, y);
                    CellState state = enemyBoard.GetCellState(coord);

                    if (state == CellState.Empty)
                    {
                        return coord;
                    }
                    else
                    {
                        throw new InvalidOperationException("Вы уже стреляли в эту клетку!");
                    }
                }
                catch (Exception ex)
                {
                    Color.Red();
                    Console.WriteLine($"Ошибка: {ex.Message}");
                    Color.ResetColor();
                    Console.WriteLine("Используйте формат A1-J10.");
                }
            }
        }

        private void ManualPlacement()
        {
            for (int i = 0; i < shipSizes.Length; i++)
            {
                int size = shipSizes[i];
                bool placed = false;

                while (!placed)
                {
                    try
                    {
                        Color.Yellow();
                        Console.WriteLine($"\nРазмещение {shipNames[i]} корабля ({size} палубы)");
                        Color.ResetColor();
                        myBoard.Display(true);

                        Console.Write("Введите начальную координату (например, A1): ");
                        string input = Console.ReadLine()?.ToUpper();

                        if (string.IsNullOrEmpty(input) || input.Length < 2)
                        {
                            throw new ArgumentException("Неверный формат координат");
                        }

                        char letter = input[0];
                        if (!int.TryParse(input.Substring(1), out int number))
                        {
                            throw new FormatException("Неверный числовой формат координаты");
                        }

                        int x = letter - 'A';
                        int y = number - 1;

                        if (x < 0 || x >= GameBoard.GetBoardSize() || y < 0 || y >= GameBoard.GetBoardSize())
                        {
                            throw new ArgumentOutOfRangeException("Координаты выходят за пределы поля");
                        }

                        if (size > 1)
                        {
                            Console.Write("Выберите ориентацию (H - горизонтально, V - вертикально): ");
                            char orient = Console.ReadKey().KeyChar;
                            Console.WriteLine();

                            Orientation orientation = (char.ToUpper(orient) == 'H') ?
                                Orientation.Horizontal : Orientation.Vertical;

                            Ship ship = new Ship(size, new Coordinate(x, y), orientation);

                            if (IsValidShipPlacement(size, new Coordinate(x, y), orientation))
                            {
                                if (myBoard.PlaceShip(ship))
                                {
                                    ships.Add(ship);
                                    placed = true;
                                    Color.Green();
                                    Console.WriteLine("Корабль размещен успешно!");
                                    Color.ResetColor();
                                }
                            }
                            else
                            {
                                throw new InvalidOperationException("Неверное размещение корабля! Попробуйте снова.");
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
                                    Color.Green();
                                    Console.WriteLine("Корабль размещен успешно!");
                                    Color.ResetColor();
                                }
                            }
                            else
                            {
                                throw new InvalidOperationException("Неверное размещение корабля! Попробуйте снова.");
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        Color.Red();
                        Console.WriteLine($"Ошибка: {ex.Message}");
                        Color.ResetColor();
                    }
                }
            }
        }

        private bool AutomaticPlacement()
        {
            Random rand = new Random();
            const int MAX_ATTEMPTS = 5;
            const int MAX_SINGLE_PLACEMENT_TRIES = 100;

            for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++)
            {
                myBoard.ClearBoard();
                ships.Clear();
                bool success = true;

                int shipIndex = 0;
                foreach (int size in shipSizes)
                {
                    bool placed = false;
                    int placementAttempts = 0;
                    while (!placed && placementAttempts < MAX_SINGLE_PLACEMENT_TRIES)
                    {
                        int x = rand.Next(GameBoard.GetBoardSize());
                        int y = rand.Next(GameBoard.GetBoardSize());
                        Orientation orientation = (rand.Next(2) == 0) ?
                            Orientation.Horizontal : Orientation.Vertical;

                        ShipBase ship;
                        if (shipIndex == 0) // Первый корабль - специальный
                        {
                            ship = new SpecialShip(size, new Coordinate(x, y), orientation, 1);
                        }
                        else
                        {
                            ship = new Ship(size, new Coordinate(x, y), orientation);
                        }

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
                    shipIndex++;
                }

                if (success)
                {
                    Color.Green();
                    Console.WriteLine("Автоматическая расстановка завершена!");
                    Color.ResetColor();
                    myBoard.Display(true);
                    return true;
                }
            }

            Color.Red();
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