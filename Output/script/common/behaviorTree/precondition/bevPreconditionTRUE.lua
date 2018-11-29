--[[Created by Luada
前提条件TRUE类
--]]

BevPreconditionTRUE = class(BevPrecondition)

function BevPreconditionTRUE:__init(name)
	BevPrecondition.__init(self, name)
end

function BevPreconditionTRUE:__release()
end

function BevPreconditionTRUE:load()
end

function BevPreconditionTRUE:test(input)
	return true
end
