using UnityEngine;
using System.Collections;

[RequireComponent (typeof(Camera))]
public class SelectObject : MonoBehaviour {
	
	public int Iterations = 3;
	public float Spread = 0.7f;
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
	
	
	public Shader cutoffShader;
	Material m_cutoffMaterial = null;
	protected Material cutoffMaterial {
		get {
			if (m_cutoffMaterial == null) {
				m_cutoffMaterial = new Material( cutoffShader );
				m_cutoffMaterial.hideFlags = HideFlags.HideAndDontSave;
			}
			return m_cutoffMaterial;
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
		if( !cutoffMaterial.shader.isSupported )
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
			outterLineTexture = null;
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
	
	public void FourTapCone (RenderTexture source, RenderTexture dest, int iteration)
	{
		float off = 0.5f + iteration*Spread;
		Graphics.BlitMultiTap (source, dest, blurMaterial,
            new Vector2( off, off),
			new Vector2(-off, off),
            new Vector2( off,-off),
            new Vector2(-off,-off)
		);
	}
	
	void OnPreRender() 
	{
		outterLineCamera.targetTexture = outterLineTexture;
		outterLineCamera.RenderWithShader(outterLineMat.shader,"");
	}
	
	void OnRenderImage (RenderTexture source, RenderTexture destination)
	{
		Iterations = Mathf.Clamp( Iterations, 0, 15 );
		Spread = Mathf.Clamp( Spread, 0.5f, 6.0f );
		
		RenderTexture buffer = RenderTexture.GetTemporary(outterLineTexture.width, outterLineTexture.height, 0);
		RenderTexture buffer2 = RenderTexture.GetTemporary(outterLineTexture.width, outterLineTexture.height, 0);
		
		Graphics.Blit(outterLineTexture, buffer);

		bool oddEven = true;
		for(int i = 0; i < Iterations; i++)
		{
			if( oddEven )
				FourTapCone (buffer, buffer2, i);
			else
				FourTapCone (buffer2, buffer, i);
			oddEven = !oddEven;
		}
		Graphics.Blit(source,destination);
		if( oddEven ){
			Graphics.Blit(outterLineTexture,buffer, cutoffMaterial);
			Graphics.Blit(buffer,destination,compositeMaterial);
		}
		else{
			Graphics.Blit(outterLineTexture,buffer2, cutoffMaterial);
			Graphics.Blit(buffer2,destination,compositeMaterial);
		}		
		
		RenderTexture.ReleaseTemporary(buffer);
		RenderTexture.ReleaseTemporary(buffer2);
	}
}
