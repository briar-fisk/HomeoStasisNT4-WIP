// HomeoNetWIP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "includes/NT4.h"

int main()
{
    NT4::c_Node ** tst_Node;
    int tst_Node_Count = 10;

    tst_Node = new NT4::c_Node*[tst_Node_Count];

    for (int cou_Index = 0; cou_Index < tst_Node_Count; cou_Index++)
    {
        tst_Node[cou_Index] = new NT4::c_Node;
    }


    tst_Node[4]->set_Dendrites(tst_Node, 4);

    tst_Node[0]->add_Axon_F(tst_Node[4]);
    for (int cou_Index = 0; cou_Index < 3; cou_Index++)
    {
        tst_Node[cou_Index + 1]->add_Axon(tst_Node[4]);
    }

    tst_Node[5]->set_Dendrites(tst_Node, 3);
    tst_Node[0]->add_Axon_F(tst_Node[5]);
    for (int cou_Index = 0; cou_Index < 2; cou_Index++)
    {
        tst_Node[cou_Index + 1]->add_Axon(tst_Node[4]);
    }

    tst_Node[6]->set_Dendrites(tst_Node, 2);
    tst_Node[0]->add_Axon_F(tst_Node[6]);
    for (int cou_Index = 0; cou_Index < 1; cou_Index++)
    {
        tst_Node[cou_Index + 1]->add_Axon(tst_Node[4]);
    } 

    for (int cou_Index = 0; cou_Index < tst_Node_Count; cou_Index++)
    {
        std::cout << "\n\n\n tst_Node[" << cou_Index << "] " << tst_Node[cou_Index];
        tst_Node[cou_Index]->output_Node_Raw();
    }
    std::cout << "\n\n\n";

    for (int cou_Index = 0; cou_Index < tst_Node_Count; cou_Index++)
    {
        std::cout << "\n tst_Node[" << cou_Index << "] " << tst_Node[cou_Index];
        std::cout << " DLCE: " << tst_Node[cou_Index]->does_Lower_Connection_Exist(tst_Node, 4);
    }
    std::cout << "\n\n\n";

    for (int cou_Index = 0; cou_Index < tst_Node_Count; cou_Index++)
    {
        std::cout << "\n tst_Node[" << cou_Index << "] " << tst_Node[cou_Index];
        std::cout << " DLCE: " << tst_Node[cou_Index]->does_Lower_Connection_Exist(tst_Node, 3);
    }
    std::cout << "\n\n\n";

    for (int cou_Index = 0; cou_Index < tst_Node_Count; cou_Index++)
    {
        std::cout << "\n tst_Node[" << cou_Index << "] " << tst_Node[cou_Index];
        std::cout << " DLCE: " << tst_Node[cou_Index]->does_Lower_Connection_Exist(tst_Node, 2);
    }
    std::cout << "\n\n\n";







    for (int cou_Index = 0; cou_Index < tst_Node_Count; cou_Index++)
    {
        std::cout << "\n tst_Node[" << cou_Index << "] " << tst_Node[cou_Index];
        std::cout << " DUPTE: " << tst_Node[cou_Index]->does_Upper_Tier_Connection_Exist(tst_Node, 2);
    }
    std::cout << "\n\n\n";

    for (int cou_Index = 0; cou_Index < tst_Node_Count; cou_Index++)
    {
        std::cout << "\n tst_Node[" << cou_Index << "] " << tst_Node[cou_Index];
        std::cout << " DUPTE: " << tst_Node[cou_Index]->does_Upper_Tier_Connection_Exist(tst_Node, 3);
    }
    std::cout << "\n\n\n";

    for (int cou_Index = 0; cou_Index < tst_Node_Count; cou_Index++)
    {
        std::cout << "\n tst_Node[" << cou_Index << "] " << tst_Node[cou_Index];
        std::cout << " DUPTE: " << tst_Node[cou_Index]->does_Upper_Tier_Connection_Exist(tst_Node, 4);
    }
    std::cout << "\n\n\n";

    for (int cou_Index = 0; cou_Index < tst_Node_Count; cou_Index++)
    {
        std::cout << "\n tst_Node[" << cou_Index << "] " << tst_Node[cou_Index];
        std::cout << " DUPTE: " << tst_Node[cou_Index]->does_Upper_Tier_Connection_Exist(tst_Node, 5);
    }
    std::cout << "\n\n\n";



    tst_Node[0]->bind_State(25);
    tst_Node[1]->bind_State(50);
    tst_Node[2]->bind_State(75);

    tst_Node[7]->set_Dendrites(&tst_Node[0], 2);
    tst_Node[8]->set_Dendrites(&tst_Node[1], 2);
    tst_Node[9]->set_Dendrites(&tst_Node[7], 2);

    tst_Node[9]->bp_O();

    NT4::c_Node_Network Testernet;
    Testernet.new_Node();
    Testernet.new_Node();
    Testernet.new_Node();
    Testernet.new_Node();
    Testernet.new_Node();
    Testernet.new_Node();
    Testernet.new_Node();

    std::cout << "\n\n\n CONSTRUCT: " << Testernet.register_New_Construct();

    for (int cou_Index = 0; cou_Index < 10; cou_Index++)
    {
        Testernet.new_State_Node(0, (rand() % 100));
        Testernet.new_State_Node(1, (rand() % 100));
    }


    Testernet.Nodes.output_Tree();
    Testernet.State_Nodes[0]->output_Tree();
    Testernet.State_Nodes[1]->output_Tree();

    tst_Node[0] = Testernet.new_Node();
    tst_Node[1] = Testernet.new_Node();
    tst_Node[2] = Testernet.new_Node();
    tst_Node[3] = Testernet.new_Node();
    tst_Node[4] = Testernet.new_Node();

    std::cout << "\n\n\n\n\n";
    Testernet.Nodes.output_Tree();

    tst_Node[5] = Testernet.get_Upper_Tier_Node(tst_Node, 5);
    std::cout << "\n\n GUTN[0]: " << tst_Node[5] << "\n\n";
    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        tst_Node[cou_Index]->output_Node_Raw();
    }

    tst_Node[5] = Testernet.get_Upper_Tier_Node(tst_Node, 5);
    std::cout << "\n\n GUTN[1]: " << tst_Node[5] << "\n\n";
    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        tst_Node[cou_Index]->output_Node_Raw();
    }

    tst_Node[6] = tst_Node[0];
    tst_Node[0] = Testernet.new_Node();

    tst_Node[5] = Testernet.get_Upper_Tier_Node(tst_Node, 5);
    std::cout << "\n\n GUTN[2]: " << tst_Node[5] << "\n\n";
    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        tst_Node[cou_Index]->output_Node_Raw();
    }
    

    tst_Node[0] = tst_Node[6];

    tst_Node[5] = Testernet.get_Upper_Tier_Node(tst_Node, 5);
    std::cout << "\n\n GUTN[3]: " << tst_Node[5] << "\n\n";
    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        tst_Node[cou_Index]->output_Node_Raw();
    }

    //Testernet.Nodes.output_Tree();


    tst_Node[5] = Testernet.get_Upper_Tier_Node(tst_Node, 5);
    std::cout << "\n\n GUTN[4]: " << tst_Node[5] << "\n\n";
    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        tst_Node[cou_Index]->output_Node_Raw();
    }


    tst_Node[5] = Testernet.get_Upper_Tier_Node(tst_Node, 4);
    std::cout << "\n\n GUTN[5]: " << tst_Node[5] << "\n\n";
    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        tst_Node[cou_Index]->output_Node_Raw();
    }


    tst_Node[5] = Testernet.get_Upper_Tier_Node(tst_Node, 5);
    std::cout << "\n\n GUTN[6]: " << tst_Node[5] << "\n\n";
    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        tst_Node[cou_Index]->output_Node_Raw();
    }


    tst_Node[5] = Testernet.get_Upper_Tier_Node(tst_Node, 4);
    std::cout << "\n\n GUTN[5]: " << tst_Node[5] << "\n\n";
    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        tst_Node[cou_Index]->output_Node_Raw();
    }


    NT4::c_Node * tmp_SNode = NULL;

    tmp_SNode = Testernet.new_State_Node(1, 123);
    std::cout << "\n\n\n DSNE: 123 " << Testernet.does_State_Node_Exist(1, 123);
    std::cout << "\n  DSNE: 13 " << Testernet.does_State_Node_Exist(1, 13);
    std::cout << "\n  DSNE 23 " << Testernet.does_State_Node_Exist(1, 23);
    std::cout << "\n  DSNE 123 " << Testernet.does_State_Node_Exist(1, 123);
    std::cout << "\n\n\n\n";
    std::cout << "\n\n\n GSN: 123 " << Testernet.get_State_Node(1, 123);
    std::cout << "\n  GSN: 13 " << Testernet.get_State_Node(1, 13);
    std::cout << "\n  GSN 23 " << Testernet.get_State_Node(1, 23);
    std::cout << "\n  GSN 123 " << Testernet.get_State_Node(1, 123);
    std::cout << "\n\n\n\n";
    std::cout << "\n\n\n DSNE: 123 " << Testernet.does_State_Node_Exist(1, 123);
    std::cout << "\n  DSNE: 13 " << Testernet.does_State_Node_Exist(1, 13);
    std::cout << "\n  DSNE 23 " << Testernet.does_State_Node_Exist(1, 23);
    std::cout << "\n  DSNE 123 " << Testernet.does_State_Node_Exist(1, 123);
    std::cout << "\n\n\n\n";

     
    NT4::c_CAN_Many_To_One TesterCAN;
    TesterCAN.set_NNet(&Testernet);

    uint64_t tmp_CANput[5] = { 1, 2, 4, 6, 10 };

    TesterCAN.set_Input(tmp_CANput, 5);
    TesterCAN.output_Input();

    TesterCAN.setup_CAN_Scaffold();
    TesterCAN.fill_State();

    TesterCAN.output_Scaffold(); 

    tmp_CANput[1] = 99;
    tmp_CANput[2] = 99;
    tmp_CANput[3] = 95;

    TesterCAN.set_Input(tmp_CANput, 5);
    TesterCAN.output_Input();

    TesterCAN.setup_CAN_Scaffold();
    TesterCAN.fill_State();

    TesterCAN.output_Scaffold();

    tmp_CANput[1] = 2;
    tmp_CANput[2] = 4;
    tmp_CANput[3] = 95;

    TesterCAN.set_Input(tmp_CANput, 5);
    TesterCAN.output_Input();

    TesterCAN.setup_CAN_Scaffold();
    TesterCAN.fill_State();

    TesterCAN.output_Scaffold();

    tmp_CANput[1] = 2;
    tmp_CANput[2] = 4;
    tmp_CANput[3] = 6;

    TesterCAN.set_Input(tmp_CANput, 5);
    TesterCAN.output_Input();

    TesterCAN.setup_CAN_Scaffold();
    TesterCAN.fill_State();

    TesterCAN.output_Scaffold();

    std::cout << "\n\n\n\n\n testin scaffold -----------------------------------------------------------------------\n\n";

    TesterCAN.encode(tmp_CANput, 5);

    tmp_CANput[1] = 2;
    tmp_CANput[2] = 4;
    tmp_CANput[3] = 95;

    TesterCAN.encode(tmp_CANput, 5);

    tmp_CANput[1] = 2;
    tmp_CANput[2] = 4;
    tmp_CANput[3] = 95;

    TesterCAN.encode(tmp_CANput, 5);

    tmp_CANput[1] = 99;
    tmp_CANput[2] = 99;
    tmp_CANput[3] = 95;

    TesterCAN.encode(tmp_CANput, 5);

    tmp_CANput[1] = 2;
    tmp_CANput[2] = 4;
    tmp_CANput[3] = 6;

    TesterCAN.encode(tmp_CANput, 5);

    tmp_CANput[1] = 2;
    tmp_CANput[2] = 4;
    tmp_CANput[3] = 95;

    TesterCAN.encode(tmp_CANput, 5);

    tmp_CANput[1] = 99;
    tmp_CANput[2] = 99;
    tmp_CANput[3] = 95;

    TesterCAN.encode(tmp_CANput, 5);
}