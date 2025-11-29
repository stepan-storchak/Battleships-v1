using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace SeaBattleCSharp
{
	public class AdvancedAIPlayer : AIPlayer
	{
		private List<Coordinate> priorityTargets;

		public AdvancedAIPlayer(string name = "Advanced Computer") : base(name)
		{
			priorityTargets = new List<Coordinate>();
			InitializePriorityTargets();
		}

		private void InitializePriorityTargets()
		{
			for (int y = 0; y < GameBoard.GetBoardSize(); y += 2)
			{
				for (int x = 0; x < GameBoard.GetBoardSize(); x += 2)
				{
					priorityTargets.Add(new Coordinate(x, y));
				}
			}
		}

		public override void DisplayPlayerInfo()
		{
			base.DisplayPlayerInfo();
			Console.WriteLine("Уровень: Продвинутый");
			Console.WriteLine("Стратегия: Приоритетная стрельба по шахматной доске");
		}

		public override bool MakeMoveWithResult(Player enemy)
		{
			Console.WriteLine("\n=== Ход продвинутого компьютера ===");
			Color.Yellow();
			Console.Write("Продвинутый компьютер анализирует поле");

			for (int i = 0; i < 3; i++)
			{
				Console.Write(".");
				Console.Out.Flush();
				Thread.Sleep(600); // Немного дольше думает
			}
			Console.WriteLine();
			Color.ResetColor();

			Coordinate target = GetPriorityTarget();

			Color.Blue();
			Console.WriteLine($"Продвинутый компьютер стреляет в {(char)('A' + target.X)}{target.Y + 1}");
			Color.ResetColor();

			try
			{
				ShotResult result = enemy.GetShotResult(target);
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
						AddSurroundingTargets(target);
						break;
					case ShotResult.Sunk:
						enemyBoard.SetCellState(target, CellState.Hit);
						Color.Red();
						Console.WriteLine("Уничтожен корабль!");
						Color.ResetColor();
						wasHit = true;
						MarkAreaAroundDestroyedShip(enemy, target);
						break;
				}
				return wasHit;
			}
			catch (Exception ex)
			{
				Color.Red();
				Console.WriteLine($"Ошибка при выстреле: {ex.Message}");
				Color.ResetColor();
				return false;
			}
		}

		private Coordinate GetPriorityTarget()
		{
			// Сначала проверяем приоритетные цели
			foreach (var target in priorityTargets.ToList())
			{
				if (enemyBoard.GetCellState(target) == CellState.Empty)
				{
					priorityTargets.Remove(target);
					return target;
				}
			}

			// Если приоритетные цели закончились, используем обычную логику
			return GenerateRandomTarget();
		}

		private Coordinate GenerateRandomTarget()
		{
			Random rand = new Random();
			Coordinate target;
			do
			{
				target = new Coordinate(
					rand.Next(GameBoard.GetBoardSize()),
					rand.Next(GameBoard.GetBoardSize())
				);
			} while (enemyBoard.GetCellState(target) != CellState.Empty);
			return target;
		}

		private void AddSurroundingTargets(Coordinate hitCoord)
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
						// Добавляем в начало списка для приоритета
						priorityTargets.Insert(0, newTarget);
					}
				}
			}
		}
	}
}