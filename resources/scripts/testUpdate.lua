function Update()
	if input.getKeyDown("W") then
		print("Current time: " .. time.getTime())
	end
	if input.getKeyDown("S") then
		print("Current delta time: " .. time.getDeltaTime())
	end
	if input.getKeyDown("1") then
		print("Current entity: " .. entity.getCurrentEntity())
	end
	if input.getKeyDown("2") then
		entity.destroyEntity(entity.getCurrentEntity())
	end
end
