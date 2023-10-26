// HomeoNetWIP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "includes/NT4.h"
#include "includes/HomeoStasis.h"


//Each input in the homeostasis module input array is given a granulator that can be configured individually.
class c_Homeostasis_Module
{
public:

    //This will hold the shared neural network.
    NT4::c_Node_Network Nodes;

    //We've flattened the lowest tiers, instead of raw constructs being built into a multi-sensory-construct (MSC) we instead directly encode all the raw into an MSC through a many-to-one CAN scaffold.
    NT4::c_CAN_Many_To_One Raw_MSC;

    //The count.
    int Afferent_Count; //Variables dependent on this node: Afferent[]
    int Efferent_Count; //Variables dependent on this node: Efferent[]

    //One input for each index.
    c_Afferent_Input** Afferent;
    c_Efferent_Input_Output** Efferent;

    //The array to hold the gathered input, and the count of elements in it.
    uint64_t* Gathered_Afferent[3]; //[0]: Concrete, [1]: Granulated, [2]: Delta
    uint64_t * Gathered_Efferent; //This is separate because it can have a different depth than the afferent one.

    c_Homeostasis_Module()
    {
        Raw_MSC.set_NNet(&Nodes);

        Afferent_Count = 1;
        Efferent_Count = 1;

        Afferent = new c_Afferent_Input * [1]; Afferent[0] = new c_Afferent_Input;
        Efferent = new c_Efferent_Input_Output * [1]; Efferent[0] = new c_Efferent_Input_Output;

        //For afferent and efferent 1 each. The depth of this is based on (Afferent_Count + Efferent_Count)
        Gathered_Afferent[0] = new uint64_t[1];
        Gathered_Afferent[1] = new uint64_t[1];
        Gathered_Afferent[2] = new uint64_t[1];
        Gathered_Efferent = new uint64_t[1];

        std::cout << "\n A: " << Afferent_Count << " E: " << Efferent_Count;
    }

    //Register Afferent input. The module always starts with 1.
    int register_Afferent()
    {
        //Declare the tmp arrays.
        c_Afferent_Input ** tmp_A;
        tmp_A = new c_Afferent_Input * [Afferent_Count];

        //Swap the indices to the tmp array.
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            tmp_A[cou_A] = Afferent[cou_A]; Afferent[cou_A] = NULL;
        }

        //Delete and redeclare the real arrays.
        delete[] Afferent; Afferent = NULL;

        //Redeclare the OG arrays.
        Afferent = new c_Afferent_Input*[Afferent_Count + 1];

        //create the new indices.
        Afferent[Afferent_Count] = new c_Afferent_Input;

        //Recover the old data.
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            Afferent[cou_A] = tmp_A[cou_A]; tmp_A[cou_A] = NULL;
        }

        //Cleanup tmp vars
        //Delete and redeclare the real arrays.
        delete[] tmp_A; tmp_A = NULL;

        Afferent_Count++;

        return (Afferent_Count - 1);
    }

    //Register Efferent input/output.
    int register_Efferent()
    {
        //Declare the tmp arrays.
        c_Efferent_Input_Output** tmp_E;
        tmp_E = new c_Efferent_Input_Output * [Efferent_Count];

        //Swap the indices to the tmp array.
        for (int cou_E = 0; cou_E < Efferent_Count; cou_E++)
        {
            tmp_E[cou_E] = Efferent[cou_E]; Efferent[cou_E] = NULL;
        }

        //Delete and redeclare the real arrays.
        delete[] Efferent; Efferent = NULL;

        //Redeclare the OG arrays.
        Efferent = new c_Efferent_Input_Output * [Efferent_Count + 1];

        //NULL the new indices.
        Efferent[Efferent_Count] = new c_Efferent_Input_Output;

        //Recover the old data.
        for (int cou_E = 0; cou_E < Efferent_Count; cou_E++)
        {
            Efferent[cou_E] = tmp_E[cou_E]; tmp_E[cou_E] = NULL;
        }

        //Cleanup tmp vars
        //Delete and redeclare the real arrays.
        delete[] tmp_E; tmp_E = NULL;

        Efferent_Count++;

        return (Efferent_Count - 1);
    }

    //Accepts an input and updates the correct index with it
    void add_Afferent_Granulation(double p_Bottom, double p_Top, int p_Index = -1) //p_Index comes after since it may be 0
    {
        if (p_Index == -1)
        {
            p_Index = Afferent_Count - 1;
        }

        Afferent[p_Index]->add_Granulation(p_Bottom, p_Top);
    }

    //Exposes the set_Depth() so the user can access it through a function.
    void set_Depth(int p_Depth)
    {
        //For every Afferent and Efferent we set the depth.
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            Afferent[cou_A]->set_Depth(p_Depth);
        }
        for (int cou_E = 0; cou_E < Efferent_Count; cou_E++)
        {
            Efferent[cou_E]->set_Depth(p_Depth);
        }
    }

    //Exposes the set_Value() so the user can access it through a function.
    //Assumes p_Index has been registered in the Afferent array already.
    void set_Afferent_Value(int p_Index, double p_Value)
    {
        Afferent[p_Index]->set_Value(p_Value);
    }

    //Exposes the set_Value() so the user can access it through a function.
    //Assumes p_Index has been registered in the Afferent array already.
    void set_Efferent_Value(int p_Index, double p_Value)
    {
        Efferent[p_Index]->set_Value(p_Value);
    }

    //Exposes the set_Value() so the user can access it through a function.
    void shift_Data()
    {
        //For every Afferent and Efferent we shift the data.
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            Afferent[cou_A]->shift_Data();
        }
        for (int cou_E = 0; cou_E < Efferent_Count; cou_E++)
        {
            Efferent[cou_E]->shift_Data();
        }
    }

    //This is done whenever an afferent or efferent is registered.
    void resize_Gathered_Input()
    {
        delete[] Gathered_Afferent[0]; Gathered_Afferent[0] = NULL;
        delete[] Gathered_Afferent[1]; Gathered_Afferent[1] = NULL;
        delete[] Gathered_Afferent[2]; Gathered_Afferent[2] = NULL;
        delete[] Gathered_Efferent; Gathered_Efferent = NULL;

        Gathered_Afferent[0] = new uint64_t[Afferent_Count];
        Gathered_Afferent[1] = new uint64_t[Afferent_Count];
        Gathered_Afferent[2] = new uint64_t[Afferent_Count];
        Gathered_Efferent = new uint64_t[Efferent_Count];

        for (int cou_Index = 0; cou_Index < Afferent_Count; cou_Index++)
        {
            Gathered_Afferent[0][cou_Index] = 0;
            Gathered_Afferent[1][cou_Index] = 0;
            Gathered_Afferent[2][cou_Index] = 0;
            Gathered_Efferent[cou_Index] = 0;
        }
    }

    //Gathers the input set.
    void gather_Input()
    {
        for (int cou_Index = 0; cou_Index < Afferent_Count; cou_Index++)
        {
            Gathered_Afferent[0][cou_Index] = Afferent[cou_Index]->get_Value_Data();
            Gathered_Afferent[1][cou_Index] = Afferent[cou_Index]->get_Value_Granulated();
            Gathered_Afferent[2][cou_Index] = Afferent[cou_Index]->get_Value_Delta();
        }
        for (int cou_Index = 0; cou_Index < Afferent_Count; cou_Index++)
        {
            Gathered_Efferent[cou_Index] = Efferent[cou_Index]->get_Value_Data();
        } 
    }

    //Encodes an input set.
    void encode()
    {
        //Gather the Input set
        gather_Input();

        //Outputs the gathered Inputs
        void output_Gathered();

        //Encodes the Input.
        //Raw_MSC.encode(Gathered_Input, (Afferent_Count + Efferent_Count));
    }

    //Exposes the output_IO() so the user can access it through a function.
    void output_AE()
    {
        //For every Afferent and Efferent we shift the data.
        std::cout << "\n ---Afferent_Count: " << Afferent_Count;
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            std::cout << "\n    [" << cou_A << "]";
            Afferent[cou_A]->output();
        }
        std::cout << "\n ---Efferent_Count: " << Efferent_Count;
        for (int cou_E = 0; cou_E < Efferent_Count; cou_E++)
        {
            std::cout << "\n    [" << cou_E << "]";
            Efferent[cou_E]->output();
        }
    }

    //Outputs the gathered Inputs
    void output_Gathered()
    {
        for (int cou_Index = 0; cou_Index < Afferent_Count; cou_Index++)
        {
            std::cout << "\n [0] " << Gathered_Afferent[0][cou_Index];
            std::cout << "\n [1] " << Gathered_Afferent[1][cou_Index];
            std::cout << "\n [2] " << Gathered_Afferent[2][cou_Index];
        }
        for (int cou_Index = 0; cou_Index < Efferent_Count; cou_Index++)
        {
            std::cout << "\n E " << Gathered_Efferent[cou_Index];
        }
    }
};

int main()
{
    /*
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

    c_Granulator Grangran;

    Grangran.add_Var(74.9, 75.1);
    Grangran.add_Var(65, 80);
    Grangran.add_Var(50, 95);
    Grangran.add_Var(25, 99);
    Grangran.add_Var(0, 100);

    for (int cou_Index = 0; cou_Index < 100; cou_Index++)
    {
        double tmp_Val = rand() % 100;
        std::cout << "\n\n Gran [" << tmp_Val << "] = " << Grangran.get_Granulated(tmp_Val);
    }
    std::cout << "\n\n Gran [" << 75 << "] = " << Grangran.get_Granulated(75);


    c_Afferent_Input Testerin[5];
    

    for (int cou_Index = 0; cou_Index < 5; cou_Index++)
    {
        Testerin[cou_Index].add_Granulation(74.9, 75.1);
        Testerin[cou_Index].add_Granulation(65, 80);
        Testerin[cou_Index].add_Granulation(50, 95);
        Testerin[cou_Index].add_Granulation(25, 99);
        Testerin[cou_Index].add_Granulation(0, 100);
        Testerin[cou_Index].set_Depth(5);
    }

    for (int cou_Run = 0; cou_Run < 10; cou_Run++)
    {
        std::cout << "\n\n   --<< Run " << cou_Run << " / 100 >>--";
        for (int cou_Index = 0; cou_Index < 5; cou_Index++)
        {
            Testerin[cou_Index].shift_Data();

            double tmp_Val = rand() % 100;
            Testerin[cou_Index].set_Value(tmp_Val);
            std::cout << "\n\n Input Index <" << cou_Index << "> Temp Value: " << tmp_Val;
            std::cout << "\n -----c_Afferent_Input-----";
            Testerin[cou_Index].output();
        }

    }
    */
    c_Homeostasis_Module ProtoGaia;
    int p_Input_Depth = 10;

    std::cout << "\n <a0< " << ProtoGaia.register_Afferent();
    std::cout << "\n <a1< " << ProtoGaia.register_Afferent();
    std::cout << "\n <a2< " << ProtoGaia.register_Afferent();
    std::cout << "\n <a3< " << ProtoGaia.register_Afferent();
    std::cout << "\n <a4< " << ProtoGaia.register_Afferent();

    std::cout << "\n <b5< " << ProtoGaia.register_Efferent();
    std::cout << "\n <b6< " << ProtoGaia.register_Efferent();
    std::cout << "\n <b7< " << ProtoGaia.register_Efferent();
    std::cout << "\n <b8< " << ProtoGaia.register_Efferent();
    std::cout << "\n <b9< " << ProtoGaia.register_Efferent();
    


    ProtoGaia.set_Depth(6);


    for (int cou_Index = 0; cou_Index < 6; cou_Index++)
    {
        ProtoGaia.add_Afferent_Granulation(74.9, 75.1, cou_Index);
        ProtoGaia.add_Afferent_Granulation(65, 80, cou_Index);
        ProtoGaia.add_Afferent_Granulation(50, 95, cou_Index);
        ProtoGaia.add_Afferent_Granulation(25, 99, cou_Index);
        ProtoGaia.add_Afferent_Granulation(0, 100, cou_Index);
    }

    for (int cou_Index = 0; cou_Index < 25; cou_Index++)
    {
        ProtoGaia.shift_Data();

        ProtoGaia.set_Afferent_Value(0, (cou_Index + cou_Index));
        ProtoGaia.set_Afferent_Value(1, (cou_Index + cou_Index));
        ProtoGaia.set_Afferent_Value(2, (rand() % 100));
        ProtoGaia.set_Afferent_Value(3, (cou_Index + cou_Index));
        ProtoGaia.set_Afferent_Value(4, (rand() % 100));
        ProtoGaia.set_Afferent_Value(5, (rand() % 100));

        ProtoGaia.set_Efferent_Value(0, (cou_Index + cou_Index));
        ProtoGaia.set_Efferent_Value(1, (cou_Index + cou_Index));
        ProtoGaia.set_Efferent_Value(2, (cou_Index + cou_Index));
        ProtoGaia.set_Efferent_Value(3, (cou_Index + cou_Index));
        ProtoGaia.set_Efferent_Value(4, (cou_Index + cou_Index));
        ProtoGaia.set_Efferent_Value(5, (cou_Index + cou_Index));

        ProtoGaia.output_AE();
    }


}