using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LeverController : MonoBehaviour
{
    private bool isPlayerNear = false;
    private bool isLeverActivated = false;
    private Quaternion initialRotation;

    public GameObject Door1;
    public GameObject Door2;
    public GameObject Door3;

    public Vector3 newPosDoor1On;
    public Vector3 newPosDoor1Off;

    public Vector3 newPosDoor2On;
    public Vector3 newPosDoor2Off;

    public Vector3 newPosDoor3On;
    public Vector3 newPosDoor3Off;

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

        Door1.transform.position = isLeverActivated ? newPosDoor1On : newPosDoor1Off;
        Door2.transform.position = isLeverActivated ? newPosDoor2On : newPosDoor2Off;
        Door3.transform.position = isLeverActivated ? newPosDoor3On : newPosDoor3Off;
    }
}
