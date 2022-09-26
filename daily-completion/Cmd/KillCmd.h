#pragma once
#include "YFramework/Command/ICommand.h"

class CKillCmd : YFramework::ICommand
{
public:
	virtual void Excute() override;
};