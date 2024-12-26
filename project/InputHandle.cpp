#include "InputHandle.h"
#include "Input.h"
#include <vector>

void InputHandle::AssignMoveUpCommandPressKeyW() {
	Icommand* command = new MoveUpCommand();
	this->pressKeyW_ = command;
}

void InputHandle::AssignMoveUpCommandPressKeyS() {
	Icommand* command = new MoveDownCommand();
	this->pressKeyS_ = command;
}

void InputHandle::AssignMoveUpCommandPressKeyA() {
	Icommand* command = new MoveLeftCommand();
	this->pressKeyA_ = command;
}

void InputHandle::AssignMoveUpCommandPressKeyD() {
	Icommand* command = new MoveRightCommand();
	this->pressKeyD_ = command;
}

void InputHandle::AssignAttackCommandPressKesSpace() {
	Icommand* command = new AttackCommand();
	this->pressKeySpace_ = command;
}

std::vector<Icommand*> InputHandle::HandleInput() {
    std::vector<Icommand*> commands;

    if (Input::GetInstance()->PushKey(DIK_W)) {
        commands.push_back(pressKeyW_);
    }
    if (Input::GetInstance()->PushKey(DIK_S)) {
        commands.push_back(pressKeyS_);
    }
    if (Input::GetInstance()->PushKey(DIK_A)) {
        commands.push_back(pressKeyA_);
    }
    if (Input::GetInstance()->PushKey(DIK_D)) {
        commands.push_back(pressKeyD_);
    }
    if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
        commands.push_back(pressKeySpace_);
    }

    return commands;
}