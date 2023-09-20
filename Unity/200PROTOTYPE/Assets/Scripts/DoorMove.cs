/*******************************************************************************
File: DoorMov.cs
Author: Zan Moffat
DP Email: zan.moffat@digipen.edu
Date: ??/??/2023
Course: CS165
Section: A
Description:
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus mattis dui
ipsum, vitae faucibus tortor porta sed. Cras vulputate nibh sed gravida
commodo. Donec euismod egestas pellentesque.
*******************************************************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DoorMove : MonoBehaviour
{
    public Vector3 onPosition;
    //public Vector3 defaultPosition;
    private bool onOff = false;
    public void Move()
    {
        Debug.Log("script called");
        if(onOff == false)
        {

            this.transform.position = transform.position + onPosition;
            onOff = true;
            return;
        }
        if(onOff == true)
        {
            this.transform.position = transform.position - onPosition;
            onOff = false;
            return;
        }
    }
}

