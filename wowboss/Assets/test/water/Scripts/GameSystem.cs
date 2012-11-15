using UnityEngine;
using System.Collections;

public class GameSystem : MonoBehaviour {
	
	static private bool bInit = false;
	static private GamePrefabs gps;
	public static void MakeFloor() {
		CheckInit();
		
		Vector3 postion = new Vector3( 0.0f, 0.0f, 0.0f );
		Quaternion rotation = new Quaternion( 0.0f, 0.0f, 0.0f, 0.0f );
		Instantiate( gps.prefabBattlefiled, postion, rotation);
		
/*		DestroyFloor();
		for ( int y=0; y<9; ++y )
		{
			for ( int x=0; x<9; ++x )
			{
				Vector3 postion = new Vector3( 2.0f*x, 0.5f, 2.0f*y );
				Quaternion rotation = new Quaternion( 0.0f, 0.0f, 0.0f, 0.0f );
				Instantiate( gps.prefabBlockFloor, postion, rotation);
			}
		}
*/
	}
	
	static public void Find() {
		GameObject go =  GameObject.Find("Game");
		gps = go.GetComponent<GamePrefabs>();
	}
	static void Init() {
		
		Find();
		bInit = true;
	}
	
	static void CheckInit() {
		if ( !bInit )
		{
			Init();
		}
	}
	
	static void DestroyFloor() {
		GameObject[] gos;
		gos = GameObject.FindGameObjectsWithTag(gps.prefabBattlefiled.tag);  
		foreach (GameObject go in gos) 
		{            
			Destroy(go);
		}
	}
}
