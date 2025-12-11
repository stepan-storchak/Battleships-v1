
namespace SeaBattleCSharp
{
    

    public class Game
    {
        private Player player1;
        private Player player2;
        private Player currentPlayer;
        private GameState gameState;
        private Leaderboard leaderboard;
        private string winnerName;

        public Game()
        {
            gameState = GameState.Menu;
            leaderboard = new Leaderboard();
        }

        private Player GetOpponent()
        {
            return (currentPlayer == player1) ? player2 : player1;
        }

        public void Run()
        {
            try
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
                                bool wasHit = currentPlayer.MakeMoveWithResult(opponent);

                                if (!wasHit)
                                {
                                    SwitchTurn();
                                }
                                else
                                {
                                    Console.WriteLine($"Попадание! {currentPlayer.GetName()} продолжает ход.");
                                }
                            }
                            break;
                        case GameState.AfterGame:
                            ShowAfterGameMenu();
                            break;
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"Критическая ошибка в игровом цикле: {e.Message}");
            }
        }

        private void ShowMainMenu()
        {
            Color.SetColor(Color.GREEN);
            Console.WriteLine("\n=== МОРСКОЙ БОЙ ===");
            Color.ResetColor();

            Console.WriteLine("1. Начать новую игру");
            Console.WriteLine("2. Показать таблицу лидеров");
            Console.WriteLine("3. Выход");
            Console.Write("Выберите опцию: ");

            if (int.TryParse(Console.ReadLine(), out int choice))
            {
                ProcessMenuInput(choice);
            }
        }

        private void ShowAfterGameMenu()
        {
            Console.WriteLine("\n");
            Color.SetColor(Color.GREEN);
            Console.WriteLine("=== ИГРА ЗАВЕРШЕНА ===");
            Color.ResetColor();

            Color.SetColor(Color.YELLOW);
            Console.WriteLine($"Победитель: {winnerName}!");
            Color.ResetColor();

            Console.WriteLine("\nВыберите действие:");
            Color.SetColor(Color.GREEN);
            Console.WriteLine("1. Сохранить результат и выйти в меню");
            Color.SetColor(Color.BLUE);
            Console.WriteLine("2. Продолжить играть (новая игра)");
            Color.SetColor(Color.RED);
            Console.WriteLine("3. Выйти из игры");
            Color.ResetColor();

            Console.Write("Ваш выбор: ");
            if (int.TryParse(Console.ReadLine(), out int choice))
            {
                ProcessAfterGameInput(choice);
            }
        }

        private void ProcessAfterGameInput(int choice)
        {
            switch (choice)
            {
                case 1:
                    leaderboard.AddWin(winnerName);
                    Color.SetColor(Color.GREEN);
                    Console.WriteLine("Результат сохранен в таблице лидеров!");
                    Color.ResetColor();

                    player1 = null;
                    player2 = null;
                    currentPlayer = null;
                    winnerName = "";
                    gameState = GameState.Menu;
                    break;

                case 2:
                    Color.SetColor(Color.BLUE);
                    Console.WriteLine("Начинаем новую игру!");
                    Color.ResetColor();
                    StartNewGame();
                    break;

                case 3:
                    gameState = GameState.GameOver;
                    Color.SetColor(Color.RED);
                    Console.WriteLine("Выход из игры...");
                    Color.ResetColor();
                    break;

                default:
                    Color.SetColor(Color.RED);
                    Console.WriteLine("Неверный выбор! Попробуйте снова.");
                    Color.ResetColor();
                    break;
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
                Console.WriteLine("\n");
                Color.SetColor(Color.GREEN);
                Console.WriteLine("=== ИГРА ОКОНЧЕНА ===");
                Color.ResetColor();

                Color.SetColor(Color.RED);
                Console.WriteLine("Победил компьютер!");
                Color.ResetColor();

                winnerName = "Computer";

                Console.WriteLine("\nФинальное состояние полей:");
                Color.SetColor(Color.GREEN);
                Console.WriteLine("Ваше поле:");
                Color.ResetColor();
                player1.GetMyBoard().Display(true);

                Color.SetColor(Color.BLUE);
                Console.WriteLine("\nПоле противника:");
                Color.ResetColor();
                player1.GetEnemyBoard().Display(false);
                return true;
            }
            else if (player2.AllShipsSunk())
            {
                Console.WriteLine("\n");
                Color.SetColor(Color.GREEN);
                Console.WriteLine("=== ИГРА ОКОНЧЕНA ===");
                Color.ResetColor();

                Color.SetColor(Color.GREEN);
                Console.WriteLine($"Победил {player1.GetName()}!");
                Color.ResetColor();

                winnerName = player1.GetName();

                Console.WriteLine("\nФинальное состояние полей:");
                Color.SetColor(Color.GREEN);
                Console.WriteLine("Ваше поле:");
                Color.ResetColor();
                player1.GetMyBoard().Display(true);

                Color.SetColor(Color.BLUE);
                Console.WriteLine("\nПоле противника:");
                Color.ResetColor();
                player1.GetEnemyBoard().Display(false);
                return true;
            }
            return false;
        }

        private void ProcessMenuInput(int choice)
        {
            switch (choice)
            {
                case 1:
                    StartNewGame();
                    break;
                case 2:
                    ShowLeaderboard();
                    break;
                case 3:
                    gameState = GameState.GameOver;
                    Console.WriteLine("Выход из игры...");
                    break;
                default:
                    Color.SetColor(Color.RED);
                    Console.WriteLine("Неверный выбор! Пожалуйста, введите 1, 2 или 3.");
                    Color.ResetColor();
                    break;
            }
        }

        private void StartNewGame()
        {
            try
            {
                string playerName;
                while (true)
                {
                    Console.Write("Введите ваше имя: ");
                    playerName = Console.ReadLine()?.Trim();

                    if (!string.IsNullOrWhiteSpace(playerName))
                        break;

                    Color.SetColor(Color.RED);
                    Console.WriteLine("Имя не может быть пустым!");
                    Color.ResetColor();
                }

                int gameType = 0;
                while (gameType < 1 || gameType > 2)
                {
                    Console.WriteLine("Выберите тип игры:");
                    Console.WriteLine("1. Против обычного ИИ");
                    Console.WriteLine("2. Против продвинутого ИИ");
                    Console.Write("Ваш выбор: ");

                    string input = Console.ReadLine();
                    if (!int.TryParse(input, out gameType) || (gameType != 1 && gameType != 2))
                    {
                        Color.SetColor(Color.RED);
                        Console.WriteLine("Введите 1 или 2!");
                        Color.ResetColor();
                        gameType = 0; 
                    }
                }

                player1 = new HumanPlayer(playerName);

                if (gameType == 2)
                    player2 = new AdvancedAIPlayer("Advanced AI");
                else
                    player2 = new AIPlayer();

                currentPlayer = player1;
                gameState = GameState.Placement;
                winnerName = "";

                Color.SetColor(Color.GREEN);
                Console.WriteLine($"\nНовая игра началась! Удачи, {playerName}!");
                Color.ResetColor();
            }
            catch (Exception e)
            {
                Console.WriteLine($"Ошибка при создании новой игры: {e.Message}");
                gameState = GameState.Menu;
            }
        }

        private void ShowLeaderboard()
        {
            leaderboard.Display();
            Console.WriteLine("\nНажмите Enter для продолжения...");
            Console.ReadLine();
        }
    }
}