#include "AdjacentDirectionHelper.h"

#include "AdjecantDirections.h"

TArray<EAdjacentDirection> UAdjacentDirectionHelper::GetAllDirections()
{
	return {
		EAdjacentDirection::TopLeft,
		EAdjacentDirection::TopCenter,
		EAdjacentDirection::TopRight,
		EAdjacentDirection::Left,
		EAdjacentDirection::Right,
		EAdjacentDirection::BottomLeft,
		EAdjacentDirection::BottomCenter,
		EAdjacentDirection::BottomRight
	};
}

TArray<FIntPoint> UAdjacentDirectionHelper::GetAllDirectionOffsets()
{
	return {
		FIntPoint(-1,  1),  // TopLeft
		FIntPoint( 0,  1),  // TopCenter
		FIntPoint( 1,  1),  // TopRight
		FIntPoint(-1,  0),  // Left
		FIntPoint( 1,  0),  // Right
		FIntPoint(-1, -1),  // BottomLeft
		FIntPoint( 0, -1),  // BottomCenter
		FIntPoint( 1, -1)   // BottomRight
	};
}

FIntPoint UAdjacentDirectionHelper::GetOffset(EAdjacentDirection Direction)
{
	switch (Direction)
	{
	case EAdjacentDirection::TopLeft:      return FIntPoint(-1,  1);
	case EAdjacentDirection::TopCenter:    return FIntPoint( 0,  1);
	case EAdjacentDirection::TopRight:     return FIntPoint( 1,  1);
	case EAdjacentDirection::Left:         return FIntPoint(-1,  0);
	case EAdjacentDirection::Right:        return FIntPoint( 1,  0);
	case EAdjacentDirection::BottomLeft:   return FIntPoint(-1, -1);
	case EAdjacentDirection::BottomCenter: return FIntPoint( 0, -1);
	case EAdjacentDirection::BottomRight:  return FIntPoint( 1, -1);
	default:                               return FIntPoint(0, 0);
	}
}
