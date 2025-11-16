using System;
using System.Collections.Generic;

namespace SeaBattleCSharp
{
    public class HumanPlayer : Player
    {
        private static readonly int[] shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

        public HumanPlayer(string name) : base(name) { }

        public override void PlaceShips()
        {
            Console.WriteLine($"Расстановка кораблей для {name}");

            foreach (int size in shipSizes)
            {
                bool placed = false;
                while (!placed)
                {
                    Console.WriteLine($"Размещение корабля размером {size}");
                    myBoard.Display(true);

                    Console.Write("Введите координату (A1): ");
                    string input = Console.ReadLine()?.ToUpper();

                    if (!string.IsNullOrEmpty(input) && input.Length >= 2)
                    {
                        char letter = input[0];
                        if (int.TryParse(input.Substring(1), out int number))
                        {
                            int x = letter - 'A';
                            int y = number - 1;

                            Orientation orientation = Orientation.Horizontal;
                            if (size > 1)
                            {
                                Console.Write("Ориентация (H - горизонтально, V - вертикально): ");
                                char orient = Console.ReadKey().KeyChar;
                                Console.WriteLine();
                                orientation = char.ToUpper(orient) == 'H' ?
                                    Orientation.Horizontal : Orientation.Vertical;
                            }

                            Ship ship = new Ship(size, new Coordinate(x, y), orientation);
                            if (myBoard.PlaceShip(ship))
                            {
                                ships.Add(ship);
                                placed = true;
                                Console.WriteLine("Корабль размещен!");
                            }
                        }
                    }
                }
            }
        }

        public override void MakeMove(Player enemy)
        {
            Console.WriteLine($"\nХод игрока {name}");
            myBoard.Display(true);
            enemy.GetEnemyBoard().Display(false);

            Coordinate target = InputCoordinate();
            ShotResult result = enemy.GetShotResult(target);

            switch (result)
            {
                case ShotResult.Miss:
                    enemyBoard.SetCellState(target, CellState.Miss);
                    Console.WriteLine("Промах!");
                    break;
                case ShotResult.Hit:
                    enemyBoard.SetCellState(target, CellState.Hit);
                    Console.WriteLine("Попадание!");
                    break;
                case ShotResult.Sunk:
                    enemyBoard.SetCellState(target, CellState.Hit);
                    Console.WriteLine("Уничтожен корабль!");
                    break;
            }
        }

        private Coordinate InputCoordinate()
        {
            while (true)
            {
                Console.Write("Введите координаты (A1): ");
                string input = Console.ReadLine()?.ToUpper();

                if (!string.IsNullOrEmpty(input) && input.Length >= 2)
                {
                    char letter = input[0];
                    if (int.TryParse(input.Substring(1), out int number))
                    {
                        int x = letter - 'A';
                        int y = number - 1;

                        if (x >= 0 && x < GameBoard.GetBoardSize() &&
                            y >= 0 && y < GameBoard.GetBoardSize())
                        {
                            return new Coordinate(x, y);
                        }
                    }
                }
                Console.WriteLine("Неверные координаты!");
            }
        }
    }
}