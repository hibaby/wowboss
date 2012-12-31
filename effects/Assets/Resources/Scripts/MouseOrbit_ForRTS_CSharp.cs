using UnityEngine;
using System.Collections;

public class MouseOrbit_ForRTS_CSharp : MonoBehaviour {

	
	public float ZoomSpeed = 10;
	public float MovingSpeed = 0.5f;
	public float RotateSpeed = 1;  
	public float distance = 5;

	void Start () {
		//distance = Mathf.Abs(transform.position.y/Mathf.Sin(transform.eulerAngles.x));
	}

	void Update () {
		Quaternion rotation; 
		Vector3 position;
		float delta_x,delta_y,delta_z;
		float delta_rotation_x,delta_rotation_y;
	
		if(Input.GetMouseButton(0))
		{
			delta_x = Input.GetAxis("Mouse X") * MovingSpeed;
        	delta_y = Input.GetAxis("Mouse Y") * MovingSpeed;
 	    	rotation = Quaternion.Euler(0, transform.rotation.eulerAngles.y,0 );
          	transform.position =rotation * new Vector3(-delta_x,0,-delta_y)+transform.position;
  		}
  	
  
  		if(Input.GetAxis("Mouse ScrollWheel")!= 0){	
  			delta_z = -Input.GetAxis("Mouse ScrollWheel") * ZoomSpeed;
    		transform.Translate(0,0,-delta_z);
    		distance += delta_z;
    	}
    
  		if (Input.GetMouseButton(1)) {
    		delta_rotation_x = Input.GetAxis("Mouse X") * RotateSpeed;
        	delta_rotation_y = -Input.GetAxis("Mouse Y") * RotateSpeed;
       		position =transform.rotation* new Vector3(0,0,distance)+transform.position;
       		transform.Rotate(0,delta_rotation_x,0,Space.World);
       		transform.Rotate(delta_rotation_y,0,0);
       		transform.position =transform.rotation* new Vector3(0,0,-distance)+position;
 		}
	}
}
