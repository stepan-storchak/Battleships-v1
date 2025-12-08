namespace SeaBattleCSharp
{
    public class Coordinate
    {
        public int X { get; set; }
        public int Y { get; set; }

        public Coordinate(int x = 0, int y = 0)
        {
            X = x;
            Y = y;
        }

        public override bool Equals(object obj)
        {
            if (obj is Coordinate other)
                return X == other.X && Y == other.Y;
            return false;
        }

        public override int GetHashCode()
        {
            return (X << 16) | Y;
        }

        public static bool operator ==(Coordinate a, Coordinate b)
        {
            return a?.X == b?.X && a?.Y == b?.Y;
        }

        public static bool operator !=(Coordinate a, Coordinate b)
        {
            return !(a == b);
        }

        public static Coordinate operator +(Coordinate a, Coordinate b)
        {
            return new Coordinate(a.X + b.X, a.Y + b.Y);
        }

        public static Coordinate operator -(Coordinate a, Coordinate b)
        {
            return new Coordinate(a.X - b.X, a.Y - b.Y);
        }

        public override string ToString()
        {
            return $"{(char)('A' + X)}{Y + 1}";
        }
    }
}