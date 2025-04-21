function Start()
	setVariable("transform", getCurrentEntity():getComponent("Transform"))
	setVariable("speed", 10)
	setVariable("cameraTransform", getFirstEntityWithComponent("Camera"):getComponent("Transform"))
end

function Update()

	local transform = getVariable("transform")
	local speed = getVariable("speed")

	if (input.getKey("W")) then
		transform:movePosition(0, time.getDeltaTime() * speed)
	end

	if (input.getKey("A")) then
		transform:movePosition(-time.getDeltaTime() * speed, 0)
	end

	if (input.getKey("D")) then
		transform:movePosition(time.getDeltaTime() * speed, 0)
	end

	if (input.getKey("S")) then
		transform:movePosition(0, -time.getDeltaTime() * speed)
	end

end