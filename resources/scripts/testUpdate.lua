function Update()
	if input.getKeyDown("W") then
		print("Current time: " .. time.getTime())
	end
	if input.getKeyDown("S") then
		print("Current delta time: " .. time.getDeltaTime())
	end
end
