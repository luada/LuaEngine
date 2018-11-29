--[[Created by Luada
复合条件的基类
--]]

BevPreconditionComp = class(BevPrecondition)

function BevPreconditionComp:__init(name, ...)
	BevPrecondition.__init(self, name)
	self.arg_ = table.pack(...)
end

function BevPreconditionComp:__release()
	for _, v in ipairs(self.arg_ or table.empty) do
		v:release()
	end
	self.arg_ = nil
end

function BevPreconditionComp:load(...)
	self.arg_ = table.pack(...)
end

function BevPreconditionComp:test(input)
	return false
end
