using UnityEngine;
using System.Collections;

public class Attacker : MonoBehaviour {

	// Use this for initialization
	public GameObject goPath;
	public Path path;
	public float moveSpeed = 10.0f;
	
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
		
		Debug.Log( current );
		Debug.Log( transform.position );
	
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
}
