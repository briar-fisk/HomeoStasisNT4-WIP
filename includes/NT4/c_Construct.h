


//The construct encapsulates the node network, the CANs, I/O, granulation filter, uinterface, actuator interface, and I/O tables.
//The result is that after setting up the network the user can use it like a black box.
class c_Construct
{
	//The shared node network.
	c_Node_Network Nodes;

	//The names of each assembly within the construct.
	std::string * Assembly_Names;

	//For each assembly we have an index in the State_Tree[], CAN[], etc.
	int Assembly_Count;

	//This holds the array of CAN scaffold structures.
	//c_Base_CAN is a base class.
	c_Base_CAN** CAN;

	c_Construct()
	{
		Assembly_Count = 0; //Variables dependent on this value: Assembly_Names, CAN
		Assembly_Names = NULL;
		CAN = NULL;
		
	}

	int register_Assembly(std::string p_Assembly_Name)
	{
		std::string* tmp_Names;
		tmp_Names = new std::string[Assembly_Count];

		for (int cou_Index = 0; cou_Index < Assembly_Count; cou_Index++)
		{
			tmp_Names[cou_Index] = Assembly_Names[cou_Index];
		}
		if (Assembly_Names != NULL) { delete[] Assembly_Names; }

		Assembly_Names = new std::string[Assembly_Count + 1];

		for (int cou_Index = 0; cou_Index < Assembly_Count; cou_Index++)
		{
			Assembly_Names[cou_Index] = tmp_Names[cou_Index];
		}

		Assembly_Names[Assembly_Count] = p_Assembly_Name;
		Assembly_Count++;

		//Return the index of the newly created assembly.
		return Assembly_Count - 1;
	}
};