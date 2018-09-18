#pragma once

#include <vector>
#include <stack>
#include "Shapes.h"
using namespace std;

class UndoRedo
{
public:
	UndoRedo() {};
	vector <Shapes*> Undo()
	{
		if (!undo.empty())
		{
			redo.push(undo.top());
			undo.pop();
			temp.pop_back();
		}

		return temp;
	}
	vector <Shapes*> Redo()
	{
		if (!redo.empty())
		{
			undo.push(redo.top());
			temp.push_back(redo.top());
			redo.pop();
		}
		return temp;
	}

	void delAll()
	{
		while(!redo.empty())
			redo.pop();
		while(!undo.empty())
			undo.pop();
		while(!temp.empty())
			temp.pop_back();
	}
	void move(Shapes *s)
	{
		temp.push_back(s);
		undo.push(s);
	}

private:
	stack <Shapes*> undo;
	stack <Shapes*> redo;
	vector<Shapes*> temp;
};