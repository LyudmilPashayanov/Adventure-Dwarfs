// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPosition.h"

GridPosition::GridPosition()
{
}

GridPosition::GridPosition(int PosX, int PosY) : X{ PosX }, Y{ PosY }
{
}

GridPosition::~GridPosition()
{
}

void GridPosition::SetGridPos(const int row, const int column)
{
	Row = row;
	Column = column;
}
