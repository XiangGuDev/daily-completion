#pragma once
#include "YFramework/Command/BaseCommand.h"

class CKillCmd : public BaseCommand
{
protected:
	virtual void Init() override;
	virtual void Execute() override;
};