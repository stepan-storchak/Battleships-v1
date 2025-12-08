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
        protected Dictionary<string, int> statistics;

        public static int PlayerCount { get; private set; }

        public Player(string name)
        {
            this.name = name;
            myBoard = new GameBoard();
            enemyBoard = new GameBoard();
            ships = new List<Ship>();
            statistics = new Dictionary<string, int>
            {
                ["hits"] = 0,
                ["misses"] = 0,
                ["ships_destroyed"] = 0
            };
            PlayerCount++;
        }

        public abstract bool IsHuman();
        public virtual bool IsAdvancedAI() => false;
        public abstract bool MakeMoveWithResult(Player enemy);
        public abstract void PlaceShips();
        public abstract void MakeMove(Player enemy);
        public abstract void MarkAreaAroundDestroyedShip(Player enemy, Coordinate hitCoord);
        public virtual string GetPlayerType() => "Base Player";

        public bool AllShipsSunk()
        {
            return ships.All(ship => ship.IsSunk());
        }

        public ShotResult GetShotResult(Coordinate coord)
        {
            ShotResult result = myBoard.ReceiveShot(coord);

            if (result == ShotResult.Hit || result == ShotResult.Sunk)
            {
                var ship = ships.FirstOrDefault(s =>
                    s.Coordinates.Any(c => c.Equals(coord)));

                if (ship != null)
                {
                    bool wasSunk = ship.TakeHit(coord);
                    if (wasSunk)
                    {
                        result = ShotResult.Sunk;
                        myBoard.MarkAreaAroundSunkShip(ship);
                        statistics["ships_destroyed"]++;
                    }
                }
            }
            return result;
        }

        public string GetName() => name;
        public GameBoard GetMyBoard() => myBoard;
        public GameBoard GetEnemyBoard() => enemyBoard;
        public List<Ship> GetShips() => ships;

        public void AddShip(Ship ship)
        {
            ships.Add(ship);
        }

        public void UpdateStatistics(string key, int value)
        {
            if (statistics.ContainsKey(key))
                statistics[key] += value;
        }
    }
}