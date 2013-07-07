using UnityEngine;
using System.Collections;

public class Attacker : MonoBehaviour {

	// Use this for initialization
	public GameObject goPath;
	public Path path;
	public float moveSpeed = 10.0f;
	
	//红色血条贴图  
	public Texture2D blood_red;  
	//黑色血条贴图  
	public Texture2D blood_black; 

	
	private Camera camera;
	private float height = 5.0f;
	private string name = "monster";
	
	private int current = 0;
	private int life = 100;
	
	private const float cellWidth = 10.0f;
	private const float terrainWidth = 10 * cellWidth;
	private Vector2 orgin = new Vector2( - terrainWidth/2 + cellWidth/2, - terrainWidth/2 + cellWidth/2 );
	void Start () {
		//path = goPath.GetComponent<Path>();
		current = 0;
		Vector2 nextPoint = path.Paths[current];
		Vector2 nextPostion = new Vector2( nextPoint.x * cellWidth + orgin.x, nextPoint.y * cellWidth + orgin.y );
		transform.position = new Vector3( nextPostion.x,transform.position.y, nextPostion.y );	
		
	camera = Camera.main;  
	//得到模型原始高度  
//	float size_y = collider.bounds.size.y;  
	//得到模型缩放比例  
//	float scal_y = transform.localScale.y;  
	//它们的乘积就是高度  
//	height = (size_y *scal_y) ;  

	
	}
	
	// Update is called once per frame
	void Update () {
		if ( life < 1 )
		{
			return;
		}
		
		float deltaLength = Time.deltaTime * moveSpeed;
			
		int next = current + 1;
		while ( next < path.Paths.Length )
		{
			Vector2 currentPostion = new Vector2( transform.position.x, transform.position.z );
			Vector2 nextPoint = path.Paths[next];
			Vector2 nextPostion = new Vector2( nextPoint.x * cellWidth + orgin.x, nextPoint.y * cellWidth + orgin.y );
			float thisLength = Vector2.Distance(nextPostion, currentPostion);
			
			if ( deltaLength > thisLength )
			{
				transform.position = new Vector3( nextPostion.x,transform.position.y, nextPostion.y );
				current = current + 1;
				next = current + 1;
				
				
		Debug.Log( current );
		Debug.Log( transform.position );
			}
			else
			{
				Vector2 dir = nextPostion - currentPostion;
				dir.Normalize();
				dir = dir * deltaLength;
				transform.position = new Vector3( transform.position.x + dir.x,transform.position.y, transform.position.z + dir.y );
				break;
			}
		}
		
		if ( next >= path.Paths.Length )
		{
			Dead ();
		}
	}
	
	void Dead () {
		if ( life > 0 )
		{
			life = 0;
			Destroy( gameObject, 1 );
			Debug.Log( "Dead" );
		}
	}
	
	public void Damage(int v) {
		if ( life > v )
		{
			life = life - v;
			Debug.Log( "Damage ("+ life.ToString() + ")" );
		}
		else
		{
			Dead();
		}
	}
	
	    void OnGUI() {
		//得到NPC头顶在3D世界中的坐标  
		//默认NPC坐标点在脚底下，所以这里加上npcHeight它模型的高度即可  
		Vector3 worldPosition = new Vector3 (transform.position.x , transform.position.y + height,transform.position.z);  
		
		//根据NPC头顶的3D坐标换算成它在2D屏幕中的坐标  
		Vector2 position = camera.WorldToScreenPoint (worldPosition);  
		
		//得到真实NPC头顶的2D坐标  
		position = new Vector2 (position.x, Screen.height - position.y);
		
		//注解2  
		//计算出血条的宽高  
		Vector2 bloodSize = GUI.skin.label.CalcSize (new GUIContent(blood_red));  
		bloodSize.y = bloodSize.y/5;
		
		//通过血值计算红色血条显示区域  
		int blood_width = blood_red.width * life/100;  
		
		//先绘制黑色血条  
		GUI.DrawTexture(new Rect(position.x - (bloodSize.x/2),position.y - bloodSize.y ,bloodSize.x,bloodSize.y),blood_black);  
		
		//在绘制红色血条  
		GUI.DrawTexture(new Rect(position.x - (bloodSize.x/2),position.y - bloodSize.y ,blood_width,bloodSize.y),blood_red);  
		
		//注解3  
		//计算NPC名称的宽高  
		Vector2 nameSize = GUI.skin.label.CalcSize (new GUIContent(name));  
		//设置显示颜色为黄色  
		GUI.color  = Color.yellow;  
		//绘制NPC名称  
		 GUI.Label(new Rect(position.x - (nameSize.x/2),position.y - nameSize.y - bloodSize.y ,nameSize.x,nameSize.y), name);  
		} 

}
