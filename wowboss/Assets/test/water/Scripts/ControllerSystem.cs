using UnityEngine;
using System.Collections;

public class ControllerSystem : MonoBehaviour {
	
	public string sAnimation = "idle";
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		animation.CrossFade(sAnimation);
	
	}
}
