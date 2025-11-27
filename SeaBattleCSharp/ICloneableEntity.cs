namespace SeaBattleCSharp
{
    public interface ICloneableEntity
    {
        object Clone();
        object DeepClone();
    }
}