#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"

// Tween task struct for procedural animations
struct FTweenTask
{
	float Elapsed = 0.0f;
	float Duration = 1.0f;
	TFunction<void(float)> OnUpdate;
	TFunction<void()> OnComplete;
	UCurveFloat* Curve = nullptr;
	bool bIsComplete = false;

	void Tick(float DeltaTime)
	{
		if (bIsComplete) return;

		Elapsed += DeltaTime;
		float Alpha = FMath::Clamp(Elapsed / Duration, 0.0f, 1.0f);
		float CurveValue = Curve ? Curve->GetFloatValue(Alpha) : Alpha;
		if (OnUpdate) OnUpdate(CurveValue);

		if (Elapsed >= Duration)
		{
			bIsComplete = true;
			if (OnComplete) OnComplete();
		}
	}
};
