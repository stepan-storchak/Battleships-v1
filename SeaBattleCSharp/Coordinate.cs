using System;

namespace SeaBattleCSharp
{
    public class Coordinate : ICloneable
    {
        private int x;
        private int y;

        public int X
        {
            get { return x; }
            set
            {
                if (value < 0)
                    throw new ArgumentException(" оордината X не может быть отрицательной");
                x = value;
            }
        }

        public int Y
        {
            get { return y; }
            set
            {
                if (value < 0)
                    throw new ArgumentException(" оордината Y не может быть отрицательной");
                y = value;
            }
        }

        public Coordinate(int x = 0, int y = 0)
        {
            X = x;
            Y = y;
        }

        public override bool Equals(object obj)
        {
            return obj is Coordinate coordinate &&
                   X == coordinate.X &&
                   Y == coordinate.Y;
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(X, Y);
        }

        public object Clone()
        {
            return this.MemberwiseClone();
        }

        public Coordinate DeepClone()
        {
            return new Coordinate(X, Y);
        }
    }
}