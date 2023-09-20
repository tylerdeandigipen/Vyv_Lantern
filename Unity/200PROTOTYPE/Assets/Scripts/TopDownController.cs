using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TopDownController : MonoBehaviour
{

    public KeyCode InteractKey = KeyCode.E;
    public float InteractionRadius = 1.5f;
    //camera follows player
    public Camera Cam;

    //player movement speed
    public float MoveSpeed = 5f;
    
    //Add player's rigidbody component
    private Rigidbody2D RB;
    
    // Start is called before the first frame update
    void Start()
    {
        //get the rigidbody component
        RB = GetComponent<Rigidbody2D>();      
    }

    // Update is called once per frame
    void Update()
    {
        //Applying funtions
        ApplyMovement();
        ApplyRotations();        
    }

    public void ApplyRotations()
    {
        //The player character rotates on z axis based on the mouse position
        Vector3 worldMousePos = Cam.ScreenToWorldPoint(Input.mousePosition);
        Vector3 Target = new Vector3(worldMousePos.x, worldMousePos.y, 0f);
        transform.right = (Target - transform.position).normalized;
    }

    public void ApplyMovement()
    {
        //Basic player movement (Up, Left, Right, Down)
        Vector3 dir = Vector3.zero;

        if (Input.GetKey(KeyCode.W))
            dir += Vector3.up;

        if (Input.GetKey(KeyCode.A))
            dir += Vector3.left;

        if (Input.GetKey(KeyCode.S))
            dir += Vector3.down;

        if (Input.GetKey(KeyCode.D))
            dir += Vector3.right;

        dir = dir.normalized;
        RB.velocity = dir * MoveSpeed;
    }

    
}
