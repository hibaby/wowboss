using UnityEngine;
using System.Collections;

public class Battlefiled : MonoBehaviour {
	
	public bool[,] positions;
	// Use this for initialization
	void Start () {
		NewBattle();
	}
	
	void NewBattle () {
		positions = new bool[3,4]
		{
			{ true, false, true, false },
			{ true, false, true, false },
			{ true, false, true, true }
		};
	
	}
}
