function Update()
	if (input.getKeyDown("W")) then
		entity.movePosition(entity.getCurrentEntity(), 0, 1)
	end
	if (input.getKeyDown("S")) then
		entity.movePosition(entity.getCurrentEntity(), 0, -1)
	end
	if (input.getKeyDown("A")) then
		entity.movePosition(entity.getCurrentEntity(), -1, 0)
	end
	if (input.getKeyDown("D")) then
		entity.movePosition(entity.getCurrentEntity(), 1, 0)
	end
end