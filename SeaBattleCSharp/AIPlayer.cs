using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace SeaBattleCSharp
{
    public class AIPlayer : Player
    {
        private Coordinate lastHit;
        private bool huntMode;
        private List<Coordinate> possibleTargets;
        private Random random;

        public AIPlayer(string name = "Computer") : base(name)
        {
            huntMode = false;
            possibleTargets = new List<Coordinate>();
            random = new Random();
        }

        public override void PlaceShips()
        {
            int[] shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
            const int MAX_ATTEMPTS = 10;

            for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++)
            {
                myBoard.ClearBoard();
                ships.Clear();
                bool success = true;

                foreach (int size in shipSizes)
                {
                    bool placed = false;
                    int placementAttempts = 0;

                    while (!placed && placementAttempts < 100)
                    {
                        int x = random.Next(GameBoard.GetBoardSize());
                        int y = random.Next(GameBoard.GetBoardSize());
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

                if (success) return;
            }
        }

        public override void MakeMove(Player enemy)
        {
            Console.WriteLine("\n=== Ход компьютера ===");

            Color.Yellow();
            Console.Write("Компьютер думает...");
            Thread.Sleep(1000);
            Console.WriteLine();
            Color.ResetColor();

            Coordinate target;
            if (huntMode && possibleTargets.Count > 0)
            {
                target = possibleTargets[0];
                possibleTargets.RemoveAt(0);
            }
            else
            {
                do
                {
                    target = new Coordinate(
                        random.Next(GameBoard.GetBoardSize()),
                        random.Next(GameBoard.GetBoardSize())
                    );
                } while (enemyBoard.GetCellState(target) != CellState.Empty);
            }

            Color.Blue();
            Console.WriteLine($"Компьютер стреляет в {(char)('A' + target.X)}{target.Y + 1}");
            Color.ResetColor();

            ShotResult result = enemy.GetShotResult(target);

            // Логика ИИ
            if (result == ShotResult.Hit || result == ShotResult.Sunk)
            {
                huntMode = true;
                lastHit = target;
                GeneratePossibleTargets(target);
            }

            if (result == ShotResult.Sunk)
            {
                huntMode = false;
                possibleTargets.Clear();
            }

            switch (result)
            {
                case ShotResult.Miss:
                    enemyBoard.SetCellState(target, CellState.Miss);
                    Color.Blue();
                    Console.WriteLine("Промах!");
                    Color.ResetColor();
                    break;
                case ShotResult.Hit:
                    enemyBoard.SetCellState(target, CellState.Hit);
                    Color.Yellow();
                    Console.WriteLine("Попадание!");
                    Color.ResetColor();
                    break;
                case ShotResult.Sunk:
                    enemyBoard.SetCellState(target, CellState.Hit);
                    Color.Red();
                    Console.WriteLine("Уничтожен корабль!");
                    Color.ResetColor();
                    break;
            }
        }

        private void GeneratePossibleTargets(Coordinate hitCoord)
        {
            List<Coordinate> directions = new List<Coordinate>
            {
                new Coordinate(1, 0), new Coordinate(-1, 0),
                new Coordinate(0, 1), new Coordinate(0, -1)
            };

            foreach (var dir in directions)
            {
                Coordinate newTarget = new Coordinate(hitCoord.X + dir.X, hitCoord.Y + dir.Y);
                if (newTarget.X >= 0 && newTarget.X < GameBoard.GetBoardSize() &&
                    newTarget.Y >= 0 && newTarget.Y < GameBoard.GetBoardSize())
                {
                    if (enemyBoard.GetCellState(newTarget) == CellState.Empty)
                    {
                        possibleTargets.Add(newTarget);
                    }
                }
            }
        }
    }
}