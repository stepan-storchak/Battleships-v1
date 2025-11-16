using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace SeaBattleCSharp
{
    public class Leaderboard
    {
        private Dictionary<string, int> records;
        private const string filename = "leaderboard.txt";

        public Leaderboard()
        {
            records = new Dictionary<string, int>();
            LoadFromFile();
        }

        public void LoadFromFile()
        {
            if (!File.Exists(filename))
            {
                return;
            }

            try
            {
                string[] lines = File.ReadAllLines(filename);
                foreach (string line in lines)
                {
                    string[] parts = line.Split(' ');
                    if (parts.Length == 2 && int.TryParse(parts[1], out int wins))
                    {
                        records[parts[0]] = wins;
                    }
                }
            }
            catch (Exception)
            {
                // Если файл поврежден, начинаем с пустой таблицы
            }
        }

        public void SaveToFile()
        {
            try
            {
                using StreamWriter file = new StreamWriter(filename);
                foreach (var record in records)
                {
                    file.WriteLine($"{record.Key} {record.Value}");
                }
            }
            catch (Exception)
            {
                Console.WriteLine("Ошибка сохранения таблицы лидеров!");
            }
        }

        public void AddWin(string playerName)
        {
            if (records.ContainsKey(playerName))
            {
                records[playerName]++;
            }
            else
            {
                records[playerName] = 1;
            }
            SaveToFile();

            Color.Green();
            Console.WriteLine($"Победа игрока {playerName} сохранена в таблице лидеров!");
            Color.ResetColor();
            Display();
        }

        public void Display()
        {
            Console.WriteLine();
            Color.Yellow();
            Console.WriteLine("=== ТАБЛИЦА ЛИДЕРОВ ===");
            Color.ResetColor();

            if (records.Count == 0)
            {
                Console.WriteLine("Пока нет записей.");
                return;
            }

            var sortedRecords = records.OrderByDescending(r => r.Value).ToList();

            Console.WriteLine("------------------------------");
            Console.WriteLine("Игрок\t\tПобеды");
            Console.WriteLine("------------------------------");

            for (int i = 0; i < sortedRecords.Count; i++)
            {
                var record = sortedRecords[i];

                if (i == 0)
                {
                    Color.Yellow();
                }
                else if (i == 1)
                {
                    Color.Gray();
                }
                else if (i == 2)
                {
                    Color.Red();
                }
                else
                {
                    Color.White();
                }

                Console.Write(record.Key);
                if (record.Key.Length < 8)
                {
                    Console.Write("\t\t");
                }
                else
                {
                    Console.Write("\t");
                }
                Console.WriteLine(record.Value);

                Color.ResetColor();
            }
            Console.WriteLine("------------------------------");
        }
    }
}