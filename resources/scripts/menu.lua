function Start()
	graphics.addBackground("pic1")
	local entt = entity.addEntity()
	entity.addScriptToEntity("testUpdate", entt)
end