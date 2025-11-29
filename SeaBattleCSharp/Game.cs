using System;

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
                                    Console.WriteLine($"Óńďĺő! {currentPlayer.GetName()} ďđîäîëćŕĺň őîä.");
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
                Console.WriteLine($"Íĺďđĺäâčäĺííŕ˙ îřčáęŕ â čăđĺ: {ex.Message}");
                Color.ResetColor();
                Console.WriteLine("Íŕćěčňĺ Enter äë˙ ďđîäîëćĺíč˙...");
                Console.ReadLine();
            }
        }

        private Player GetOpponent()
        {
            return (currentPlayer == player1) ? player2 : player1;
        }

        private void ShowMainMenu()
        {
            Color.Green();
            Console.WriteLine("\n=== Ěîđńęîé áîé ===");
            Color.ResetColor();
            Console.WriteLine("1. Íŕ÷ŕňü íîâóţ čăđó");
            Console.WriteLine("2. Ďîęŕçŕňü ňŕáëčöó ëčäĺđîâ");
            Console.WriteLine("3. Äĺěîíńňđŕöč˙ âîçěîćíîńňĺé ÎÎĎ");
            Console.WriteLine("4. Âűőîä");
            Console.Write("Âűáĺđčňĺ ďóíęň: ");

            try
            {
                if (int.TryParse(Console.ReadLine(), out int choice))
                {
                    ProcessMenuInput(choice);
                }
                else
                {
                    throw new FormatException("Íĺâĺđíűé ôîđěŕň ââîäŕ");
                }
            }
            catch (FormatException ex)
            {
                Color.Red();
                Console.WriteLine($"Îřčáęŕ ââîäŕ: {ex.Message}");
                Color.ResetColor();
            }
        }

        private void ShowAfterGameMenu()
        {
            Console.WriteLine();
            Color.Green();
            Console.WriteLine("=== Čăđŕ çŕâĺđřĺíŕ ===");
            Color.ResetColor();
            Color.Yellow();
            Console.WriteLine($"Ďîáĺäčňĺëü: {winnerName}!");
            Color.ResetColor();
            Console.WriteLine("\nÄîńňóďíűĺ äĺéńňâč˙:");
            Color.Green();
            Console.WriteLine("1. Ńîőđŕíčňü đĺçóëüňŕň â ňŕáëčöó ëčäĺđîâ");
            Color.Blue();
            Console.WriteLine("2. Íŕ÷ŕňü íîâóţ čăđó (ňîň ćĺ đĺćčě)");
            Color.Red();
            Console.WriteLine("3. Âűőîä â ěĺíţ");
            Color.ResetColor();
            Console.Write("Âŕř âűáîđ: ");

            try
            {
                if (int.TryParse(Console.ReadLine(), out int choice))
                {
                    ProcessAfterGameInput(choice);
                }
                else
                {
                    throw new FormatException("Íĺâĺđíűé ôîđěŕň ââîäŕ");
                }
            }
            catch (FormatException ex)
            {
                Color.Red();
                Console.WriteLine($"Îřčáęŕ ââîäŕ: {ex.Message}");
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
                    Console.WriteLine("Íŕ÷číŕĺě íîâóţ čăđó!");
                    Color.ResetColor();
                    ResetGame();
                    StartNewGame();
                    break;
                case 3:
                    gameState = GameState.GameOver;
                    Color.Red();
                    Console.WriteLine("Âűőîä â ěĺíţ...");
                    Color.ResetColor();
                    break;
                default:
                    throw new ArgumentOutOfRangeException("Âűáđŕííűé ďóíęň íĺ ńóůĺńňâóĺň");
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
            Console.WriteLine("=== Čăđŕ çŕâĺđřĺíŕ ===");
            Color.ResetColor();
            if (winner == "Computer" || winner == "Advanced Computer")
            {
                Color.Red();
                Console.WriteLine("Ęîěďüţňĺđ ďîáĺäčë!");
            }
            else
            {
                Color.Green();
                Console.WriteLine($"Ďîáĺäčë {winner}!");
            }
            Color.ResetColor();
            Console.WriteLine("\nÔčíŕëüíîĺ ńîńňî˙íčĺ äîńęč:");
            Color.Green();
            Console.WriteLine("Âŕřŕ äîńęŕ:");
            Color.ResetColor();
            player1.GetMyBoard().Display(true);
            Color.Blue();
            Console.WriteLine("\nÄîńęŕ ďđîňčâíčęŕ:");
            Color.ResetColor();
            player1.GetEnemyBoard().Display(false);
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
                    break;
                case 4:
                    gameState = GameState.GameOver;
                    Console.WriteLine("Âűőîä...");
                    Color.ResetColor();
                    break;
                default:
                    throw new ArgumentOutOfRangeException("Âűáđŕííűé ďóíęň íĺ ńóůĺńňâóĺň");
            }
        }

        private void StartNewGame()
        {
            ResetGame();
            Console.Write("Ââĺäčňĺ âŕřĺ čě˙: ");
            string playerName = Console.ReadLine();

            if (string.IsNullOrWhiteSpace(playerName))
            {
                throw new ArgumentException("Čě˙ íĺ ěîćĺň áűňü ďóńňűě");
            }

            Console.WriteLine("\nÂűáĺđčňĺ ňčď ďđîňčâíčęŕ:");
            Console.WriteLine("1 - Îáű÷íűé ęîěďüţňĺđ");
            Console.WriteLine("2 - Ďđîäâčíóňűé ęîěďüţňĺđ");
            Console.Write("Âŕř âűáîđ: ");

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
            Console.WriteLine($"\nČăđŕ íŕ÷ŕëŕńü! Óäŕ÷č, {playerName}!");
            Color.ResetColor();
        }

        private void ResetGame()
        {
            player1 = null;
            player2 = null;
            currentPlayer = null;
            winnerName = "";
        }

        private void ShowLeaderboard()
        {
            leaderboard.Display();
            Console.WriteLine("\nÍŕćěčňĺ Enter äë˙ ďđîäîëćĺíč˙...");
            Console.ReadLine();
        }

        private void DemonstrateOOPFeatures()
        {
            Console.WriteLine("\n=== Äĺěîíńňđŕöč˙ âîçěîćíîńňĺé ÎÎĎ ===");

            // 1. Äĺěîíńňđŕöč˙ ęëîíčđîâŕíč˙
            Console.WriteLine("\n--- Ęëîíčđîâŕíčĺ ---");
            var originalCoord = new Coordinate(5, 5);
            var shallowClone = (Coordinate)originalCoord.Clone();
            var deepClone = originalCoord.DeepClone();

            Console.WriteLine($"Îđčăčíŕë: X={originalCoord.X}, Y={originalCoord.Y}");
            Console.WriteLine($"Ďîâĺđőíîńňíűé ęëîí: X={shallowClone.X}, Y={shallowClone.Y}");
            Console.WriteLine($"Ăëóáîęčé ęëîí: X={deepClone.X}, Y={deepClone.Y}");

            // 2. Äĺěîíńňđŕöč˙ âčđňóŕëüíűő ôóíęöčé
            Console.WriteLine("\n--- Âčđňóŕëüíűĺ ôóíęöčč ---");
            Player human = new HumanPlayer("Ňĺńňîâűé čăđîę");
            Player ai = new AIPlayer();
            Player advancedAI = new AdvancedAIPlayer();

            Console.WriteLine("\nČíôîđěŕöč˙ îá čăđîęŕő:");
            human.DisplayPlayerInfo();
            ai.DisplayPlayerInfo();
            advancedAI.DisplayPlayerInfo();

            // 3. Äĺěîíńňđŕöč˙ íŕńëĺäîâŕíč˙ č ďîëčěîđôčçěŕ
            Console.WriteLine("\n--- Íŕńëĺäîâŕíčĺ č ďîëčěîđôčçě ---");
            Player[] players = { human, ai, advancedAI };
            foreach (var player in players)
            {
                Console.WriteLine($"Ňčď: {player.GetType().Name}, Čě˙: {player.GetName()}");
            }

            Console.WriteLine("\nÍŕćěčňĺ Enter äë˙ ďđîäîëćĺíč˙...");
            Console.ReadLine();
        }
    }
}