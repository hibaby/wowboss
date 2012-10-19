using UnityEngine;
using System.Collections;

public class Login : MonoBehaviour {
	
	public Texture btnTextureLogin;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	
	void OnGUI() {
		if (!btnTextureLogin) {
			Debug.LogError("Please assign a texture on the inspector");
			return;
		}
		if (GUI.Button(new Rect(10, 10, 200, 200), btnTextureLogin)) {
			Debug.Log("Clicked the button with an image");     
		}
	}
}
