// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "SUWindowsMainMenu.h"

#if !UE_SERVER
class UNREALTOURNAMENT_API SUTReplayMenu : public SUWindowsMainMenu
{
	//Disable the frag center
	virtual void SetInitialPanel() override { ; }

	//Remove the play game stuff for now
	virtual void BuildLeftMenuBar() override;

	virtual FReply OnReturnToMainMenu();
	virtual FReply OnCloseMenu();

	virtual void BuildExitMenu(TSharedPtr <SUTComboButton> ExitButton) override;

	virtual EVisibility GetBackVis() const;
	virtual FReply OnShowHomePanel();

protected:
	virtual TSharedRef<SWidget> BuildBackground();
};
#endif
