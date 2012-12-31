using UnityEngine;
using System.Collections;

public class ShowSelected : MonoBehaviour {
	
	public Shader selectedShader;
	public Color outterColor;

	
	private Color myColor ;
	private Shader myShader;
	private bool Selected = false;
	
	// Use this for initialization
	void Start () {
		myColor = renderer.material.color;
		myShader = renderer.material.shader;
		selectedShader = Shader.Find("Hidden/RimLightSpce");
		if(!selectedShader)
		{
			enabled = false;
			return;
		}
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	  void OnMouseEnter() {
        //renderer.material.color = Color.black;
		renderer.material.shader = selectedShader;
		renderer.material.SetColor("_RimColor",outterColor);
    }
	void OnMouseExit(){
		renderer.material.color = myColor;
		renderer.material.shader = myShader;
	}
	void OnMouseDown(){
		Selected  = !Selected;
		gameObject.layer= Selected ? 8 : 0;
	}
}
