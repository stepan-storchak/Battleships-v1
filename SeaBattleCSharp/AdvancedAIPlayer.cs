using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace SeaBattleCSharp
{
    public class AdvancedAIPlayer : AIPlayer
    {
        private List<Coordinate> priorityTargets;
        private bool isTrackingShip;
        private Coordinate firstHit;
        private List<Coordinate> shipDirectionCandidates;
        private Ship currentTargetShip;

        public AdvancedAIPlayer(string name = "Advanced Computer") : base(name)
        {
            priorityTargets = new List<Coordinate>();
            isTrackingShip = false;
            shipDirectionCandidates = new List<Coordinate>();
            currentTargetShip = null;
        }

        public override bool IsAdvancedAI() => true;

        public override void PlaceShips()
        {
            base.PlaceShips();
        }

        public override bool MakeMoveWithResult(Player enemy)
        {
            Console.WriteLine("\n=== Ход продвинутого ИИ ===");
            Color.SetColor(Color.YELLOW);
            Console.Write("Продвинутый ИИ анализирует поле...");
            Thread.Sleep(500);
            Console.WriteLine();
            Color.ResetColor();

            Coordinate target;
            if (priorityTargets.Count > 0)
            {
                target = priorityTargets[0];
                priorityTargets.RemoveAt(0);
            }
            else
            {
                target = CalculateOptimalShot();
            }

            Color.SetColor(Color.BLUE);
            Console.WriteLine($"Продвинутый ИИ стреляет в {target}");
            Color.ResetColor();

            ShotResult result = enemy.GetShotResult(target);

            if (result == ShotResult.Hit || result == ShotResult.Sunk)
            {
                enemyBoard.SetCellState(target, CellState.Hit);

                if (!isTrackingShip)
                {
                    isTrackingShip = true;
                    firstHit = target;
                    FindAndSetCurrentShip(enemy, target);

                    if (currentTargetShip != null)
                    {
                        GenerateShipCompletionTargets();
                    }
                }
                else if (currentTargetShip != null)
                {
                    GenerateShipCompletionTargets();
                }

                if (result == ShotResult.Sunk)
                {
                    Color.SetColor(Color.RED);
                    Console.WriteLine("Уничтожен корабль!");
                    Color.ResetColor();
                    MarkAreaAroundDestroyedShip(enemy, target);
                    ClearTrackingData();
                    return true;
                }
                else
                {
                    Color.SetColor(Color.YELLOW);
                    Console.WriteLine("Попадание!");
                    Color.ResetColor();
                    return true;
                }
            }
            else
            {
                enemyBoard.SetCellState(target, CellState.Miss);
                Color.SetColor(Color.BLUE);
                Console.WriteLine("Промах!");
                Color.ResetColor();

                if (isTrackingShip && currentTargetShip != null)
                {
                    GenerateShipCompletionTargets();
                }
                return false;
            }
        }

        public override void MarkAreaAroundDestroyedShip(Player enemy, Coordinate hitCoord)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    Coordinate around = new Coordinate(hitCoord.X + dx, hitCoord.Y + dy);
                    if (around.X >= 0 && around.X < 10 && around.Y >= 0 && around.Y < 10)
                    {
                        if (enemyBoard.GetCellState(around) == CellState.Empty)
                        {
                            enemyBoard.SetCellState(around, CellState.Miss);
                        }
                    }
                }
            }
            priorityTargets.Clear();
        }

        public override string GetPlayerType() => "Advanced AI Player";

        private Coordinate CalculateOptimalShot()
        {
            Coordinate target;
            Random rand = new Random();

            do
            {
                target = new Coordinate(rand.Next(10), rand.Next(10));
            } while (enemyBoard.GetCellState(target) != CellState.Empty);

            return target;
        }

        private void FindAndSetCurrentShip(Player enemy, Coordinate hitCoord)
        {
            List<Ship> enemyShips = enemy.GetShips();

            foreach (Ship ship in enemyShips)
            {
                foreach (Coordinate coord in ship.Coordinates)
                {
                    if (coord.Equals(hitCoord) && !ship.IsSunk())
                    {
                        currentTargetShip = ship;
                        return;
                    }
                }
            }
            currentTargetShip = null;
        }

        private void GenerateShipCompletionTargets()
        {
            if (currentTargetShip == null || currentTargetShip.IsSunk())
                return;

            foreach (Coordinate coord in currentTargetShip.Coordinates)
            {
                if (enemyBoard.GetCellState(coord) == CellState.Empty)
                {
                    bool alreadyInList = priorityTargets.Any(existing => existing.Equals(coord));
                    if (!alreadyInList)
                    {
                        priorityTargets.Add(coord);
                    }
                }
            }
        }

        private void ClearTrackingData()
        {
            isTrackingShip = false;
            currentTargetShip = null;
            priorityTargets.Clear();
        }

        private bool IsValidTarget(Coordinate coord)
        {
            return coord.X >= 0 && coord.X < 10 &&
                   coord.Y >= 0 && coord.Y < 10 &&
                   enemyBoard.GetCellState(coord) == CellState.Empty;
        }
    }
}