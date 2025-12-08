using System.Collections.Generic;
using System.Linq;

namespace SeaBattleCSharp
{
    public class Ship
    {
        public int Size { get; private set; }
        public List<Coordinate> Coordinates { get; private set; }
        public bool[] Hits { get; private set; }
        public string Name { get; private set; }

        public Ship(int size, Coordinate start, Orientation orientation, string name = "")
        {
            Size = size;
            Name = name;
            Coordinates = new List<Coordinate>();
            Hits = new bool[size];

            int dx = (orientation == Orientation.Horizontal) ? 1 : 0;
            int dy = (orientation == Orientation.Vertical) ? 1 : 0;

            for (int i = 0; i < size; i++)
            {
                Coordinates.Add(new Coordinate(start.X + dx * i, start.Y + dy * i));
            }
        }

        public bool IsSunk()
        {
            return Hits.All(hit => hit);
        }

        public bool TakeHit(Coordinate coord)
        {
            for (int i = 0; i < Coordinates.Count; i++)
            {
                if (Coordinates[i].Equals(coord))
                {
                    Hits[i] = true;
                    return IsSunk();
                }
            }
            return false;
        }
    }
}