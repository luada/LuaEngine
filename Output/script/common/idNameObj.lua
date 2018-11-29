--[[
��������ӵ��Ψһid��Ψһ���ֵĻ���
--]]

require "base.class"
require "base.property"
require "common.idName"

IdNameObj = class(IIdNameObj)

local prop = Property(IdNameObj)
prop:reader("id")
prop:accessor("name")


function IdNameObj:__init(id, name)
	local this = prop[self]
	this.id = id
	this.name = name
end

function IdNameObj:__release()
end