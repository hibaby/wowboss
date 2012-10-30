using UnityEngine;
using System.Collections;

public class ExampleManager : MonoBehaviour 
{

    public XffectCache EffectCache;

    //loop xffet
    public CompositeXffect XLightBeams;
    protected bool ShowLightBeams = false;
    public XffectComponent XRaining;
    protected bool ShowRaining = false;
    protected bool ShowVolumeFog = false;
    public XffectComponent XPortalCone;
    protected bool ShowPortalCone = false;


    public XffectComponent XMissile3;


    //Explode
    public CompositeXffect XExplode3;


    public XffectComponent XVolumetricLight1;
    protected bool ShowVolumetricLight1 = false;

    protected Vector2 ScrollPosition = Vector2.zero;
    protected float m_checkTime = 0f;
	
	void Update () 
    {

        if (ShowLightBeams)
            XLightBeams.Active();
        else
            XLightBeams.DeActive();
        if (ShowRaining)
            XRaining.Active();
        else
            XRaining.DeActive();

        if (ShowPortalCone)
            XPortalCone.Active();
        else
            XPortalCone.DeActive();

        if (ShowVolumetricLight1)
            XVolumetricLight1.Active();
        else
            XVolumetricLight1.DeActive();
	}

    void OnGUI()
    {
        GUI.Label(new Rect(150, 0, 350, 25), "left button to rotate, middle button to pan, wheel to zoom.");
        GUI.Label(new Rect(150, 18, 600, 25), "xffect free version 2.0.0.");
        ScrollPosition = GUI.BeginScrollView(new Rect(0, 0, 140, 600), ScrollPosition, new Rect(0, 0, 140, 690));
        ShowLightBeams = GUI.Toggle(new Rect(10, 0, 80, 20), ShowLightBeams, "LightBeams");
        ShowRaining = GUI.Toggle(new Rect(10, 20, 80, 20), ShowRaining, "Raining");
        ShowPortalCone = GUI.Toggle(new Rect(10, 40, 80, 20), ShowPortalCone, "PortalCone");
        if (GUI.Button(new Rect(10, 60, 80, 20), "missile3"))
        {
            XMissile3.Active();
            XMissile3.transform.position = new Vector3(0, 0, 40);
            SimpleMissile smile = XMissile3.transform.GetComponent<SimpleMissile>();
            smile.Reset();
        }
        if (GUI.Button(new Rect(10, 80, 80, 20), "explosion3"))
        {
            XExplode3.Active();
        }
        ShowVolumetricLight1 = GUI.Toggle(new Rect(10, 100, 120, 20), ShowVolumetricLight1, "VolumetricLight1");
        GUI.EndScrollView();
    }
}
