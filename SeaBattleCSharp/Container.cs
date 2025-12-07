using System;
using System.Collections.Generic;
using System.Linq;

namespace SeaBattleCSharp
{
    public class Container<T> where T : ShipBase
    {
        private List<T> items = new List<T>();

        public void Add(T item) => items.Add(item);
        public bool Remove(T item) => items.Remove(item);
        public int Count => items.Count;
        public T this[int index] => items[index];

        public void SortBySize() => items.Sort((a, b) => a.Size.CompareTo(b.Size));

        public T FindById(int id) => items.Find(x => x.GetHashCode() == id);

        public List<T> FindAll(Predicate<T> match) => items.FindAll(match);

        public List<U> ConvertAll<U>(Converter<T, U> converter) => items.ConvertAll(converter);

        public bool Any(Predicate<T> match) => items.Any(x => match(x));

        public IEnumerable<T> Where(Func<T, bool> predicate) => items.Where(predicate);

        public IEnumerable<U> Select<U>(Func<T, U> selector) => items.Select(selector);

        public void ForEach(Action<T> action) => items.ForEach(action);

        public List<T> ToList() => new List<T>(items);

        public T[] ToArray() => items.ToArray();

        public Dictionary<int, T> ToDictionary() => items.ToDictionary(x => x.GetHashCode());

        public void Clear() => items.Clear();

        public bool Contains(T item) => items.Contains(item);

        public T Find(Predicate<T> match) => items.Find(match);

        public T FindLast(Predicate<T> match) => items.FindLast(match);

        public int FindIndex(Predicate<T> match) => items.FindIndex(match);

        public int FindLastIndex(Predicate<T> match) => items.FindLastIndex(match);

        public List<T> GetRange(int index, int count) => items.GetRange(index, count);

        public void Insert(int index, T item) => items.Insert(index, item);

        public void RemoveAt(int index) => items.RemoveAt(index);

        public void RemoveAll(Predicate<T> match) => items.RemoveAll(match);

        public void Reverse() => items.Reverse();

        public T Min(Func<T, int> selector) => items.MinBy(selector);

        public T Max(Func<T, int> selector) => items.MaxBy(selector);

        public double Average(Func<T, int> selector) => items.Average(selector);

        public int Sum(Func<T, int> selector) => items.Sum(selector);
    }
}