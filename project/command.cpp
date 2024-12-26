#include "command.h"

void MoveUpCommand::Exec(Player& player) {
	player.MoveUp();
}

void MoveDownCommand::Exec(Player& player) {
	player.MoveDown();
}

void MoveLeftCommand::Exec(Player& player) {
	player.MoveLeft();
}

void MoveRightCommand::Exec(Player& player) {
	player.MoveRight();
}

void AttackCommand::Exec(Player& player) {
    player.Attack();
}
