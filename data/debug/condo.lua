-- This is just a file for testing the Lua item inteface.

-- Initialize the item info.
local item = {}
item.id = "condo"
item.min_rating = 1
item.w = 16
item.h = 1


-- Create a condo class.
class("CondoItem", Item)

function Item:Item()
	print("constructing Item")
	self.floor = 12;
end

function CondoItem:CondoItem()
	self:Item()
	print("constructing CondoItem")
	self.children = {"Fred", "Anna"}
end

-- Simulation
function CondoItem:simulate(dt)
	print("Simulating a CondoItem (dt = " .. dt .. "s)")
	local sprite = Sprite:new()
	sprite:setPosition(100, 200)
	self:addSprite(sprite)
end


local i = CondoItem:new("Hello, World")
dump(i)


-- Return the item template.
return item
