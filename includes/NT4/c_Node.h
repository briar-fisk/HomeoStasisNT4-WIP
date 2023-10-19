//The state is stored as uint64_t so you will need to typecast to appropriate datatype.
class c_Node
{
public:

	//The Node ID, used because the raw address may change upon saving/loading and external hooks need a solid grasp.
	uint64_t NID;

	//The next node in the network.
	c_Node* Next;

	//Axons and the counters. Axons_F are the ones connected to the first leg, used in finding UTN
	c_Node** Axons_F;
	c_Node** Axons;
	int Axon_Count_F;
	int Axon_Count;

	//Dendrites, lower connections, leg order is of highest importance.
	c_Node** Dendrites;
	int Dendrite_Count;

	//The reinforcement counter.
    //A filter function will be built into the c_Neural_Network that applies:
    // exponential growth to reduce new trace impact, one offs and flukes get filtered
    // Sigmoid curve for the top end to prevent extreme bias in highly reinforced nodes.
	float RC;

	//The current charge of the node.
	float Current_Charge;

	//The quanta bound to the node.
    uint64_t State;

	c_Node()
	{
        NID = 0;

        //The next node.
        Next = NULL;

        //Axons and their counters.
        Axons_F = NULL;
        Axon_Count_F = 0;

        Axons = NULL;
        Axon_Count = 0;

        //Dendrites.
        Dendrites = NULL;
		Dendrite_Count = 0;

        //Reinforcement Counters.
        RC = 1.0;

        //The nodes state.
        State = 0;

		//No charge atm.
        Current_Charge = 0.0;
	}


    //Adds an axon on the first leg.
    void add_Axon_F(c_Node* p_Axon)
    {
        c_Node** tmp_Axons;
        tmp_Axons = new c_Node * [Axon_Count_F];
        for (int cou_A = 0; cou_A < Axon_Count_F; cou_A++)
        {
            tmp_Axons[cou_A] = Axons_F[cou_A];
            Axons_F[cou_A] = NULL;
        }

        delete[] Axons_F;
        Axons_F = NULL;

        Axons_F = new c_Node * [Axon_Count_F + 1];

        for (int cou_A = 0; cou_A < Axon_Count_F; cou_A++)
        {
            Axons_F[cou_A] = tmp_Axons[cou_A];
            tmp_Axons[cou_A] = NULL;
        }
        delete[] tmp_Axons;
        tmp_Axons = NULL;

        Axons_F[Axon_Count_F] = p_Axon;
        Axon_Count_F++;
    }

    //Adds an axon to the axon list.
    void add_Axon(c_Node* p_Axon)
    {
        c_Node** tmp_Axons;
        tmp_Axons = new c_Node * [Axon_Count];
        for (int cou_A = 0; cou_A < Axon_Count; cou_A++)
        {
            tmp_Axons[cou_A] = Axons[cou_A];
            Axons[cou_A] = NULL;
        }

        delete[] Axons;
        Axons = NULL;

        Axons = new c_Node * [Axon_Count + 1];

        for (int cou_A = 0; cou_A < Axon_Count; cou_A++)
        {
            Axons[cou_A] = tmp_Axons[cou_A];
            tmp_Axons[cou_A] = NULL;
        }
        delete[] tmp_Axons;
        tmp_Axons = NULL;

        Axons[Axon_Count] = p_Axon;
        Axon_Count++;
    }

    //Sets the dendrites of the node.
    //This assumes the node has no dendrites yet, if it does you be dangling and jangling
    void set_Dendrites(c_Node** p_Dendrites, int p_Count)
    {
        Dendrites = new c_Node * [p_Count];

        for (int cou_D = 0; cou_D < p_Count; cou_D++)
        {
            Dendrites[cou_D] = p_Dendrites[cou_D];
        }

        Dendrite_Count = p_Count;
    }

    //Searches the axons to see if an upper tier connection exists.
    //This is always called from the first leg, that is why we separate _F from normal.
    c_Node* does_Upper_Tier_Connection_Exist(c_Node** p_Nodes, int p_Count)
    {
        std::cout << "\n Axon_Count_F: " << Axon_Count_F;

        //Check the axons on the right side for the 
        for (int cou_A = 0; cou_A < Axon_Count_F; cou_A++)
        {
            std::cout << "\n   Axon_F[" << cou_A << "] " << Axons_F[cou_A];
            if (Axons_F[cou_A]->does_Lower_Connection_Exist(p_Nodes, p_Count))
            {
                std::cout << "\n >>>>>>>>>>>>>Foundit: " << Axons_F[cou_A];
                return Axons_F[cou_A];
            }
        }
        return NULL;
    }

    //Checks if the given node matches a dendrite on the right leg.
    bool does_Lower_Connection_Exist(c_Node ** p_Nodes, int p_Count)
    {
        if (Dendrite_Count != p_Count) { return 0; }

        //Check the lower connections to see if they match.
        for (int cou_C = 1; cou_C < p_Count; cou_C++)
        {
            if (Dendrites[cou_C] != p_Nodes[cou_C]) { return 0; }
        }
        return 1;
    }

    //Binds a node to a quanta of data, the state of the input.
    void bind_State(uint64_t p_State)
    {
        State = p_State;
    }

    //Initiates a backpropagation that 
    void bp_O()
    {
        std::cout << "<=- ";
        //If a left leg exists then initiate a backpropagation along it, then along the right side.
        if (Dendrite_Count != 0)
        {
            if (Dendrites[0] != NULL) { Dendrites[0]->bp_F(); }
            for (int cou_D = 1; cou_D < Dendrite_Count; cou_D++)
            {
                if (Dendrites[cou_D] != NULL)
                {
                    Dendrites[cou_D]->bp_M();
                }
            }
        }
        else
        {
            //Output the state
            std::cout << " <" << State << "> ";
        }
        std::cout << " -=>";
    }

    //bp_Output the left node.
    void bp_F()
    {
        //If a left leg exists then initiate a backpropagation along it, then along the right side.
        if (Dendrite_Count != 0)
        {
            if (Dendrites[0] != NULL) { Dendrites[0]->bp_F(); }
            for (int cou_D = 1; cou_D < Dendrite_Count; cou_D++)
            {
                if (Dendrites[cou_D] != NULL) 
                {
                    Dendrites[cou_D]->bp_M();
                }
            }
        }
        else
        {
            //Output the state
            std::cout << " <" << State << "> ";
        }
    }

    //bp_Output the other nodes, M stands for many.
    void bp_M()
    {
        //If a left leg exists then initiate a backpropagation along it, then along the right side.
        if (Dendrite_Count != 0)
        {
            for (int cou_D = 1; cou_D < Dendrite_Count; cou_D++)
            {
                if (Dendrites[cou_D] != NULL)
                {
                    Dendrites[cou_D]->bp_M();
                }
            }
        }
        else
        {
            //Output the state
            std::cout << " <" << State << "> ";
        }
    }


    void output_Node_Raw()
    {
        std::cout << "\n\n --== Node " << NID << " == --";
        std::cout << "\n State { " << State << " }";
        std::cout << "\n Dendrites: ";
        for (int cou_D = 0; cou_D < Dendrite_Count; cou_D++)
        {
            std::cout << "\n --[" << cou_D << "] " << Dendrites[cou_D];
        }

        std::cout << "\n Axons_F: ";
        for (int cou_A = 0; cou_A < Axon_Count_F; cou_A++)
        {
            std::cout << "\n --[" << cou_A << "] " << Axons_F[cou_A];
        }

        std::cout << "\n Axons: ";
        for (int cou_A = 0; cou_A < Axon_Count; cou_A++)
        {
            std::cout << "\n --[" << cou_A << "] " << Axons[cou_A];
        }
    }

    void output_Node_Char()
    {
        std::cout << char(this);
    }
};