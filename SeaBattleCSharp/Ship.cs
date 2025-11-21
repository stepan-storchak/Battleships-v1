using System;
using System.Collections.Generic;
using System.Linq;

namespace SeaBattleCSharp
{
    public class Ship
    {
        private int size;
        private Coordinate start;
        private Orientation orientation;

        public int Size
        {
            get { return size; }
            private set
            {
                if (value <= 0 || value > 4)
                    throw new ArgumentException("Размер корабля должен быть от 1 до 4");
                size = value;
            }
        }

        public Coordinate Start
        {
            get { return start; }
            private set
            {
                start = value ?? throw new ArgumentNullException("Начальная координата не может быть null");
            }
        }

        public Orientation Orientation
        {
            get { return orientation; }
            private set { orientation = value; }
        }

        public List<Coordinate> Coordinates { get; private set; }
        public List<Coordinate> Hits { get; } = new List<Coordinate>();

        public Ship(int size, Coordinate start, Orientation orientation)
        {
            Size = size;
            Start = start;
            Orientation = orientation;
            Coordinates = CalculateCoordinates();
        }

        private List<Coordinate> CalculateCoordinates()
        {
            var coords = new List<Coordinate>();

            try
            {
                for (int i = 0; i < Size; i++)
                {
                    if (Orientation == Orientation.Horizontal)
                    {
                        coords.Add(new Coordinate(Start.X + i, Start.Y));
                    }
                    else
                    {
                        coords.Add(new Coordinate(Start.X, Start.Y + i));
                    }
                }
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException($"Ошибка при расчете координат корабля: {ex.Message}");
            }

            return coords;
        }

        public bool TakeHit(Coordinate coord)
        {
            if (Coordinates.Any(c => c.X == coord.X && c.Y == coord.Y) &&
                !Hits.Any(c => c.X == coord.X && c.Y == coord.Y))
            {
                Hits.Add(coord);
                return true;
            }
            return false;
        }

        public bool IsSunk()
        {
            return Hits.Count == Size;
        }
    }
}