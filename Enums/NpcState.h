#pragma once

enum NpcState
{
	MovingToEndOfPath,
	MovingToStartOfPath,
	Waiting,
	LookUp,
	LookDown,
	LookLeft,
	LookRight
};