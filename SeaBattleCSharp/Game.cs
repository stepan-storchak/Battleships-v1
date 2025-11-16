using System;

namespace SeaBattleCSharp
{
	public class Game
	{
		private Player player1;
		private Player player2;
		private Player currentPlayer;
		private GameState gameState;

		public Game()
		{
			player1 = null;
			player2 = null;
			currentPlayer = null;
			gameState = GameState.Menu;
		}

		public void Run()
		{
			while (gameState != GameState.GameOver)
			{
				switch (gameState)
				{
					case GameState.Menu:
						ShowMainMenu();
						break;
					case GameState.Placement:
						player1.PlaceShips();
						player2.PlaceShips();
						gameState = GameState.Battle;
						currentPlayer = player1;
						break;
					case GameState.Battle:
						if (CheckWinCondition())
						{
							gameState = GameState.AfterGame;
						}
						else
						{
							Player opponent = GetOpponent();
							currentPlayer.MakeMove(opponent);
							SwitchTurn();
						}
						break;
					case GameState.AfterGame:
						ShowAfterGameMenu();
						break;
				}
			}
		}

		private Player GetOpponent()
		{
			return (currentPlayer == player1) ? player2 : player1;
		}

		private void ShowMainMenu()
		{
			Console.WriteLine("\n=== МОРСКОЙ БОЙ ===");
			Console.WriteLine("1. Начать новую игру");
			Console.WriteLine("2. Выход");
			Console.Write("Выберите опцию: ");

			if (int.TryParse(Console.ReadLine(), out int choice))
			{
				if (choice == 1)
				{
					StartNewGame();
				}
				else if (choice == 2)
				{
					gameState = GameState.GameOver;
				}
			}
		}

		private void ShowAfterGameMenu()
		{
			Console.WriteLine("\n=== ИГРА ЗАВЕРШЕНА ===");
			Console.WriteLine("1. Новая игра");
			Console.WriteLine("2. Выход");
			Console.Write("Выберите опцию: ");

			if (int.TryParse(Console.ReadLine(), out int choice))
			{
				if (choice == 1)
				{
					StartNewGame();
				}
				else
				{
					gameState = GameState.GameOver;
				}
			}
		}

		private void SwitchTurn()
		{
			currentPlayer = (currentPlayer == player1) ? player2 : player1;
		}

		private bool CheckWinCondition()
		{
			if (player1.AllShipsSunk())
			{
				Console.WriteLine("\nПобедил компьютер!");
				return true;
			}
			else if (player2.AllShipsSunk())
			{
				Console.WriteLine($"\nПобедил {player1.GetName()}!");
				return true;
			}
			return false;
		}

		private void StartNewGame()
		{
			Console.Write("Введите ваше имя: ");
			string playerName = Console.ReadLine();

			player1 = new HumanPlayer(playerName);
			player2 = new AIPlayer();
			gameState = GameState.Placement;

			Console.WriteLine($"\nНовая игра началась! Удачи, {playerName}!");
		}
	}
}