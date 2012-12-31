using UnityEngine;
using System.Collections;

[RequireComponent (typeof(Camera))]
public class SelectObjectNew : MonoBehaviour {
	
	public int borderWidth = 3;
	public Color outterColor = new Color(0.133f,1,0,1);
	
	public GameObject[] targetsGroup1;
	public GameObject[] targetsGroup2;
	
	
	public Camera outterLineCamera;

	public Shader compositeShader;
    Material m_CompositeMaterial = null;
	protected Material compositeMaterial {
		get {
			if (m_CompositeMaterial == null) {
                m_CompositeMaterial = new Material(compositeShader);
				m_CompositeMaterial.hideFlags = HideFlags.HideAndDontSave;
			}
			return m_CompositeMaterial;
		} 
	}
	
	public Shader blurShader;
    Material m_BlurMaterial = null;
	protected Material blurMaterial {
		get {
			if (m_BlurMaterial == null) {
                m_BlurMaterial = new Material(blurShader);
                m_BlurMaterial.hideFlags = HideFlags.HideAndDontSave;
			}
			return m_BlurMaterial;
		} 
	}
	
	private Material m_outterLineMat = null;
	protected Material outterLineMat{
		get{
			if(m_outterLineMat == null){
				m_outterLineMat = new Material("Shader\"Hidden/SolidBody1\"{SubShader{Pass{Color("+
					outterColor.r +","+outterColor.g +","+outterColor.b +","+outterColor.a +")}}}");
				m_outterLineMat.hideFlags = HideFlags.HideAndDontSave;
				m_outterLineMat.shader.hideFlags =  HideFlags.HideAndDontSave; 
			}
			return m_outterLineMat;
		}
	}
	
	private RenderTexture outterLineTexture = null;
	

	
	void Start () {
		
		if (!SystemInfo.supportsImageEffects)
		{
			enabled = false;
			return;
		}
		if( !blurMaterial.shader.isSupported )
			enabled = false;
		if( !compositeMaterial.shader.isSupported )
			enabled = false;
		if( !outterLineMat.shader.isSupported )
			enabled = false;

		foreach(GameObject target in targetsGroup1)
		{
			target.AddComponent<ShowSelected>().outterColor = outterColor;
		}
		foreach(GameObject target in targetsGroup2)
		{
			target.AddComponent<ShowSelectedBump>().outterColor = outterColor;
		}
		if(!outterLineTexture)
		{
			outterLineTexture =  new RenderTexture( (int)camera.pixelWidth,(int)camera.pixelHeight, 16 );
			outterLineTexture.hideFlags = HideFlags.DontSave;
		}
 	}
	
	
	void OnDisable()
    {
		if(outterLineTexture)
		{
			DestroyImmediate(outterLineTexture);
		}
		if( m_CompositeMaterial ) {
			DestroyImmediate( m_CompositeMaterial );
		}
		if( m_BlurMaterial ) {
			DestroyImmediate( m_BlurMaterial );
		}
		if(m_outterLineMat) {
			DestroyImmediate (m_outterLineMat);
		}
	}
		
	void OnPreRender() 
	{
		outterLineCamera.targetTexture = outterLineTexture;
		outterLineCamera.RenderWithShader(outterLineMat.shader,"");
	}
	
	void OnRenderImage (RenderTexture source, RenderTexture destination)
	{
		borderWidth = Mathf.Clamp( borderWidth, 1,5);
		
		RenderTexture bufferFull = RenderTexture.GetTemporary(outterLineTexture.width, outterLineTexture.height, 0);
		
		Graphics.BlitMultiTap (outterLineTexture, bufferFull, blurMaterial,new Vector2(borderWidth,borderWidth));
		Graphics.Blit(source,destination);
		Graphics.Blit(bufferFull,destination,compositeMaterial);
		
		RenderTexture.ReleaseTemporary(bufferFull);
	}
}
