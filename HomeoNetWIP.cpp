// HomeoNetWIP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "includes/NT4.h"
#include "includes/HomeoStasis.h"

//This class serves as the user interface for I/O.
class c_IO
{
private:

    //Data is stored internally as uint64_t.
    uint64_t * Data;

    //The datatype to present the user. 
    int Type;


};

//Each input in the homeostasis module input array is given a granulator that can be configured individually.
class c_Homeostasis_Module
{
public:

    //This will hold the shared neural network.
    NT4::c_Node_Network Nodes;

    //We've flattened the lowest tiers, instead of raw constructs being built into a multi-sensory-construct (MSC) we instead directly encode all the raw into an MSC through a many-to-one CAN scaffold.
    NT4::c_CAN_Many_To_One Raw_MSC;



    //The count.
    int Input_Count; //Variables dependent on this node: Inputs[], Output[][], Granulator[]

    //One input for each index.


    //One granulator for each input index.
    c_Granulator** Granulator;

    c_Homeostasis_Module()
    {
        Raw_MSC.set_NNet(&Nodes);

        Granulator = NULL;

        Input_Count = 0;
    }

    //Registers an input goal with the network.
    int create_Input()
    {
        //If none yet exist then create the Granulator.
        if (Granulator == NULL)
        {
            Granulator = new c_Granulator*[1];
            Input_Count = 1;
        }
    }

    //Registers an input for feedback.
};

int main()
{
    c_Sim Simboi; //He is reborn.

    NT4::c_Node_Network Testernet;
     
    NT4::c_CAN_Many_To_One TesterCAN;
    TesterCAN.set_NNet(&Testernet);

    uint64_t tmp_CAN_Input[10];
    for (int cou_Index = 0; cou_Index < 10; cou_Index++)
    {
        tmp_CAN_Input[cou_Index] = rand() % 2; // (cou_Index * (rand() % 100));
    }

    TesterCAN.encode(tmp_CAN_Input, 10);

    tmp_CAN_Input[0] = !tmp_CAN_Input[0];
    tmp_CAN_Input[1] = !tmp_CAN_Input[1];

    TesterCAN.encode(tmp_CAN_Input, 10);
        tmp_CAN_Input[1] = (rand() % 2);

    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        tmp_CAN_Input[0] = (rand() % 2);

        TesterCAN.encode(tmp_CAN_Input, 10);
        tmp_CAN_Input[1] = (rand() % 2);
    }

    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        tmp_CAN_Input[0] = (rand() % 2);
        tmp_CAN_Input[1] = (rand() % 2);

        TesterCAN.query(tmp_CAN_Input, 10);
    }
}