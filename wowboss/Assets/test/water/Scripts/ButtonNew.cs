using UnityEngine;
using System.Collections;

public class ButtonNew : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	void OnClick() {
		GameSystem.MakeFloor();
	}
}
