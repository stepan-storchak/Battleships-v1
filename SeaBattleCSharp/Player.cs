using System;
using System.Collections.Generic;
using System.Linq;

namespace SeaBattleCSharp
{
    public abstract class Player : IPlayer, ICloneableEntity
    {
        protected string name;
        protected GameBoard myBoard;
        protected GameBoard enemyBoard;
        protected Container<ShipBase> ships;

        public Player(string name)
        {
            this.name = name;
            myBoard = new GameBoard();
            enemyBoard = new GameBoard();
            ships = new Container<ShipBase>();
        }

        public abstract void PlaceShips();
        public abstract void MakeMove(Player enemy);
        public abstract bool MakeMoveWithResult(Player enemy);
        public abstract void MarkAreaAroundDestroyedShip(Player enemy, Coordinate hitCoord);

        public virtual void DisplayPlayerInfo()
        {
            Console.WriteLine($"Игрок: {name}");
        }

        public virtual void DisplayPlayerInfo(bool showShips)
        {
            Console.WriteLine($"Игрок: {name}");
            if (showShips)
            {
                myBoard.Display(true);
            }
        }

        public bool AllShipsSunk()
        {
            foreach (var ship in ships.ToList())
            {
                if (!ship.IsSunk())
                {
                    return false;
                }
            }
            return true;
        }

        public ShotResult GetShotResult(Coordinate coord)
        {
            ShotResult result = myBoard.ReceiveShot(coord);
            if (result == ShotResult.Hit)
            {
                foreach (var ship in ships.ToList())
                {
                    if (ship.Coordinates.Any(c => c.X == coord.X && c.Y == coord.Y))
                    {
                        bool wasHit = ship.TakeHit(coord);
                        if (wasHit && ship.IsSunk())
                        {
                            result = ShotResult.Sunk;
                            myBoard.MarkAreaAroundSunkShip(ship);
                        }
                        break;
                    }
                }
            }
            return result;
        }

        public void AddShip(ShipBase ship)
        {
            ships.Add(ship);
        }

        public string GetName() => name;
        public GameBoard GetMyBoard() => myBoard;
        public GameBoard GetEnemyBoard() => enemyBoard;
        public List<ShipBase> GetShips() => ships.ToList();

        protected ShipBase FindShipByCoordinate(Coordinate coord)
        {
            return ships.Find(ship => ship.Coordinates.Any(c => c.X == coord.X && c.Y == coord.Y));
        }

        public void DisplayShipStatistics()
        {
            Console.WriteLine($"\nСтатистика кораблей игрока {name}:");
            Console.WriteLine($"Всего кораблей: {ships.Count}");

            if (ships.Count > 0)
            {
                ships.SortBySize();

                var aliveShips = ships.FindAll(s => !s.IsSunk());
                var destroyedShips = ships.FindAll(s => s.IsSunk());

                Console.WriteLine($"Живых кораблей: {aliveShips.Count}");
                Console.WriteLine($"Уничтоженных кораблей: {destroyedShips.Count}");

                var minSizeShip = ships.Min(s => s.Size);
                var maxSizeShip = ships.Max(s => s.Size);
                var averageSize = ships.Average(s => s.Size);

                Console.WriteLine($"Минимальный размер корабля: {minSizeShip}");
                Console.WriteLine($"Максимальный размер корабля: {maxSizeShip}");
                Console.WriteLine($"Средний размер корабля: {averageSize:F1}");

                var shipTypes = ships.Select(s => s.GetShipType()).Distinct();
                Console.WriteLine($"Типы кораблей: {string.Join(", ", shipTypes)}");

                var shipDictionary = ships.ToDictionary();
                Console.WriteLine($"Корабли в словаре: {shipDictionary.Count}");
            }
        }

        public virtual object Clone()
        {
            var cloned = (Player)this.MemberwiseClone();
            cloned.myBoard = (GameBoard)myBoard.Clone();
            cloned.enemyBoard = (GameBoard)enemyBoard.Clone();
            cloned.ships = new Container<ShipBase>();
            foreach (var ship in ships.ToList())
            {
                cloned.ships.Add((ShipBase)ship.Clone());
            }
            return cloned;
        }

        public virtual object DeepClone()
        {
            var cloned = (Player)this.MemberwiseClone();
            cloned.myBoard = (GameBoard)myBoard.DeepClone();
            cloned.enemyBoard = (GameBoard)enemyBoard.DeepClone();
            cloned.ships = new Container<ShipBase>();
            foreach (var ship in ships.ToList())
            {
                cloned.ships.Add((ShipBase)ship.DeepClone());
            }
            return cloned;
        }
    }
}