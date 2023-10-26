
//This class serves as the user interface for the goal input
//It encapsulates the concrete data, granulated data, and delta.
//Contains the settings such as the granulation filter, and the data table for the input (1D string of data)
//May implement templates later.
class c_Afferent_Input
{
public:

    //The granulation portion. When an input granularity is needed this calls.
    c_Granulator Granulator;

    //Concrete data is stored internally as an array of double.
    double * Data;

    //Granulated Data & Deltas are stored here at the same depth os the Data.
    double* Granulated;
    double* Delta;

    //The depth of the data.
    int Depth;

    c_Afferent_Input()
    {
        Data = NULL;
        Granulated = NULL;
        Delta = NULL;

        Depth = 0;
    }

    void set_Depth(int p_Depth)
    {
        //If Data is NULL then everything else should be as well.
        if (Data != NULL)
        {
            delete[] Data;
            Data = NULL;

            delete[] Granulated;
            Granulated = NULL;

            delete[] Delta;
            Delta = NULL;
        }

        Depth = p_Depth;

        Data = new double[Depth];
        Granulated = new double[Depth];
        Delta = new double[Depth];

        wipe_Data();
    }


    //This shifts the output from current to 0.
    void wipe_Data()
    {
        for (int cou_Index = 0; cou_Index < Depth; cou_Index++)
        {
            Data[cou_Index] = 0.0f;
            Granulated[cou_Index] = 0.0f;
            Delta[cou_Index] = 0.0f;
        }
    }

    //When adding granulations start at the goal range and work outwards.
    void add_Granulation(double p_Bottom, double p_Top)
    {
        Granulator.add_Var(p_Bottom, p_Top);
    }

    //This gets the delta of the variable at a given index. It cannot calculate the index 0 as there is no data to compare too previously.
    //When the array is shifted the deltas are copied, this way we can still record deltas that fall off from [0]. 
    //If no p_Index is specified it calculates the latest, used after shifting.
    //Assumes index exists.
    void calculate_Delta(int p_Index = -1)
    {
        //If no index is selected it assumes the current index and adjusts accordingly.
        if (p_Index == -1)
        {
            p_Index = Depth - 1;
        }

        if (Data[Depth - 1] < Data[Depth - 2]) { Delta[Depth - 1] = -1; }
        if (Data[Depth - 1] == Data[Depth - 2]) { Delta[Depth - 1] = 0; }
        if (Data[Depth - 1] > Data[Depth - 2]) { Delta[Depth - 1] = 1; }
    }

    //Accepts an input and updates the correct index with it, then granulates it, then deltatizes it.
    void set_Value(double p_Value, int p_Index = -1) //p_Index comes after since it may be 0
    {
        if (p_Index == -1)
        {
            p_Index = Depth - 1;
        }

        //First get the concrete value.
        Data[p_Index] = p_Value;

        //Then the granulated value.
        Granulated[p_Index] = Granulator.get_Granulated(p_Value);

        //The the delta.
        calculate_Delta();
    }

    //Returns the value.
    double get_Value_Data(int p_Index = -1)
    {
        if (p_Index == -1)
        {
            p_Index = Depth - 1;
        }
    
        return Data[p_Index];
    }

    //Returns the value.
    double get_Value_Granulated(int p_Index = -1)
    {
        if (p_Index == -1)
        {
            p_Index = Depth - 1;
        }
    
        return Granulated[p_Index];
    }

    //Returns the value.
    double get_Value_Delta(int p_Index = -1)
    {
        if (p_Index == -1)
        {
            p_Index = Depth - 1;
        }
    
        return Delta[p_Index];
    }

    //This shifts the output from current to 0.
    void shift_Data()
    {
        //Shift each array one index towards [0].
        for (int cou_Index = 0; cou_Index < (Depth - 1); cou_Index++)
        {
            Data[cou_Index] = Data[cou_Index + 1];
            Granulated[cou_Index] = Granulated[cou_Index + 1];
            Delta[cou_Index] = Delta[cou_Index + 1];
        }

        //Set the current to 0.0 in preparation for input.
        Data[Depth - 1] = 0.0f;
        Granulated[Depth - 1] = 0.0f;
        Delta[Depth - 1] = 0.0f;
    }

    //Outputs the data
    void output()
    {
        for (int cou_Index = 0; cou_Index < Depth; cou_Index++)
        {
            std::cout << "\n[" << cou_Index <<"] Data: " << Data[cou_Index] << " << Granulated: " << Granulated[cou_Index] << " << Delta: " << Delta[cou_Index];
        }
    }
};



//This class serves as the user interface for the actuator I/O
class c_Efferent_Input_Output
{
public:

    //Concrete data is stored internally as an array of double.
    double* Data;

    //The depth of the data.
    int Depth;

    c_Efferent_Input_Output()
    {
        Data = NULL;

        Depth = 0;
    }

    void set_Depth(int p_Depth)
    {
        //If Data is NULL then everything else should be as well.
        if (Data != NULL)
        {
            delete[] Data;
            Data = NULL;
        }

        Depth = p_Depth;

        Data = new double[Depth];

        wipe_Data();
    }


    //This shifts the output from current to 0.
    void wipe_Data()
    {
        for (int cou_Index = 0; cou_Index < Depth; cou_Index++)
        {
            Data[cou_Index] = 0.0f;
        }
    }

    //Accepts an input and updates the correct index with it
    void set_Value(double p_Value, int p_Index = -1) //p_Index comes after since it may be 0
    {
        if (p_Index == -1)
        {
            p_Index = Depth - 1;
        }

        //First get the concrete value.
        Data[p_Index] = p_Value;
    }

    //Returns the value.
    double get_Value_Data(int p_Index = -1)
    {
        if (p_Index == -1)
        {
            p_Index = Depth - 1;
        }

        return Data[p_Index];
    }

    //This shifts the output from current to 0.
    void shift_Data()
    {
        //Shift each array one index towards [0].
        for (int cou_Index = 0; cou_Index < (Depth - 1); cou_Index++)
        {
            Data[cou_Index] = Data[cou_Index + 1];
        }

        //Set the current to 0.0 in preparation for input.
        Data[Depth - 1] = 0.0f;
    }

    //Outputs the data
    void output()
    {
        for (int cou_Index = 0; cou_Index < Depth; cou_Index++)
        {
            std::cout << "\n[" << cou_Index << "] Data: " << Data[cou_Index];
        }
    }
};