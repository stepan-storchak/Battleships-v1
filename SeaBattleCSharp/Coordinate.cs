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
            return obj is Coordinate coordinate &&
                   X == coordinate.X &&
                   Y == coordinate.Y;
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(X, Y);
        }
    }
}