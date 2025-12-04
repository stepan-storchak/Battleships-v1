using System;
using System.Collections.Generic;
using System.Linq;

namespace SeaBattleCSharp
{
    public static class GenericHelper
    {
        public static T CalculateTotal<T>(IEnumerable<T> items, Func<T, int> selector) where T : struct
        {
            int total = 0;
            foreach (var item in items)
            {
                total += selector(item);
            }
            return (T)Convert.ChangeType(total, typeof(T));
        }

        public static int CalculateTotalSize(IEnumerable<ShipBase> ships)
        {
            return ships.Sum(s => s.Size);
        }

        public static T FindMax<T>(IEnumerable<T> items) where T : IComparable<T>
        {
            if (items == null || !items.Any())
                throw new ArgumentException("Коллекция не может быть пустой");

            T max = items.First();
            foreach (var item in items)
            {
                if (item.CompareTo(max) > 0)
                    max = item;
            }
            return max;
        }

        public static T FindMin<T>(IEnumerable<T> items) where T : IComparable<T>
        {
            if (items == null || !items.Any())
                throw new ArgumentException("Коллекция не может быть пустой");

            T min = items.First();
            foreach (var item in items)
            {
                if (item.CompareTo(min) < 0)
                    min = item;
            }
            return min;
        }
    }
}