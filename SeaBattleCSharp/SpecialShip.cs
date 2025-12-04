using System;
using System.Collections.Generic;
using System.Linq;

namespace SeaBattleCSharp
{
    public class SpecialShip : ShipBase
    {
        private int size;
        private Coordinate start;
        private Orientation orientation;
        private int armor;

        public override int Size
        {
            get { return size; }
            protected set
            {
                if (value <= 0 || value > 4)
                    throw new ArgumentException("Размер корабля должен быть от 1 до 4");
                size = value;
            }
        }

        public override Coordinate Start
        {
            get { return start; }
            protected set
            {
                start = value ?? throw new ArgumentNullException("Начальная координата не может быть null");
            }
        }

        public override Orientation Orientation
        {
            get { return orientation; }
            protected set { orientation = value; }
        }

        public override List<Coordinate> Coordinates { get; protected set; }
        public override List<Coordinate> Hits { get; } = new List<Coordinate>();

        public int Armor => armor;

        public SpecialShip(int size, Coordinate start, Orientation orientation, int armor = 1)
            : base()
        {
            Size = size;
            Start = start;
            Orientation = orientation;
            this.armor = armor;
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

        public override bool TakeHit(Coordinate coord)
        {
            if (Coordinates.Any(c => c.X == coord.X && c.Y == coord.Y))
            {
                if (armor > 0)
                {
                    armor--;
                    return false;
                }

                if (!Hits.Any(c => c.X == coord.X && c.Y == coord.Y))
                {
                    Hits.Add(coord);
                    return true;
                }
            }
            return false;
        }

        public override bool IsSunk()
        {
            return Hits.Count == Size;
        }

        public override object Clone()
        {
            var cloned = (SpecialShip)this.MemberwiseClone();
            cloned.Start = (Coordinate)this.Start.Clone();
            cloned.Coordinates = new List<Coordinate>(this.Coordinates.Select(c => (Coordinate)c.Clone()));
            return cloned;
        }

        public override object DeepClone()
        {
            var cloned = (SpecialShip)this.MemberwiseClone();
            cloned.Start = this.Start.DeepClone();
            cloned.Coordinates = new List<Coordinate>(this.Coordinates.Select(c => c.DeepClone()));
            return cloned;
        }

        public override string GetShipType() => "Special Armored Ship";
    }
}