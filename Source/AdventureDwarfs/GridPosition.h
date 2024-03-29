// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ADVENTUREDWARFS_API GridPosition
{
public:
	int X;
	int Y;
	int Row;
	int Column;
	GridPosition();
	GridPosition(int PosX, int PosY);
	~GridPosition();
	void SetGridPos(int Row,int Column);
};
