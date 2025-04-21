function Start()
	setVariable("transform", getCurrentEntity():getComponent("Transform"))
	setVariable("cameraTransform", getFirstEntityWithComponent("Camera"):getComponent("Transform"))
end

function Update()

	local transform = getVariable("transform")

	if (input.getKeyDown("W")) then
		transform:movePosition(0, 1)
	end

	if (input.getKeyDown("A")) then
		transform.scaleX = transform.scaleX + 0.1
	end

	if (input.getKeyDown("D")) then
		transform.scaleX = transform.scaleX - 0.1
	end

	if (input.getKeyDown("ESCAPE")) then
		local background = getFirstEntityWithComponent("Background"):getComponent("Background")
		background.enabled = not background.enabled
	end
end