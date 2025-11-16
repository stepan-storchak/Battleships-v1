using System;

namespace SeaBattleCSharp
{
    public class GameBoard
    {
        private const int BOARD_SIZE = 10;
        private CellState[,] grid;

        public GameBoard()
        {
            grid = new CellState[BOARD_SIZE, BOARD_SIZE];
            ClearBoard();
        }

        public bool IsValidPlacement(Ship ship)
        {
            foreach (var coord in ship.Coordinates)
            {
                if (coord.X < 0 || coord.X >= BOARD_SIZE || coord.Y < 0 || coord.Y >= BOARD_SIZE)
                {
                    return false;
                }

                if (!IsCellEmpty(coord))
                {
                    return false;
                }
            }
            return true;
        }

        public bool PlaceShip(Ship ship)
        {
            if (!IsValidPlacement(ship))
            {
                return false;
            }

            CellState shipState = ship.Size switch
            {
                1 => CellState.Ship1,
                2 => CellState.Ship2,
                3 => CellState.Ship3,
                4 => CellState.Ship4,
                _ => CellState.Empty
            };

            foreach (var coord in ship.Coordinates)
            {
                grid[coord.Y, coord.X] = shipState;
            }
            return true;
        }

        public ShotResult ReceiveShot(Coordinate coord)
        {
            if (coord.X < 0 || coord.X >= BOARD_SIZE || coord.Y < 0 || coord.Y >= BOARD_SIZE)
            {
                return ShotResult.Miss;
            }

            CellState cell = grid[coord.Y, coord.X];
            switch (cell)
            {
                case CellState.Empty:
                    grid[coord.Y, coord.X] = CellState.Miss;
                    return ShotResult.Miss;
                case CellState.Ship1:
                case CellState.Ship2:
                case CellState.Ship3:
                case CellState.Ship4:
                    grid[coord.Y, coord.X] = CellState.Hit;
                    return ShotResult.Hit;
                default:
                    return ShotResult.Miss;
            }
        }

        public bool IsCellEmpty(Coordinate coord)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    int nx = coord.X + dx;
                    int ny = coord.Y + dy;

                    if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE)
                    {
                        CellState state = grid[ny, nx];
                        if (state != CellState.Empty && state != CellState.Miss)
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        public void Display(bool showShips)
        {
            Console.Write("  ");
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                Console.Write($"{(char)('A' + i)} ");
            }
            Console.WriteLine();

            for (int y = 0; y < BOARD_SIZE; y++)
            {
                Console.Write($"{y + 1}");
                if (y < 9) Console.Write(" ");
                Console.Write(" ");

                for (int x = 0; x < BOARD_SIZE; x++)
                {
                    CellState state = grid[y, x];
                    char symbol = state switch
                    {
                        CellState.Empty => '~',
                        CellState.Miss => 'O',
                        CellState.Hit => 'X',
                        CellState.Ship1 => showShips ? '1' : '~',
                        CellState.Ship2 => showShips ? '2' : '~',
                        CellState.Ship3 => showShips ? '3' : '~',
                        CellState.Ship4 => showShips ? '4' : '~',
                        _ => '~'
                    };
                    Console.Write($"{symbol} ");
                }
                Console.WriteLine();
            }
        }

        public CellState GetCellState(Coordinate coord)
        {
            if (coord.X < 0 || coord.X >= BOARD_SIZE || coord.Y < 0 || coord.Y >= BOARD_SIZE)
            {
                return CellState.Empty;
            }
            return grid[coord.Y, coord.X];
        }

        public void SetCellState(Coordinate coord, CellState state)
        {
            if (coord.X >= 0 && coord.X < BOARD_SIZE && coord.Y >= 0 && coord.Y < BOARD_SIZE)
            {
                grid[coord.Y, coord.X] = state;
            }
        }

        public void ClearBoard()
        {
            for (int y = 0; y < BOARD_SIZE; y++)
            {
                for (int x = 0; x < BOARD_SIZE; x++)
                {
                    grid[y, x] = CellState.Empty;
                }
            }
        }

        public static int GetBoardSize() => BOARD_SIZE;
    }
}