using System.Collections.Generic;
using System.Linq;

namespace SeaBattleCSharp
{
    public class Ship
    {
        public int Size { get; }
        public Coordinate Start { get; }
        public Orientation Orientation { get; }
        public List<Coordinate> Coordinates { get; }
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