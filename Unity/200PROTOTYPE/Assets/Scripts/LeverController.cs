using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LeverController : MonoBehaviour
{
    private bool isPlayerNear = false;
    private bool isLeverActivated = false;
    private Quaternion initialRotation;

    public DoorMove doorScript1;
    public DoorMove doorScript2;
    public DoorMove doorScript3;
    /*
     public Vector3 newPosDoor1On;
     public Vector3 newPosDoor1Off;

     public Vector3 newPosDoor2On;
     public Vector3 newPosDoor2Off;

     public Vector3 newPosDoor3On;
     public Vector3 newPosDoor3Off; */

    // Start is called before the first frame update
    void Start()
    {
        initialRotation = transform.rotation;    
    }

    // Update is called once per frame
    void Update()
    {
        if (isPlayerNear)
        {
            if (Input.GetKeyDown(KeyCode.E))
            {
                ToggleLever();
            }
        }
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if (other.CompareTag("Player"))
        {
            isPlayerNear = true;
        }
    }

    private void OnTriggerExit2D(Collider2D other)
    {
        if (other.CompareTag("Player"))
        {
            isPlayerNear = false;
        }
    }

    private void ToggleLever()
    {
        isLeverActivated = !isLeverActivated;

        if (isLeverActivated)
        {
            transform.rotation = Quaternion.Euler(180, 0, 46) ;
        }

        else
        {
            transform.rotation = initialRotation;
        }
        if(doorScript1 != null)
        {
            Debug.Log("door1");
            doorScript1.Move();
        }
        if (doorScript2 != null)
        {
            Debug.Log("door2");
            doorScript2.Move();
        }
        if (doorScript3 != null)
        {
            Debug.Log("door3");
            doorScript3.Move();
        }

    }
}
