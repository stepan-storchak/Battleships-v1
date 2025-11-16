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

        public bool AllShipsSunk()
        {
            return ships.All(ship => ship.IsSunk());
        }

        public ShotResult GetShotResult(Coordinate coord)
        {
            ShotResult result = myBoard.ReceiveShot(coord);

            if (result == ShotResult.Hit)
            {
                foreach (var ship in ships)
                {
                    if (ship.Coordinates.Any(c => c.X == coord.X && c.Y == coord.Y))
                    {
                        bool wasHit = ship.TakeHit(coord);
                        if (wasHit && ship.IsSunk())
                        {
                            result = ShotResult.Sunk;
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
    }
}