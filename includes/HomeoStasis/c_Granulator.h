
//This class handles data granulation
class c_Granulator
{
public:

	float * Top;
	float * Bottom;
	int Count;

	c_Granulator()
	{
		Top = NULL;
		Bottom = NULL;
		Count = 0;
	}

	void add_Var(float p_Bottom, float p_Top)
	{
		float* tmp_Top;
		float* tmp_Bottom;

		tmp_Top = new float[Count + 1];
		tmp_Bottom = new float[Count + 1];

		for (int cou_Index = 0; cou_Index < Count; cou_Index++)
		{
			tmp_Top[cou_Index] = Top[cou_Index];
			tmp_Bottom[cou_Index] = Bottom[cou_Index];
		}

		if (Top != NULL)
		{
			delete[] Top;
			delete[] Bottom;
		}

		Top = new float[Count + 1];
		Bottom = new float[Count + 1];

		for (int cou_Index = 0; cou_Index < Count; cou_Index++)
		{
			Top[cou_Index] = tmp_Top[cou_Index];
			Bottom[cou_Index] = tmp_Bottom[cou_Index];
		}

		Top[Count] = p_Top;
		Bottom[Count] = p_Bottom;

		delete [] tmp_Top;
		delete [] tmp_Bottom;

		Count++;
	}

	float get_Value(int p_Var, float p_Value)
	{
		//Range goals
		float tmp_Value = 0;

		if (p_Value < Bottom[p_Var]) { tmp_Value = -1.0; }
		if ((p_Value >= Bottom[p_Var]) && (p_Value <= Top[p_Var])) { tmp_Value = 0.0; }
		if (p_Value > Top[p_Var]) { tmp_Value = 1.0; }

		//std::cout << "\n Goal_Index: " << p_Goal;
		//std::cout << " State: >" << tmp_State << "< tmp_Target_Goal_State: >" << tmp_Target_Goal_State_Low << "< >" << tmp_Target_Goal_State_High << "< Delta: >" << tmp_Delta << "<";

		return tmp_Value;
	}
};
