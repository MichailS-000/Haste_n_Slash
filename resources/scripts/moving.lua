function Start()
	setVariable("transform", getCurrentEntity():getComponent("Transform"))
	setVariable("cameraTransform", getFirstEntityWithComponent("Camera"):getComponent("Transform"))
end

function Update()

	if (input.getKeyDown("W")) then
		getVariable("transform"):movePosition(0, 1)
	end

	if (input.getKeyDown("UP")) then
		getVariable("cameraTransform"):movePosition(0, 1)
	end
end