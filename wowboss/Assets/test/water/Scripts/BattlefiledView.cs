using UnityEngine;
using System.Collections;

public class BattlefiledView : MonoBehaviour {
	
	public GameObject prefabBlockFloor;
	
	private Battlefiled battlefiled;
	// Use this for initialization
	void Start () {
		battlefiled = GetComponent<Battlefiled>();
	}
	
	void NewBattlefiled () {
		for ( int i=0; i<battlefiled.positions.GetLength(0); ++i )
		{
			for ( int j=0; j<battlefiled.positions.GetLength(1); ++j )
			{
				Vector3 postion = new Vector3( 2.0f*i, 0.5f, 2.0f*i );
				Quaternion rotation = new Quaternion( 0.0f, 0.0f, 0.0f, 0.0f );
				Instantiate( prefabBlockFloor, postion, rotation);
			}
		}
	}
}
