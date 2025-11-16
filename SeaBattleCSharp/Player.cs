using System;
using System.Collections.Generic;
using System.Linq;

namespace SeaBattleCSharp
{
    public abstract class Player
    {
        protected string name;
        protected GameBoard myBoard;
        protected GameBoard enemyBoard;
        protected List<Ship> ships;

        public Player(string name)
        {
            this.name = name;
            myBoard = new GameBoard();
            enemyBoard = new GameBoard();
            ships = new List<Ship>();
        }

        public abstract void PlaceShips();
        public abstract void MakeMove(Player enemy);
        public abstract bool MakeMoveWithResult(Player enemy);
        public abstract void MarkAreaAroundDestroyedShip(Player enemy, Coordinate hitCoord);

        public bool AllShipsSunk()
        {
            foreach (var ship in ships)
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
                foreach (var ship in ships)
                {
                    // Проверяем, принадлежит ли координата этому кораблю
                    if (ship.Coordinates.Any(c => c.X == coord.X && c.Y == coord.Y))
                    {
                        // Попадание в корабль
                        bool wasHit = ship.TakeHit(coord);

                        // Проверяем, был ли корабль уничтожен этим попаданием
                        if (wasHit && ship.IsSunk())
                        {
                            result = ShotResult.Sunk;
                            // Помечаем область вокруг уничтоженного корабля на СВОЕМ поле
                            myBoard.MarkAreaAroundSunkShip(ship);
                        }
                        break;
                    }
                }
            }

            return result;
        }

        public void AddShip(Ship ship)
        {
            ships.Add(ship);
        }

        public string GetName() => name;
        public GameBoard GetMyBoard() => myBoard;
        public GameBoard GetEnemyBoard() => enemyBoard;
        public List<Ship> GetShips() => ships;

        // Метод для поиска корабля по координате
        protected Ship FindShipByCoordinate(Coordinate coord)
        {
            return ships.FirstOrDefault(ship => ship.Coordinates.Any(c => c.X == coord.X && c.Y == coord.Y));
        }
    }
}