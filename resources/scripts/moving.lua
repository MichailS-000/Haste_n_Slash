function Update()
	
	transform = entity.getComponent(entity.getCurrentEntity(), "Transform")

	if (input.getKeyDown("W")) then
		transform.positionY = transform.positionY + 1;
	end

end