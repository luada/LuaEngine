--[[Created by Luada
�����
--]]

--------------------------------------------------------------------------------
Player = class()


--------------------------------------------------------------------------------
local prop = Property(Player)


--------------------------------------------------------------------------------
function Player:__init(name)
	--[[
	����
	--]]
	local this = prop[self]
	this._name = name
	this._tickCount = 0
end

function Player:__release()
	--[[
	����
	--]]
	local this = prop[self]
end

function Player:tick()
	--[[
	����tick����
	--]]
	local this = prop[self]
	this._tickCount = this._tickCount + 1
	if this._tickCount % 100 == 0 then
	    print(this._name, this._tickCount, "----xx")
	end
end
