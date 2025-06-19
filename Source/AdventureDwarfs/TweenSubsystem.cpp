#include "TweenSubsystem.h"
#include "HAL/PlatformTime.h"
#include "FTweenTask.h"

void UTweenSubsystem::Tick(float DeltaTime)
{
	// Use index-based loop to allow safe removal
	for (int32 i = ActiveTweens.Num() - 1; i >= 0; --i)
	{
		FTweenTask& Tween = ActiveTweens[i];
		Tween.Tick(DeltaTime);

		if (Tween.bIsComplete)
		{
			ActiveTweens.RemoveAt(i);
		}
	}
}

TStatId UTweenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTweenSubsystem, STATGROUP_Tickables);
}

void UTweenSubsystem::AddTween(FTweenTask&& Tween)
{
	ActiveTweens.Add(MoveTemp(Tween));
}
