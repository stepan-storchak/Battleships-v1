
namespace SeaBattleCSharp
{
    public class AIPlayer : Player
    {
        protected Coordinate lastHit;
        protected bool huntMode;
        protected List<Coordinate> possibleTargets;

        private static Random random = new Random();
        private readonly int[] shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
        private const int BOARD_SIZE = 10;
        private const int MAX_ATTEMPTS = 10;
        private const int MAX_SINGLE_PLACEMENT_TRIES = 100;
        private const int THINKING_DOTS = 3;
        private const int THINKING_DELAY_MS = 500;
        private static int attempts = 0;

        public AIPlayer(string name = "Computer") : base(name)
        {
            huntMode = false;
            possibleTargets = new List<Coordinate>();
        }

        public override bool IsHuman() => false;

        public virtual AIPlayer Copy()
        {
            var copy = new AIPlayer(name);
            copy.myBoard = this.myBoard;
            copy.enemyBoard = this.enemyBoard;
            copy.ships = new List<Ship>(this.ships);
            copy.lastHit = this.lastHit;
            copy.huntMode = this.huntMode;
            copy.possibleTargets = new List<Coordinate>(this.possibleTargets);
            return copy;
        }

        public override void PlaceShips()
        {
            attempts = 0;
            Random rand = new Random();

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
                        int x = rand.Next(BOARD_SIZE);
                        int y = rand.Next(BOARD_SIZE);
                        Orientation orientation = (rand.Next(2) == 0) ?
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
                    return;

                attempts++;
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
            Color.SetColor(Color.YELLOW);
            Console.Write("Компьютер думает");

            for (int i = 0; i < THINKING_DOTS; i++)
            {
                Console.Write(".");
                Console.Out.Flush();
                Thread.Sleep(THINKING_DELAY_MS);
            }
            Console.WriteLine();
            Color.ResetColor();

            Coordinate target = GenerateSmartMove();
            Color.SetColor(Color.BLUE);
            Console.WriteLine($"Компьютер стреляет в {target}");
            Color.ResetColor();

            ShotResult result = enemy.GetShotResult(target);
            UpdateStrategy(result, target);
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
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    Coordinate around = new Coordinate(hitCoord.X + dx, hitCoord.Y + dy);
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

        protected Coordinate GenerateSmartMove()
        {
            if (possibleTargets.Count > 0)
            {
                Coordinate target = possibleTargets[0];
                possibleTargets.RemoveAt(0);
                return target;
            }

            Coordinate targetCoord;
            do
            {
                targetCoord = new Coordinate(random.Next(BOARD_SIZE), random.Next(BOARD_SIZE));
            } while (enemyBoard.GetCellState(targetCoord) != CellState.Empty);

            return targetCoord;
        }

        protected void UpdateStrategy(ShotResult result, Coordinate coord)
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

                for (int dy = -1; dy <= 1; dy++)
                {
                    for (int dx = -1; dx <= 1; dx++)
                    {
                        Coordinate around = new Coordinate(coord.X + dx, coord.Y + dy);
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
        }

        protected void GeneratePossibleTargets(Coordinate hitCoord)
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
                if (newTarget.X >= 0 && newTarget.X < BOARD_SIZE &&
                    newTarget.Y >= 0 && newTarget.Y < BOARD_SIZE)
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