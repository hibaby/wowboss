using UnityEngine;
using System.Collections;

public class ShowSelectedBump : MonoBehaviour {

	public Shader selectedShader;
	public Color outterColor;

	
	private Color myColor ;
	private Shader myShader;
	private SkinnedMeshRenderer sRenderer;
	private bool Selected = false;

	// Use this for initialization
	void Start () {
		sRenderer = GetComponentInChildren<SkinnedMeshRenderer>();
		myColor = sRenderer.material.color;
		myShader = sRenderer.material.shader;
		selectedShader = Shader.Find("Hidden/RimLightSpecBump");
		if(!selectedShader)
		{
			enabled = false;
			return;
		}
	}
	
	void SetLayerRecursively(GameObject obj,int newLayer)
	{
		obj.layer = newLayer;    
		
    	foreach (Transform child in obj.transform)
		{
			SetLayerRecursively( child.gameObject, newLayer );
   		}
	}
	
	// Update is called once per frame
	void Update () {
	}
	  void OnMouseEnter() {
        //renderer.material.color = Color.black;
		sRenderer.material.shader = selectedShader;
		sRenderer.material.SetColor("_RimColor",outterColor);
    }
	void OnMouseExit(){
		sRenderer.material.color = myColor;
		sRenderer.material.shader = myShader;
	}
	void OnMouseDown(){
		Selected  = !Selected;
		if(Selected)
		{
			SetLayerRecursively(gameObject,8);
			animation.Play();
		}
		else
		{
			SetLayerRecursively(gameObject,0);
			animation.Stop();
		}
			
	}
	
}
