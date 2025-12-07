using System;
using System.Linq;

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
            player1 = null;
            player2 = null;
            currentPlayer = null;
            gameState = GameState.Menu;
            leaderboard = new Leaderboard();
            winnerName = "";
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

                            player1.DisplayShipStatistics();
                            player2.DisplayShipStatistics();

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
                                bool wasHit = false;
                                if (currentPlayer is HumanPlayer human)
                                {
                                    wasHit = human.MakeMoveWithResult(opponent);
                                }
                                else if (currentPlayer is AIPlayer ai)
                                {
                                    wasHit = ai.MakeMoveWithResult(opponent);
                                }

                                if (!wasHit)
                                {
                                    SwitchTurn();
                                }
                                else
                                {
                                    Console.WriteLine($"Успех! {currentPlayer.GetName()} продолжает ход.");
                                }
                            }
                            break;
                        case GameState.AfterGame:
                            ShowAfterGameMenu();
                            break;
                    }
                }
            }
            catch (Exception ex)
            {
                Color.Red();
                Console.WriteLine($"Непредвиденная ошибка в игре: {ex.Message}");
                Color.ResetColor();
                Console.WriteLine("Нажмите Enter для продолжения...");
                Console.ReadLine();
            }
        }

        private void DemonstrateCollections()
        {
            Console.WriteLine("\n=== Демонстрация коллекций ===");

            Container<ShipBase> shipCollection = new Container<ShipBase>();

            shipCollection.Add(new Ship(4, new Coordinate(0, 0), Orientation.Horizontal));
            shipCollection.Add(new SpecialShip(3, new Coordinate(5, 5), Orientation.Vertical, 2));
            shipCollection.Add(new Ship(2, new Coordinate(2, 2), Orientation.Horizontal));
            shipCollection.Add(new SpecialShip(1, new Coordinate(8, 8), Orientation.Horizontal, 1));

            Console.WriteLine($"Всего кораблей в коллекции: {shipCollection.Count}");

            shipCollection.SortBySize();
            Console.WriteLine("Корабли отсортированы по размеру:");
            shipCollection.ForEach(s => Console.WriteLine($"- {s.GetShipType()}, размер: {s.Size}"));

            var largeShips = shipCollection.FindAll(s => s.Size >= 3);
            Console.WriteLine($"Больших кораблей (>=3): {largeShips.Count}");

            var shipTypes = shipCollection.Select(s => s.GetShipType()).Distinct();
            Console.WriteLine($"Уникальные типы кораблей: {string.Join(", ", shipTypes)}");

            var hasSpecialShips = shipCollection.Any(s => s is SpecialShip);
            Console.WriteLine($"Есть ли специальные корабли: {hasSpecialShips}");

            var shipArray = shipCollection.ToArray();
            Console.WriteLine($"Корабли в массиве: {shipArray.Length}");

            var shipDict = shipCollection.ToDictionary();
            Console.WriteLine($"Корабли в словаре: {shipDict.Count}");

            var minSize = shipCollection.Min(s => s.Size);
            var maxSize = shipCollection.Max(s => s.Size);
            var avgSize = shipCollection.Average(s => s.Size);

            Console.WriteLine($"Статистика размеров: мин={minSize}, макс={maxSize}, сред={avgSize:F1}");

            int totalSize = GenericHelper.CalculateTotalSize(shipCollection.ToList());
            Console.WriteLine($"Общий размер всех кораблей: {totalSize}");
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
                    DemonstrateOOPFeatures();
                    DemonstrateCollections();
                    DemonstrateGenericFunctions();
                    break;
                case 4:
                    gameState = GameState.GameOver;
                    Console.WriteLine("Выход...");
                    Color.ResetColor();
                    break;
                default:
                    throw new ArgumentOutOfRangeException("Выбранный пункт не существует");
            }
        }

        private void DemonstrateGenericFunctions()
        {
            Console.WriteLine("\n=== Демонстрация шаблонных функций ===");

            int[] numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            double[] doubles = { 1.5, 2.5, 3.5, 4.5, 5.5 };

            int maxNumber = GenericHelper.FindMax(numbers);
            int minNumber = GenericHelper.FindMin(numbers);

            double maxDouble = GenericHelper.FindMax(doubles);
            double minDouble = GenericHelper.FindMin(doubles);

            Console.WriteLine($"Максимальное число: {maxNumber}");
            Console.WriteLine($"Минимальное число: {minNumber}");
            Console.WriteLine($"Максимальное double: {maxDouble}");
            Console.WriteLine($"Минимальное double: {minDouble}");

            int totalNumbers = GenericHelper.CalculateTotal(numbers, n => n);
            Console.WriteLine($"Сумма чисел: {totalNumbers}");
        }

        private void ShowMainMenu()
        {
            Color.Green();
            Console.WriteLine("\n=== Морской бой ===");
            Color.ResetColor();
            Console.WriteLine("1. Начать новую игру");
            Console.WriteLine("2. Показать таблицу лидеров");
            Console.WriteLine("3. Демонстрация возможностей ООП и коллекций");
            Console.WriteLine("4. Выход");
            Console.Write("Выберите пункт: ");

            try
            {
                if (int.TryParse(Console.ReadLine(), out int choice))
                {
                    ProcessMenuInput(choice);
                }
                else
                {
                    throw new FormatException("Неверный формат ввода");
                }
            }
            catch (FormatException ex)
            {
                Color.Red();
                Console.WriteLine($"Ошибка ввода: {ex.Message}");
                Color.ResetColor();
            }
        }

        private void StartNewGame()
        {
            ResetGame();
            Console.Write("Введите ваше имя: ");
            string playerName = Console.ReadLine();

            if (string.IsNullOrWhiteSpace(playerName))
            {
                throw new ArgumentException("Имя не может быть пустым");
            }

            Console.WriteLine("\nВыберите тип противника:");
            Console.WriteLine("1 - Обычный компьютер");
            Console.WriteLine("2 - Продвинутый компьютер");
            Console.Write("Ваш выбор: ");

            if (int.TryParse(Console.ReadLine(), out int aiType))
            {
                if (aiType == 1)
                {
                    player2 = new AIPlayer();
                }
                else if (aiType == 2)
                {
                    player2 = new AdvancedAIPlayer();
                }
                else
                {
                    player2 = new AIPlayer();
                }
            }
            else
            {
                player2 = new AIPlayer();
            }

            player1 = new HumanPlayer(playerName);
            currentPlayer = player1;
            gameState = GameState.Placement;
            winnerName = "";

            Color.Green();
            Console.WriteLine($"\nИгра началась! Удачи, {playerName}!");
            Color.ResetColor();
        }

        private void ResetGame()
        {
            player1 = null;
            player2 = null;
            currentPlayer = null;
            winnerName = "";
        }

        private Player GetOpponent() => (currentPlayer == player1) ? player2 : player1;
        private void SwitchTurn() => currentPlayer = (currentPlayer == player1) ? player2 : player1;

        private bool CheckWinCondition()
        {
            if (player1.AllShipsSunk())
            {
                ShowGameOver(player2.GetName());
                winnerName = player2.GetName();
                return true;
            }
            else if (player2.AllShipsSunk())
            {
                ShowGameOver(player1.GetName());
                winnerName = player1.GetName();
                return true;
            }
            return false;
        }

        private void ShowGameOver(string winner)
        {
            Console.WriteLine();
            Color.Green();
            Console.WriteLine("=== Игра завершена ===");
            Color.ResetColor();

            if (winner == "Computer" || winner == "Advanced Computer")
            {
                Color.Red();
                Console.WriteLine("Компьютер победил!");
            }
            else
            {
                Color.Green();
                Console.WriteLine($"Победил {winner}!");
            }
            Color.ResetColor();

            Console.WriteLine("\nФинальное состояние доски:");
            Color.Green();
            Console.WriteLine("Ваша доска:");
            Color.ResetColor();
            player1.GetMyBoard().Display(true);

            Color.Blue();
            Console.WriteLine("\nДоска противника:");
            Color.ResetColor();
            player1.GetEnemyBoard().Display(false);
        }

        private void ShowAfterGameMenu()
        {
            Console.WriteLine();
            Color.Green();
            Console.WriteLine("=== Игра завершена ===");
            Color.ResetColor();
            Color.Yellow();
            Console.WriteLine($"Победитель: {winnerName}!");
            Color.ResetColor();

            Console.WriteLine("\nДоступные действия:");
            Color.Green();
            Console.WriteLine("1. Сохранить результат в таблицу лидеров");
            Color.Blue();
            Console.WriteLine("2. Начать новую игру (тот же режим)");
            Color.Red();
            Console.WriteLine("3. Выход в меню");
            Color.ResetColor();
            Console.Write("Ваш выбор: ");

            try
            {
                if (int.TryParse(Console.ReadLine(), out int choice))
                {
                    ProcessAfterGameInput(choice);
                }
                else
                {
                    throw new FormatException("Неверный формат ввода");
                }
            }
            catch (FormatException ex)
            {
                Color.Red();
                Console.WriteLine($"Ошибка ввода: {ex.Message}");
                Color.ResetColor();
            }
        }

        private void ProcessAfterGameInput(int choice)
        {
            switch (choice)
            {
                case 1:
                    leaderboard.AddWin(winnerName);
                    ResetGame();
                    gameState = GameState.Menu;
                    break;
                case 2:
                    Color.Blue();
                    Console.WriteLine("Начинаем новую игру!");
                    Color.ResetColor();
                    ResetGame();
                    StartNewGame();
                    break;
                case 3:
                    gameState = GameState.GameOver;
                    Color.Red();
                    Console.WriteLine("Выход в меню...");
                    Color.ResetColor();
                    break;
                default:
                    throw new ArgumentOutOfRangeException("Выбранный пункт не существует");
            }
        }

        private void ShowLeaderboard()
        {
            leaderboard.Display();
            Console.WriteLine("\nНажмите Enter для продолжения...");
            Console.ReadLine();
        }

        private void DemonstrateOOPFeatures()
        {
            Console.WriteLine("\n=== Демонстрация возможностей ООП ===");
            Console.WriteLine("\n--- Клонирование ---");

            var originalCoord = new Coordinate(5, 5);
            var shallowClone = (Coordinate)originalCoord.Clone();
            var deepClone = originalCoord.DeepClone();

            Console.WriteLine($"Оригинал: X={originalCoord.X}, Y={originalCoord.Y}");
            Console.WriteLine($"Поверхностный клон: X={shallowClone.X}, Y={shallowClone.Y}");
            Console.WriteLine($"Глубокий клон: X={deepClone.X}, Y={deepClone.Y}");

            Console.WriteLine("\n--- Виртуальные функции ---");

            Player human = new HumanPlayer("Тестовый игрок");
            Player ai = new AIPlayer();
            Player advancedAI = new AdvancedAIPlayer();

            Console.WriteLine("\nИнформация об игроках:");
            human.DisplayPlayerInfo();
            ai.DisplayPlayerInfo();
            advancedAI.DisplayPlayerInfo();

            Console.WriteLine("\n--- Наследование и полиморфизм ---");

            Player[] players = { human, ai, advancedAI };
            foreach (var player in players)
            {
                Console.WriteLine($"Тип: {player.GetType().Name}, Имя: {player.GetName()}");
            }

            Console.WriteLine("\nНажмите Enter для продолжения...");
            Console.ReadLine();
        }
    }
}