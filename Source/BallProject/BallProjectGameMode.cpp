// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BallProjectGameMode.h"
#include "BallProjectBall.h"

ABallProjectGameMode::ABallProjectGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = ABallProjectBall::StaticClass();
}
