function Start()
	setVariable("transform", entity.getComponent(entity.getCurrentEntity(), "Transform"))
end

function Update()
	
	transform = getVariable("transform")

	if (input.getKeyDown("W")) then
		transform.positionY = transform.positionY + 1;
	end

end