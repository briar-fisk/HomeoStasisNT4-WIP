//The Current Active Node Scaffold for Trace Encoding (CAN) is the scaffold we use to iteratively encode traces.
//For example, a network with an I/O depth of 5 and a single tier w/ one 5 legged node may look like this:
// Memory_Tier[1]: [0] //The top node, represents one permutation of the input tier. Multiple Memory tiers.
// The memory tiers start at [1] since the State tier is technically Memory tier [0].
// State_Tier (0): [0] [1] [2] [3] [4] //This is the tier with the states. 
// Each [] represents a pointer to a node. We iterate from bottom to top, using get_State_Node,
// and get_Upper_Tier_Node() to fill it out tier by tier.

//--Not using a polymorphic base class here, different CANs may have wildly different features.
//--Once enough are written then we can go back and write one if need be.
//Scratch that, making a base class so that when an assembly is registered a CAN can (hue hue hue) be chosen and function as the interface.

//This is the base class for a CAN. It has the input, encode, and outputs.
class c_Base_CAN //Man
{
public:

	//The reference to the node network to query for nodes.
	c_Node_Network* NNet;

	//This is the index in the c_Node_Network::State_Nodes[] array to request from.
	int State_Nodes_Index;

	//The inputs for the CAN, they are uint64_t internally, the data is not modified so as long as the correct interface is used then none will be lost.
	uint64_t* Input;
	int Input_Depth;
	
	//These may vary wildly depending on the CAN structure, the scaffold will be completely internal.
	//Because of this we will declare the actual scaffold in the derived classes rather than here.
	//c_Node** Scaffold[2];
	//int State_Depth; //We track this so that if the input is changed we can still properly delete the scaffold.

	//This is the reference for leg connection order firing control. It is assumed that the number of axon hillocks won't exceed this count for any node called.
	bool* Leg_Firing_Order;
	
	//The leg count is used by the Leg_Firing_Order control members. 
	int Leg_Count;

	//==--   Member functions   --==//

	//This encodes the p_Input data, if the nodes aren't found they are created, used for training.
	virtual void encode(uint64_t* p_Input, int p_Depth)=0;

	//This sets up the scaffold as encode does, but it doesn't create nodes if they aren't found, they remain NULL in the scaffold, this we call a NULLCAN
	//Used for querying the network, you input, fill the NULLCAN, charge the network, then gather the outputs.
	//Suggested for use before encoding (if using learning mode and not locked_to_initial_training_mode) otherwise it will also find the current trace as the perfect match.
	virtual void query(uint64_t* p_Input, int p_Depth)=0;

	//This returns the treetop node at a given index, for most structures this will be a single node, but for those like stiched-base networks with a treetop node count equal to the input node count then you can access them by index.
	virtual c_Node* get_Treetop(int p_Index = 0)=0;

	//Wipe the input array.
	void reset_Input()
	{
		//Reset it if need be.
		if (Input != NULL)
		{
			delete[] Input;
			Input = NULL;
			Input_Depth = 0;
		}
	}

	//Associate the CAN with a network from which to draw nodes.
	void set_NNet(c_Node_Network* p_NNet)
	{
		NNet = p_NNet;
	}

	//Set the number of legs to fire and expand the array.
	void set_Leg_Count(int p_Count)
	{
		if (Leg_Firing_Order != NULL) { delete[] Leg_Firing_Order; Leg_Firing_Order = NULL; }

		Leg_Firing_Order = new bool[p_Count];

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			Leg_Firing_Order[cou_Index] = true;
		}

		Leg_Count = p_Count;
	}

	//It is assumed the number of legs is known as by calling this you are actively setting them to what you want.
	void set_Leg_Firing_Order(bool* Firing_Order)
	{
		for (int cou_Leg = 0; cou_Leg < Leg_Count; cou_Leg++)
		{
			Leg_Firing_Order[cou_Leg] = Firing_Order[cou_Leg];
		}
	}

	//Sets the index for the state_Node_Tree in the c_Node_Network::State_Nodes[]
	void set_State_Nodes_Index(int p_Index)
	{
		State_Nodes_Index = p_Index;
	}

	//Sets the input to the given uint64_t array.
	//The input array is 1D, but for more complex constructs dimensional index slicing is used.
	void set_Input(uint64_t* p_Input, int p_Input_Depth)
	{
		reset_Input();

		//Initialize the wiped input array and copy the data to it from the arguments.
		Input_Depth = p_Input_Depth;
		Input = new uint64_t[Input_Depth];
		for (int cou_Index = 0; cou_Index < Input_Depth; cou_Index++)
		{
			Input[cou_Index] = p_Input[cou_Index];
		}
	}

	//==--   Output Functions   --==//

	//Outputs the scaffold as addresses.
	virtual void output_Scaffold()=0;

	//The input set is output.
	virtual void output_Input()=0;

	//Each address is typecast to a char to give a pseudo-unique look to each node. For monke brain.
	virtual void output_Scaffold_Char()=0;
};

//This functions to create a trace where all legs are connected to one node.
//Load up the input, hit encode(), boom you single permutation of all legs bound to that node.
//Most useful when each index in the input is a separate state space from the other indexes, sensors == good, language == shared states across words !good
class c_CAN_Many_To_One: public c_Base_CAN
{ 
public:

	//Each CAN node is just a pointer to a node in the network.
	//For this CAN the scaffold is 2 tiers, the bottom being the state tier, and then one node on tier[1] for the treetop/symbol node.
	//So for this CAN we don't track the number of tiers, that is always 2, and the number of nodes is equal to the input depth for the state(quanta) tier, and 1 for the treetop.
	c_Node** Scaffold[2];
	int State_Depth; //We track this so that if the input is changed we can still properly delete the scaffold.

	c_CAN_Many_To_One()
	{
		NNet = NULL;

		Input = NULL;
		Input_Depth = 0;

		Scaffold[0] = NULL;
		State_Depth = 0;
		Scaffold[1] = new c_Node*[1];

		State_Depth = 0;
		State_Nodes_Index = 0;

		Leg_Firing_Order = NULL;
	}

	~c_CAN_Many_To_One()
	{
		NNet = NULL;
		reset_Scaffold();
		reset_Input();
	}

	//Resets the CAN to NULL, and deletes the state tier + treetop.
	void reset_Scaffold()
	{
		if (Scaffold[0] != NULL)
		{
			//Make sure to NULL the scaffold first.
			//DO NOT DELETE THEM, the addresses are for the node network, deleting them here will cause null pointer deletion in Node_Network.
			for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
			{
				Scaffold[0][cou_Index] = NULL;
			}
			delete[] Scaffold[0];
			Scaffold[0] = NULL;
		}
		Scaffold[1][0] = NULL;
	}

	//This sets up the actual CAN scaffold to use.
	//It assumes the input array is filled out, the size is based upon that array.
	void setup_CAN_Scaffold()
	{
		//Take it out back and put it down.
		reset_Scaffold();

		//Sizing her up!
		State_Depth = Input_Depth;

		//Setup the lowest tier of the scaffold so that every index in the input has a correlated state tier node position on the scaffold.
		Scaffold[0] = new c_Node * [State_Depth];

		//Initialize the scaffold.
		for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
		{
			Scaffold[0][cou_Index] = NULL;
		}
		Scaffold[1][0] = NULL;
	}

	//This fills the state tier by querying the node network.
	//p_How: "Encode" == Create the nodes if they aren't found, "Query" == Returns NULL if they aren't found, used for checking if something has been encoded without modifying the network.
	void fill_State(std::string p_How = "Encode")
	{
		if (p_How == "Encode")
		{
			for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
			{
				//Request the state node form the node network using get_State_Node so one is created if not found.
				//We have to make sure we request the correct state tree.
				Scaffold[0][cou_Index] = NNet->get_State_Node(0, Input[cou_Index]);

				//If the node is also a treetop then set it to state/treetop.
				if ((Scaffold[0][cou_Index]->Type == 3) || (Scaffold[0][cou_Index]->Type == 4))
				{
					Scaffold[0][cou_Index]->Type = 4;
				}
			}
		}
		if (p_How == "Query")
		{
			for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
			{
				//Request the state node form the node network using get_State_Node so one is created if not found.
				//We have to make sure we request the correct state tree.
				Scaffold[0][cou_Index] = NNet->does_State_Node_Exist(0, Input[cou_Index]);
			}
		}
	}

	//Just one node at the tippy top.
	//p_How: "Encode" == Create the nodes if they aren't found, "Query" == Returns NULL if they aren't found, used for checking if something has been encoded without modifying the network.
	void fill_Scaffold(std::string p_How = "Encode")
	{
		if (p_How == "Encode")
		{
			//We request a node that links the entire state tier together.
			Scaffold[1][0] = NNet->get_Upper_Tier_Node(Scaffold[0], State_Depth);

			Scaffold[1][0]->Type = 3;
		}
		if (p_How == "Query")
		{
			//We request a node that links the entire state tier together, but do not create them.
			Scaffold[1][0] = NNet->does_Upper_Tier_Connection_Exist(Scaffold[0], State_Depth);
		}

		for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
		{
			if (Scaffold[0][cou_Index] != NULL)
			{
				(Scaffold[0][cou_Index])->set_Leg_Firing_Order(&Leg_Firing_Order);
			}
		}
		if (Scaffold[1][0] != NULL)
		{
			(Scaffold[1][0])->set_Leg_Firing_Order(&Leg_Firing_Order);
		}
	}

	//Encodes a single trace, forcibly.
	void encode(uint64_t * p_Input, int p_Depth)
	{
		std::cout << "\n\n<<-- Begin Encoding -->>\n\n";

		//Firstly we gather the inputly
		set_Input(p_Input, p_Depth);

		//Set up the scaffold for the nodes to reside in as we build the trace.
		setup_CAN_Scaffold();

		//Work across the state tier to fill it out by requesting state nodes from the NNet, if not found they are created.
		fill_State("Encode");

		//Fills the scaffold out by requesting nodes from the NNet and creating them if they aren't found.
		fill_Scaffold("Encode");

		//To be removed later after testing.
		output_Input();
		output_Scaffold();
		output_Scaffold_Char();

		std::cout << "\n\n-- End Encoding --\n\n";
	}
	
	//Fills out the NULLCAN, does not forcibly encode, and then charges the network.
	void query(uint64_t* p_Input, int p_Depth)
	{
		//Firstly we gather the inputly
		set_Input(p_Input, p_Depth);

		//Set up the scaffold for the nodes to reside in as we build the trace.
		setup_CAN_Scaffold();

		//Work across the state tier to fill it out by requesting state nodes from the NNet, if not found they are created.
		fill_State("Query");

		//Fills the scaffold out by requesting nodes from the NNet and creating them if they aren't found.
		fill_Scaffold("Query");

		//To be removed later after testing.
		output_Input();
		output_Scaffold();
		output_Scaffold_Char();

		std::cout << "\n\n\n";
	}

	//Gets the current treetop at the given index.
	c_Node* get_Treetop(int p_Index = -1)
	{
		//For this network the treetop is the scaffold[1][0] since all lower nodes link to this higher tier node.
		return Scaffold[1][0];
	}

	//Outputs the scaffold.
	void output_Scaffold()
	{
		std::cout << "\n  --==   CAN_Scaffold   ==--";
		std::cout << "\n <- Tier[0] ->";
		for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
		{
			std::cout << "\n   [" << cou_Index << "] " <<  Scaffold[0][cou_Index];
		}
		std::cout << "\n <- Tier[1] ->";
		std::cout << "\n   [0] " << Scaffold[1][0];
	}

	//Outputs the input array.
	void output_Input()
	{
		std::cout << "\n  --==   CAN_Input   ==--";
		for (int cou_Index = 0; cou_Index < Input_Depth; cou_Index++)
		{ 
			std::cout << "\n [" << cou_Index << "] " << Input[cou_Index];
		}
	}

	//Outputs the scaffold as character representing the address.
	void output_Scaffold_Char()
	{
		std::cout << "\n\n(" << this << ")\n";
		for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
		{
			std::cout << char(Scaffold[0][cou_Index]);
		}
		std::cout << "\n" << char(Scaffold[1][0]);
	}
}; 
