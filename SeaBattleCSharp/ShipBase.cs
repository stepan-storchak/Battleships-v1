using System;
using System.Collections.Generic;

namespace SeaBattleCSharp
{
    public abstract class ShipBase : ICloneable
    {
        public abstract int Size { get; protected set; }
        public abstract Coordinate Start { get; protected set; }
        public abstract Orientation Orientation { get; protected set; }
        public abstract List<Coordinate> Coordinates { get; protected set; }
        public abstract List<Coordinate> Hits { get; }

        public abstract bool TakeHit(Coordinate coord);
        public abstract bool IsSunk();
        public abstract object Clone();
        public abstract object DeepClone();

        public virtual string GetShipType() => "Base Ship";
    }
}