using System;
using System.Collections.Generic;
using System.Linq;

namespace SeaBattleCSharp
{
    public class AIPlayer : Player
    {
        private Random random;

        public AIPlayer(string name = "Computer") : base(name)
        {
            random = new Random();
        }

        public override void PlaceShips()
        {
            int[] shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

            foreach (int size in shipSizes)
            {
                bool placed = false;
                while (!placed)
                {
                    int x = random.Next(GameBoard.GetBoardSize());
                    int y = random.Next(GameBoard.GetBoardSize());
                    Orientation orientation = random.Next(2) == 0 ?
                        Orientation.Horizontal : Orientation.Vertical;

                    Ship ship = new Ship(size, new Coordinate(x, y), orientation);
                    if (myBoard.PlaceShip(ship))
                    {
                        ships.Add(ship);
                        placed = true;
                    }
                }
            }
        }

        public override void MakeMove(Player enemy)
        {
            Console.WriteLine($"\nХод компьютера {name}");

            Coordinate target;
            do
            {
                target = new Coordinate(
                    random.Next(GameBoard.GetBoardSize()),
                    random.Next(GameBoard.GetBoardSize())
                );
            } while (enemyBoard.GetCellState(target) != CellState.Empty);

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
    }
}