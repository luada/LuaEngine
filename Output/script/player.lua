--[[Created by Luada
玩家类
--]]

--------------------------------------------------------------------------------
Player = class()


--------------------------------------------------------------------------------
local prop = Property(Player)


--------------------------------------------------------------------------------
function Player:__init(name)
	--[[
	构造
	--]]
	local this = prop[self]
	this._name = name
	this._tickCount = 0
end

function Player:__release()
	--[[
	析构
	--]]
	local this = prop[self]
end

function Player:tick()
	--[[
	更新tick操作
	--]]
	local this = prop[self]
	this._tickCount = this._tickCount + 1
	if this._tickCount % 100 == 0 then
	    print(this._name, this._tickCount, "----xx")
	end
end
