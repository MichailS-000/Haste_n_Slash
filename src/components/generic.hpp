#pragma once

namespace components
{
	struct Transform
	{
		float positionX = 0;
		float positionY = 0;
		float scaleX = 1;
		float scaleY = 1;
	};

	struct RectTransform
	{
		float pivotX = 0.5f;
		float pivotY = 0.5f;
		float alignX = 0;
		float alignY = 0;
		float height = 100;
		float width = 100;
	};
}