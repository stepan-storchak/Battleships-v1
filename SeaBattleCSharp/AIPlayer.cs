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
            const int MAX_SINGLE_PLACEMENT_TRIES = 100;

            for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++)
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

                if (success)
                {
                    return;
                }
            }

            Console.WriteLine("Компьютер не смог расставить корабли оптимально.");
        }

        public override void MakeMove(Player enemy)
        {
            MakeMoveWithResult(enemy);
        }

        public override bool MakeMoveWithResult(Player enemy)
        {
            Console.WriteLine("\n=== Ход компьютера ===");

            Color.Yellow();
            Console.Write("Компьютер думает");
            for (int i = 0; i < 3; i++)
            {
                Console.Write(".");
                Console.Out.Flush();
                Thread.Sleep(500);
            }
            Console.WriteLine();
            Color.ResetColor();

            Coordinate target;
            if (huntMode && possibleTargets.Count > 0)
            {
                target = GenerateSmartMove();
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
            UpdateStrategy(result, target);
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
                    // Закрашиваем область вокруг уничтоженного корабля на поле противника
                    MarkAreaAroundDestroyedShip(enemy, target);
                    break;
            }

            return wasHit;
        }

        public override void MarkAreaAroundDestroyedShip(Player enemy, Coordinate hitCoord)
        {
            // Находим уничтоженный корабль противника
            var enemyShips = enemy.GetShips();
            Ship destroyedShip = null;

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
                // Закрашиваем область вокруг всего уничтоженного корабля
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

        private Coordinate GenerateSmartMove()
        {
            if (possibleTargets.Count == 0)
            {
                Coordinate target;
                do
                {
                    target = new Coordinate(
                        random.Next(GameBoard.GetBoardSize()),
                        random.Next(GameBoard.GetBoardSize())
                    );
                } while (enemyBoard.GetCellState(target) != CellState.Empty);
                return target;
            }

            Coordinate smartTarget = possibleTargets[0];
            possibleTargets.RemoveAt(0);
            return smartTarget;
        }

        private void UpdateStrategy(ShotResult result, Coordinate coord)
        {
            if (result == ShotResult.Hit || result == ShotResult.Sunk)
            {
                huntMode = true;
                lastHit = coord;
                GeneratePossibleTargets(coord);
            }

            if (result == ShotResult.Sunk)
            {
                huntMode = false;
                possibleTargets.Clear();
            }
        }

        private void GeneratePossibleTargets(Coordinate hitCoord)
        {
            List<Coordinate> directions = new List<Coordinate>
            {
                new Coordinate(1, 0),
                new Coordinate(-1, 0),
                new Coordinate(0, 1),
                new Coordinate(0, -1)
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