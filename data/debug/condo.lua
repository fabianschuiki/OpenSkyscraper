-- This is just a file for testing the Lua item inteface.

-- Initialize the item info.
local item = {}
item.id = "condo"
item.min_rating = 1
item.w = 16
item.h = 1

function CondoItem:initAnimation()
	print("init condo anim")
end


-- Return the item template.
return item
