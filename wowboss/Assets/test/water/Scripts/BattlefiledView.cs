using UnityEngine;
using System.Collections;

public class BattlefiledView : MonoBehaviour {
	
	public GameObject prefabBlockFloor;
	
	private Battlefiled battlefiled;
	// Use this for initialization
	void Start () {
		battlefiled = GetComponent<Battlefiled>();
	}
	
	public void NewBattlefiled () {
		DestroyBattlefiled();
		
		for ( int i=0; i<battlefiled.positions.GetLength(0); ++i )
		{
			for ( int j=0; j<battlefiled.positions.GetLength(1); ++j )
			{
				float w = prefabBlockFloor.transform.localScale.x + 0.2f;
				float h = battlefiled.positions[i,j] ? 0.0f : -0.1f;
				Vector3 postion = new Vector3( w*i, h, w*j );
				Quaternion rotation = new Quaternion( 0.0f, 0.0f, 0.0f, 0.0f );
				Instantiate( prefabBlockFloor, postion, rotation);
			}
		}
	}
	
	void DestroyBattlefiled() {
		GameObject[] gos;
		gos = GameObject.FindGameObjectsWithTag(prefabBlockFloor.tag);  
		foreach (GameObject go in gos) 
		{            
			Destroy(go);
		}
	}
}
