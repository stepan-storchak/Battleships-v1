namespace SeaBattleCSharp
{
	public interface IPlayer
	{
		string GetName();
		void MakeMove(Player enemy);
		bool AllShipsSunk();
	}
}