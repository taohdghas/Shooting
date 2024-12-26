#pragma once
#include "command.h"

class InputHandle
{
public:
	std::vector<Icommand*> HandleInput();
	
	void AssignMoveUpCommandPressKeyW();
	void AssignMoveUpCommandPressKeyS();
    void AssignMoveUpCommandPressKeyA();
	void AssignMoveUpCommandPressKeyD();
	void AssignAttackCommandPressKesSpace();

private:
	Icommand* pressKeyW_;
	Icommand* pressKeyS_;
	Icommand* pressKeyA_;
	Icommand* pressKeyD_;
	Icommand* pressKeySpace_;

};

